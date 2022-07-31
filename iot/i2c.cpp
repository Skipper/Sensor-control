#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
//#include <stdint.h>

#include "i2c.h"

#define bus "/dev/i2c-1"

int i2c_send_bytes(int addr, unsigned char* dta, unsigned char len){

    // 1. Abrir puerto
    int archivo;
    if((archivo = open(bus, O_RDWR)) < 0){
        printf("No se pudo abrir el bus I2C. \n");
        return -1;
    }
    // 2. Obtener el dispositivo I2C, la dirección SI7021 I2C es 0x40
    if(ioctl (archivo, I2C_SLAVE, addr) < 0){
        printf ("No se puede acceder al bus para interactuar con el esclavo. \n");
        return -1;
    }

	/* 3. Iniciar la medición enviando un bit cero */
    int resultado = write(archivo, dta, len);

    close(archivo);
    return resultado;

}

int i2c_read_bytes(int addr, unsigned char* dta, unsigned char len){

    int archivo;
    if((archivo = open(bus, O_RDWR)) < 0){
        printf("No se pudo abrir el bus I2C. \n");
        return -1;
    }

    if(ioctl (archivo, I2C_SLAVE, addr) < 0){
        printf ("No se puede acceder al bus para interactuar con el esclavo. \n");
        return -1;
    }

    int resultado = read(archivo, dta, len);

    close(archivo);
    return resultado;

}
