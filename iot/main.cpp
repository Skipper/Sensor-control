#include <iostream>
#include <string.h>
#include <wiringPi.h>
#include <math.h>
#include <chrono>
#include <thread>
#include "lcd.h"

#include "iot.h"

#define pRed 0      // 17
#define pGreen 2    // 27
#define pBlue 3     // 22
#define cantidadSegundos 122
#define sensorTemperaturaHumedad "AM2320" // Si7021  - AM2320
#define periodoParpadeo 47 // +1/24 Nos define cuantos segundos demorarà el parpadeo, desde que apagado-prendido-apagado
#define cicloParpadeo 127 // 0...255 Nos define cuànto tiempo estarà encendido y cuanto apagado durante el parpadeo (a mayor ciclo mas tiempo encendido)


/*
* ------------------------------------------------------------------------------------------------------------------------
*                                                          V A R I A B L E S
* ------------------------------------------------------------------------------------------------------------------------
*/
    using namespace std;
    using namespace std::this_thread;
    using namespace std::chrono_literals;
    using std::chrono::system_clock;

    float temperatureMax = 0.0f,
          temperatureMin = 100.0f,
          temperature = 0.0f;

    float humidityMax = 0.0f,
          humidityMin = 100.0f,
          humidity = 0.0f;

    int clearColor, red, green, blue;

    int pruebaConstante     = 0,
        pruebaProlongada    = 0;

    float temperatura[120];
    float humedad[120];

    int contador30      = 0,
        contador60      = 0,
        contador3060    = 0;

    int contadorRed     = 0,
        contadorGreen   = 0,
        contadorBlue    = 0;

    string stringTyH = "";
    string stringRGB = "";

    int segundos = cantidadSegundos;

/*
* ------------------------------------------------------------------------------------------------------------------------
*                                                      M   E   T   O   D   O   S
* ------------------------------------------------------------------------------------------------------------------------
*/
    void initDisplay();
    void StartProject();
    void CalculaEstadistica(float arregloTemperatura[120], float arregloHumedad[120]);
    void DisplayTemperatureAndHumidityonLCD(string stringTyH, string temperaturaString, string humedadString);
    void DisplayRGBonLCD(string stringRGB, int red, int green, int blue, char* redChar, char* greenChar, char* blueChar);
    void FinDelProyecto();

int main(){

    initResources(pRed, pGreen, pBlue);
    initDisplay();
    StartProject();

    segundos *= 2;
    for (int i=0; i < segundos; i++){

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        readColor(clearColor, red, green, blue);

        if (sensorTemperaturaHumedad == "Si7021"){
            readSi7021(temperature, humidity);
        }else if(sensorTemperaturaHumedad == "AM2320"){
            readAM2320(temperature, humidity);
        }

        string temperaturaString = std::to_string(temperature);
        std::size_t posicionPunto = temperaturaString.find(".");
        string entero = temperaturaString.substr(0,posicionPunto);
        posicionPunto ++;
        string decimal = temperaturaString.substr(posicionPunto,1);
        temperaturaString = entero + ".";// + decimal;
        temperaturaString += decimal;

        string humedadString = std::to_string(humidity);
        posicionPunto = humedadString.find(".");
        entero = humedadString.substr(0,posicionPunto);
        posicionPunto ++;
        decimal = humedadString.substr(posicionPunto,1);
        humedadString = entero + ".";// + decimal;
        humedadString += decimal;

        if ((temperature > 30) && (humidity > 60)) {
            setRGB(0, 255, 0);
            setLCDBackLight(0, 255, 0);
            contador3060 += 1;
            contador30       = 0;
            contador60 = 0;
            printf("Temperatura y Humedad mayor a 30 y 60 \n");
        } else if (temperature > 30) {
            setRGB(255, 0, 0);
            setLCDBackLight(255, 0, 0);
            contador3060 = 0;
            contador30   += 1;
            contador60   = 0;
            printf("Temperatura mayor a 30 \n");
        } else if (humidity > 60) {
            setRGB(0, 0, 255);
            setLCDBackLight(0, 0, 255);
            contador3060 = 0;
            contador30   = 0;
            contador60  += 1;
            printf("Humedad mayor a 60 \n");
        } else {
            setLCDBackLight(clearColor, clearColor, clearColor);
            contador3060    = 0;
            contador30      = 0;
            contador60      = 0;
        }

        pruebaConstante +=1; // 1 equivale a medio segundo
        pruebaProlongada += 1;

        temperatura[pruebaProlongada]   = temperature;
        humedad[pruebaProlongada]       = humidity;

        if (pruebaConstante == 1) {

            printf("%02d-%02d-%d %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
            printf("Humidity: %.2f %% \n", humidity);
            printf("Temperature: %.2f °C \n", temperature);
            printf("Red:%d  Green:%d  Blue:%d \n", red, green, blue);
            printf("Clear: %d \n", clearColor);

            if (red > green && red > blue){
                if(red   > 255) { red     = 255; }
                printf("Color dominante: RED \n");
                setRGB(red, 0, 0);
                contadorRed += 1;
            } else if (green > red && green > blue){
                if(green > 255) { green   = 255; }
                printf("Color dominante: GREEN \n");
                setRGB(0, green, 0);
                contadorGreen += 1;
            } else if (blue > red && blue > green){
                if(blue  > 255) { blue    = 255; }
                printf("Color dominante: BLUE \n");
                setRGB(0, 0, blue);
                contadorBlue += 1;
            } else if (red == green){
                printf("Color dominante: Red - Green \n");
                setRGB(red, green, 0);
            } else if (red == blue){
                printf("Color dominante: Red - Blue \n");
                setRGB(red, 0, blue);
            } else if (blue == green){
                printf("Color dominante: Blue - Green \n");
                setRGB(0, green, blue);
            }
            printf("\n");

            char* redChar = (char*) std::to_string(red).c_str();
            char* greenChar = (char*) std::to_string(green).c_str();
            char* blueChar = (char*) std::to_string(blue).c_str();

            if (contador3060 >= 10 or contador60 >=10 or contador30 >= 10) {
                //setLCDBlink(127, 23); // 0x7F	127, 0x17	23
                setLCDBlink(cicloParpadeo, periodoParpadeo);
            }else {
                setLCDBlink(255, 0);
            }

            setLCDNoAutoscroll();
            setLCDCursor(0, 0);
            DisplayTemperatureAndHumidityonLCD(stringTyH, temperaturaString, humedadString);

            setLCDCursor(0, 1);
            DisplayRGBonLCD(stringRGB, red, green, blue, redChar, greenChar, blueChar);

            pruebaConstante = 0;

        } // End Prueba Constante

        if (pruebaProlongada == 120){

            CalculaEstadistica(temperatura, humedad);

            pruebaProlongada = 0;

        } // End Prueba Prolongada

        sleep_for(450ms);

    } // End For

    FinDelProyecto();

} // End Main

/*
* ------------------------------------------------------------------------------------------------------------------------
*                                               M   E   T   O   D   O   S
* ------------------------------------------------------------------------------------------------------------------------
*/
void initDisplay(){

    rgb_lcd screen = rgb_lcd();
    screen.begin (16, 2);

    delay (50);
    screen.noBlinkLED();
    screen.setRGB(255, 255, 255);
    screen.setCursor(0, 0);
    screen.setText(" Inicializando ");
    delay(50);
    screen.setCursor(0, 1);
    screen.setText("Por favor espere");
    screen.autoscroll();
    screen.leftToRight();
    delay (1900);

}

void StartProject(){

    setLCDBackLight(255, 255, 255);
    setLCDBlink(255,0);
    setLCDNoAutoscroll();
    delay(50);
    setLCDCursor(0, 0);
    setLCDText("  Proyecto IOT  ");
    delay(50);
    setLCDCursor(0, 1);
    setLCDText(" Presentado por ");
    delay(1900);

    delay(50);
    setLCDCursor(0, 0);
    setLCDText("Francisco Lupi  ");
    delay(50);
    setLCDCursor(0, 1);
    setLCDText("Diego Martinez  ");
    delay (1900);

}

void CalculaEstadistica (float arregloTemperatura[120], float arregloHumedad[120]){

    float maxTemp = 0.0;
    float minTemp = 100.0;
    float sumaTemp = 0.0;
    float tempMedia = 0.0;

    float maxHum = 0.0;
    float minHum = 100.0;
    float sumaHum = 0.0;
    float humMedia = 0.0;

    float sumaDistMedTemp = 0.0;
    float sumaDistMedHum = 0.0;

    float desvioTemp = 0.0;
    float desvioHum = 0.0;
    string stringColorDominante = "";

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    for (int i = 1; i <= 120; i++){
        if (maxTemp < arregloTemperatura[i]){
            maxTemp = arregloTemperatura[i];
        }
        if (minTemp > arregloTemperatura[i]){
            minTemp = arregloTemperatura[i];
        }
        sumaTemp += arregloTemperatura[i];

        if (maxHum < arregloHumedad[i]){
            maxHum = arregloHumedad[i];
        }
        if (minHum > arregloHumedad[i]){
            minHum = arregloHumedad[i];
        }
        sumaHum += arregloHumedad[i];
    }

    tempMedia   = (sumaTemp) / 120.0;
    humMedia    = (sumaHum)  / 120.0;

    for (int i = 1; i <= 120; i++){

        sumaDistMedTemp += pow((arregloTemperatura[i] - tempMedia),2);
        sumaDistMedHum  += pow((arregloHumedad[i]     - humMedia), 2);

    }

    desvioTemp = sqrt((sumaDistMedTemp/120.0));
    desvioHum = sqrt((sumaDistMedHum/120.0));

    if (contadorRed > contadorGreen && contadorRed > contadorBlue){
        stringColorDominante = "Dominant Color During Last minute: Red \n";
    }else if(contadorGreen > contadorRed && contadorGreen > contadorBlue){
        stringColorDominante = "Dominant Color During Last minute: Green \n";
    }else if (contadorBlue > contadorRed && contadorBlue > contadorGreen){
        stringColorDominante = "Dominant Color During Last minute: Blue \n";
    }else{
        stringColorDominante = "Unable to determine dominant color. \n";
    }

    printf("____________________________________________________________________\n");
    printf("%02d-%02d-%d %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    printf("Temperature: %.2f °C \n", temperature);
    printf("Temperature Max/Min/Mean/Std (%%): %.2f / %.2f / %.2f / %.2f \n", maxTemp, minTemp, tempMedia, desvioTemp);

    printf("Humidity: %.2f %% \n", humidity);
    printf("Humidity Max/Min/Mean/Std (%%): %.2f / %.2f / %.2f / %.2f \n", maxHum, minHum, humMedia, desvioHum);

    cout << stringColorDominante;
    printf("____________________________________________________________________\n\n");

    contadorRed = 0;
    contadorGreen = 0;
    contadorBlue = 0;

}

void DisplayTemperatureAndHumidityonLCD(string stringTyH, string temperaturaString, string humedadString){

    stringTyH = "T:"; // Temperatura
    stringTyH += temperaturaString;
    stringTyH += "\xDF";
    stringTyH += "C";
    stringTyH += " H:"; // Humedad
    stringTyH += humedadString;
    stringTyH += "%";

    setLCDText((char*) stringTyH.c_str());
}

void DisplayRGBonLCD(string stringRGB, int red, int green, int blue, char* redChar, char* greenChar, char* blueChar){
    /* Añade 0 al inicio si el color es menor a 100, ejemplo 99 = 090 */
    stringRGB = " R"; // Red
    if (red >= 100 && red <=999){
        stringRGB += redChar;
    }else if (red >= 10 && red <=99){
        stringRGB += "0";
        stringRGB += redChar;
    }else if (red >= 1 && red <=9){
        stringRGB += "00";
        stringRGB += redChar;
    }

    stringRGB += " G"; // Green
    if (green >= 100 && green <=999){
        stringRGB += greenChar;
    }else if (green >= 10 && green <=99){
        stringRGB += "0";
        stringRGB += greenChar;
    }else if (green >= 1 && green <=9){
        stringRGB += "00";
        stringRGB += greenChar;
    }

    stringRGB += " B"; // Blue
    if (blue >= 100 && blue <=999){
        stringRGB += blueChar;
    }else if (blue >= 10 && blue <=99){
        stringRGB += "0";
        stringRGB += blueChar;
    }else if (blue >= 1 && blue <=9){
        stringRGB += "00";
        stringRGB += blueChar;
    }
    stringRGB += " ";

    setLCDText((char*) stringRGB.c_str());
}

void FinDelProyecto(){
    delay(50);
    setLCDBlink(255,0);
    setLCDBackLight(clearColor, clearColor, clearColor);
    setLCDCursor(0, 0);
    setLCDText("Ha transcurrido ");
    delay(50);
    setLCDCursor(0, 1);
    setLCDText("2 min, 5 seg.   ");
    delay (2900);

    setRGB(0, 0, 0);
    delay(50);
    setLCDCursor(0, 0);
    setLCDText("Fin del proyecto");
    delay(50);
    setLCDCursor(0, 1);
    setLCDText(" Muchas gracias ");
    delay (2900);

    setLCDBackLight(0, 0, 0);
    delay(50);
    setLCDCursor(0, 0);
    setLCDText("                ");
    delay(50);
    setLCDCursor(0, 1);
    setLCDText("                ");
    delay (1900);
}
