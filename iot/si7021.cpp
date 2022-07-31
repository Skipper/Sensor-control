#include <stdio.h>
#include <math.h>

#include "i2c.h"
#include "si7021.h"

void sensorSi7021(float& temperature, float& humidity){

    unsigned char configHoldMaster[1] = {0xE5};

    i2c_send_bytes(0x40, configHoldMaster, 1);

    // Leer 2 bytes humedad msb, humedad lsb = Intercambio LSB y MSB de elementos diagonales
    unsigned char data[1] = {0};
    i2c_read_bytes(0x40, data, 1);

    if(i2c_read_bytes(0x40, data, 1) != 1){
        printf("Error de entrada / salida del Si7021. \n");
    }else{
        humidity = (((data[0] * 256 + data[1]) * 125.0) / 65536.0) - 6;
        humidity    = roundf (humidity    * 100) / 100;
    }

    configHoldMaster[1] = {0xE0};

    i2c_send_bytes(0x40, configHoldMaster, 1);

    // Leer 2 bytes temperatura msb, humedad lsb = Intercambio LSB y MSB de elementos diagonales
    data[1] = {0};
    i2c_read_bytes(0x40, data, 1);

    if(i2c_read_bytes(0x40, data, 1) != 1){
        printf("Error de entrada / salida del Si7021. \n");
    }else{
        temperature = (((data[0] * 256 + data[1]) * 175.72) / 65536.0) - 46.85;
        temperature = roundf (temperature * 100) / 100;
    }

}
