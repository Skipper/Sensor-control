/*
    rgb_lcd.cpp

    Author:Sergio Esquembri
	Based on Loovee code
    2020-10

    Adapted for RPI use

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.1  USA
*/

#include "lcd.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "i2c.h"
//#include "libproyect1.h"

#include <ctime>

rgb_lcd::rgb_lcd() {}

void rgb_lcd::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {

    if (lines > 1) {
        _displayfunction |= LCD_2LINE;
    }
    _numlines = lines;
    _currline = 0;

    // for some 1 line displays you can select a 10 pixel high font
    if ((dotsize != 0) && (lines == 1)) {
        _displayfunction |= LCD_5x10DOTS;
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    usleep(50000);


    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    command(LCD_FUNCTIONSET | _displayfunction);
    usleep(4500);  // wait more than 4.1ms

    // second try
    command(LCD_FUNCTIONSET | _displayfunction);
    usleep(150);

    // third go
    command(LCD_FUNCTIONSET | _displayfunction);


    // finally, set # lines, font size, etc.
    command(LCD_FUNCTIONSET | _displayfunction);

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // clear it off
    clear();

    // Initialize to default text direction (for romance languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    command(LCD_ENTRYMODESET | _displaymode);


    // backlight init
    setReg(REG_MODE1, 0);
    // set LEDs controllable by both PWM and GRPPWM registers
    setReg(REG_OUTPUT, 0xFF);
    // set MODE2 values
    // 0010 0000 -> 0x20  (DMBLNK to 1, ie blinky mode)
    setReg(REG_MODE2, 0x20);

    setColorWhite();

}

/********** high level commands, for the user! */
void rgb_lcd::clear() {
    command(LCD_CLEARDISPLAY);        // pantalla clara, ajuste la posición del cursor a cero
    usleep(2000);          // ¡este comando lleva mucho tiempo!
}

void rgb_lcd::home() {
    command(LCD_RETURNHOME);        // establecer la posición del cursor a cero
    usleep(2000);        // ¡este comando lleva mucho tiempo!
}

void rgb_lcd::setCursor(uint8_t col, uint8_t row) {

    col = (row == 0 ? col | 0x80 : col | 0xc0);
    unsigned char dta[2] = {0x80, col};

    i2c_send_bytes(LCD_ADDRESS, dta, 2);
}

// Enciende / apaga la pantalla (rápidamente)
void rgb_lcd::noDisplay() {
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void rgb_lcd::display() {
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Activa / desactiva el cursor de subrayado
void rgb_lcd::noCursor() {
    _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void rgb_lcd::cursor() {
    _displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Enciende y apaga el cursor parpadeante
void rgb_lcd::noBlink() {
    _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void rgb_lcd::blink() {
    _displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Estos comandos desplazan la pantalla sin cambiar la RAM
void rgb_lcd::scrollDisplayLeft(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void rgb_lcd::scrollDisplayRight(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// Esto es para texto que fluye de izquierda a derecha
void rgb_lcd::leftToRight(void) {
    _displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// Esto es para texto que fluye de derecha a izquierda
void rgb_lcd::rightToLeft(void) {
    _displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void rgb_lcd::autoscroll(void) {
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void rgb_lcd::noAutoscroll(void) {
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void rgb_lcd::createChar(uint8_t location, uint8_t charmap[]) {

    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));

    unsigned char dta[9];
    dta[0] = 0x40;
    for (int i = 0; i < 8; i++) {
        dta[i + 1] = charmap[i];
    }
    i2c_send_bytes(LCD_ADDRESS,dta, 9);
}

// Control the backlight LED blinking
/*void rgb_lcd::blinkLED(void) {
    // blink period in seconds = (<reg 7> + 1) / 24
    // on/off ratio = <reg 6> / 256
    setReg(0x07, 0x17);  // blink every second
    setReg(0x06, 0x7f);  // half on, half off
}*/

void rgb_lcd::blinkLED(unsigned char dutyCycle, unsigned char period){
// ciclo de trabajo: 0-255 Freq: 24/period+1
     // período de parpadeo en segundos = (<reg 7> + 1) / 24
     // relación de encendido / apagado = <reg 6> / 256
     setReg (REG_RATIO, period); // parpadea cada segundo
     setReg (REG_DUTY, dutyCycle); // mitad encendido, mitad apagado
}
void rgb_lcd::noBlinkLED(void) {
    setReg(0x07, 0x00);
    setReg(0x06, 0xff);
}

/*********** mid level commands, for sending data/cmds */
/*********** comandos de nivel medio, para enviar datos / cmds */

// send command
inline void rgb_lcd::command(uint8_t value) {
    unsigned char dta[2] = {0x80, value};
    i2c_send_bytes(LCD_ADDRESS,dta, 2);
}

// send data
inline size_t rgb_lcd::write_char(uint8_t value) {

    unsigned char dta[2] = {0x40, value};
    i2c_send_bytes(LCD_ADDRESS,dta, 2);
    return 1; // assume sucess
}

void rgb_lcd::setText(char* text){

    for(int i = 0; i< strlen(text); ++i){
        write_char(text[i]);
    }

}

void rgb_lcd::setReg(unsigned char addr, unsigned char dta) {
    unsigned char buffer[2];
    buffer[0] = addr;
    buffer[1] = dta;
    i2c_send_bytes(RGB_ADDRESS,buffer,2);
}

void rgb_lcd::setRGB(unsigned char r, unsigned char g, unsigned char b) {
    setReg(REG_RED, r);
    setReg(REG_GREEN, g);
    setReg(REG_BLUE, b);
}

const unsigned char color_define[4][3] = {
    {255, 255, 255},            // white
    {255, 0, 0},                // red
    {0, 255, 0},                // green
    {0, 0, 255},                // blue
};

void rgb_lcd::setColor(unsigned char color) {
    if (color > 3) {
        return ;
    }
    setRGB(color_define[color][0], color_define[color][1], color_define[color][2]);
}



