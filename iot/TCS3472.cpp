#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#include "i2c.h"
#include "TCS3472.h"

void inicializarTCS3472 (){

    //Command Register mando 1 y luego todos 0 --> 1000 0000 (0x80)
    //Activamos RGBC y ponemos el PON en 1 --> 0000 0011 (0x03)

    // Seleccione habilitar registro (0x80)
    // Encendido, habilitación de RGBC, deshabilitación del tiempo de espera (0x03)
    unsigned char config[2];
                  config[0] = 0x80;
                  config[1] = 0x03;

    i2c_send_bytes(0x29, config, 2);


    //Como ya estamos en 0x80 empezamos a escribir a partir de ahi (para setear ATIME tengo que hacer 0x80 + 0x01), le mandamos 0x00 (700 ms)
    // atime = 700 ms (0x00)
    config[0] = 0x81;
    config[1] = 0x00;
    i2c_send_bytes(0x29, config, 2);

    // Registro de tiempo de espera (0x83)
    // wtime: 2.4ms (0xFF)
    config[0] = 0x83;
    config[1] = 0xFF;
    i2c_send_bytes(0x29, config, 2);

    // Registro de control (0x8F)
    // gain = 1x (0x00)
    config[0] = 0x8F;
    config[1] = 0x00;
    i2c_send_bytes(0x29, config, 2);
    //sleep(1);
}
void sensorTCS3472(int &clearColor, int& red, int& green, int& blue){

    unsigned char reg[1];

    reg[0] = 0x94;
    i2c_send_bytes(0x29, reg, 1);

    unsigned char data[8];

    if(i2c_read_bytes(0x29, data, 8) != 8){
        printf("Error de entrada / salida del TCS3472. \n");
    }else{

        // Convertir los datos
        float cc  = (data[1] << 8 | data[0]);
        cc = (float)(cc/65535)*255;
        float rc  = (data[3] << 8 | data[2]);
        rc = (float)(rc/65535)*255;
        float gc  = (data[5] << 8 | data[4]);
        gc = (float)(gc/65535)*255;
        float bc  = (data[7] << 8 | data[8]);
        bc = (float)(bc/65535)*255;

        clearColor = cc;
        red = rc;
        green = gc;
        blue = bc;

    }
}
