#include <unistd.h>
#include <iostream>

#include "i2c.h"

int length;
uint8_t buffer[8];
uint8_t addr = 0x5C;

void sensorAM2320(float& temperature, float& humidity){

    i2c_send_bytes(addr,NULL,0);
    //usleep(1000);

    buffer[0]= 0x03;
    buffer[1]= 0x00;
    buffer[2]= 0x04;

    i2c_send_bytes(addr,buffer,3);

    usleep(1600);

    length = 8;

    i2c_read_bytes(addr, buffer, length);

    uint16_t hum = ((buffer[2] << 8) | buffer[3]);
    humidity = (float)(hum / 10.0);
    uint16_t temp = ((buffer[4] << 8) | buffer[5]);
    temperature = (float)(temp / 10.0);

}


