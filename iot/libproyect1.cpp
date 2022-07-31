#include <wiringPi.h>
#include <softPwm.h>

#include "libproyect1.h"
#include "si7021.h"
#include "am2320.h"
#include "TCS3472.h"
#include "lcd.h"

int pin_LDRA, pin_LDRB, pin_LEDR, pin_LEDG, pin_LEDB;

rgb_lcd lcd;

void initResources (int pinLEDR, int pinLEDG, int pinLEDB){

    inicializarTCS3472();
    wiringPiSetup();

    pin_LEDR = pinLEDR;
    pin_LEDG = pinLEDG;
    pin_LEDB = pinLEDB;

    softPwmCreate(pin_LEDR, 0, 255);
    softPwmCreate(pin_LEDG, 0, 255);
    softPwmCreate(pin_LEDB, 0, 255);

    lcd = rgb_lcd();
    lcd.begin(16, 2);

/*    delay (50);
    lcd.noBlinkLED();
    lcd.setRGB(255, 255, 255);
    lcd.setCursor(0, 0);
    lcd.setText("               ");
    delay(50);
    lcd.setCursor(0, 1);
    lcd.setText("                ");
    lcd.autoscroll();
    lcd.leftToRight();
    setLCDBackLight(0, 0, 0);
    delay(900); */

}

void readSi7021(float& temperature, float& humidity){
    sensorSi7021(temperature, humidity);
}

void readAM2320(float& temperature, float& humidity){
    sensorAM2320(temperature, humidity);
}

void readColor(int &clearColor, int& red, int& green, int& blue){
    sensorTCS3472(clearColor, red, green, blue);
}

void setLCDBackLight(unsigned char r, unsigned char g, unsigned char b) {
    lcd.setRGB(r, g, b);
}

void setLCDBlink(unsigned char dutyCycle, unsigned char period){
    lcd.blinkLED(dutyCycle, period);
}

void setLCDNoBlink() {
    lcd.noBlinkLED();
}

void setLCDText(char* text) {
    lcd.setText(text);
}

void setLCDCursor(uint8_t where_col, uint8_t where_line) {
    lcd.setCursor(where_col, where_line);
}

void setLCDAutoscroll() {
    lcd.autoscroll();
}

void setLCDNoAutoscroll() {
    lcd.noAutoscroll();
}

void setRGB(unsigned char r, unsigned char g, unsigned char b) {
    softPwmWrite(pin_LEDR, r);
    softPwmWrite(pin_LEDG, g);
    softPwmWrite(pin_LEDB, b);
}

void setClear(){
    lcd.clear();
}
