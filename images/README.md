## Main Program

The main program uses the libproyect1.so dynamic library, which compiles the functionalities of all the modules implemented for the different devices mentioned above, to take measurements of temperature, humidity and color, and based on the results print certain values ​​or do that the screen and/or the rgb led act in a certain way. Its basic operation will be as follows:

* 2 measurements of Temperature, Humidity and Color will be taken every 1 second.
* The results of the measurements will be shown on the LCD screen and by console (showing in the latter, in addition, the Date and time and the dominant color)
* Every 1 minute, statistics (Max, Min, Mean, Std) linked to Temperature and Humidity will be displayed on the screen, as well as the most dominant color during the last minute.
* Normal LCD backlight color is white.
* LCD and RGB backlight brightness is controlled by the -clear value coming from the color sensor (the higher the brightness, the lower the brightness)
* If the temperature is over 30°, the LCD backlight will turn red.
* If the humidity is above 60%, the LCD backlight will turn blue.
* If both values ​​are above these parameters at the same time, the LCD backlight will turn green.
* If any of the above conditions continue for 5 seconds or more, the LCD backlight flashes.
* LCD backlight flashes at a frequency of 2 Hz. (2 in 1 second)
* RGB represents the most dominant color according to the color sensor.

It has, in turn, defined some of its own functions to calculate statistics or perform some additional functionality. And, on the other hand, it allows certain parameters for the user to choose, for example, how long it will run for, what temperature and humidity sensor it uses or what cycle/period it will flash on the screen.
At the end of the data collection cycle (parameterizable), an informative message is displayed on the LCD screen stating that the project has finished, the time elapsed during execution in hours, minutes and seconds, and the RGB bulb turns off, color of screen background and the text that it showed in the two lines, in order to prolong the useful life of the devices by avoiding that any value remains when it is disconnected.
