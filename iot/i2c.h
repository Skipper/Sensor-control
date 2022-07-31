#ifndef I2C_H_
#define I2C_H_

int i2c_send_bytes(int addr, unsigned char* dta, unsigned char len);
int i2c_read_bytes(int addr, unsigned char* dta, unsigned char len);

#endif
