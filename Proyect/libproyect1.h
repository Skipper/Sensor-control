#ifndef LIBPROYECT1_H_INCLUDED
#define LIBPROYECT1_H_INCLUDED

#include <stdint.h>

    void initResources (int pinLEDR, int pinLEDG, int pinLEDB);
    void readAM2320(float& temperature, float& humidity);
    void readSi7021(float& temperature, float& humidity);
    void readColor(int &clearColor, int& red, int& green, int& blue);
    void setLCDBackLight(unsigned char r, unsigned char g, unsigned char b);
    void setLCDBlink(unsigned char dutyCycle, unsigned char period);
        void setLCDNoBlink();
    void setRGB(unsigned char r, unsigned char g, unsigned char b);
    void setLCDText(char* text);
        void setLCDCursor(uint8_t where_col, uint8_t where_line);
        void setLCDAutoscroll(); // Opcional
        void setLCDNoAutoscroll(); // Opcional
        void setClear(); // Pendiente por completar

#endif // LIBPROYECT1_H_INCLUDED

