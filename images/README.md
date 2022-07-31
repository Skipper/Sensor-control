## Programa Principal

El programa principal se vale de la librería dinámica libproyect1.so, que recopila las funcionalidades de todos los módulos implementados para los diferentes dispositivos antes mencionados, para tomar mediciones de temperatura, humedad y color, y en base a los resultados imprimir ciertos valores o hacer que la pantalla y/o el led rgb actúen de una determinada manera. El funcionamiento básico del mismo será el siguiente:

* Se tomarán 2 mediciones de Temperatura, Humedad y Color cada 1 segundo.
* Los resultados de las mediciones se mostrarán en la pantalla LCD y por consola (mostrandose en esta ultima, ademas, la Fecha y la hora y el color dominante)
* Cada 1 minuto se mostrarán por pantalla estadísticas (Max, Min, Mean, Std) vinculadas a la Temperatura y la Humedad, así como el color más dominante durante el último minuto.
* El color Normal de la luz de fondo del LCD es blanco.
* El brillo de la luz de fondo del LCD y del RGB está controlado por el valor del -clear proveniente del sensor de color (a mayor luminosidad, menor brillo)
* Si la temperatura está sobre 30°, la luz de fondo del LCD pasará a rojo.
* Si la humedad está por encima de 60 %, la luz de fondo del LCD pasará a azul.
* Si los dos valores se encuentran por encima de esos parámetros, a la vez, la luz de fondo del LCD pasará a verde.
* Si cualquiera de las anteriores condiciones se mantiene por 5 o más segundos, la luz de fondo del LCD parpadea.
* La luz de fondo del LCD parpadea con una frecuencia de 2 Hz. (2 en 1 segundo)
* El RGB representa el color mas dominante de acuerdo al sensor de color.

Tiene, a su vez, definidas algunas funciones propias del mismo para calcular estadísticas o realizar alguna funcionalidad adicional. Y, por otro lado, permite ciertas parametrizaciones para que el usuario elija, por ejemplo, por cuánto tiempo se ejecutará, qué sensor de temperatura y humedad emplea o qué ciclo/período le dará al parpadeo de pantalla.
Al terminar el ciclo de toma de datos (parametrizable), se muestra en la pantalla LCD un mensaje informativo de que el proyecto ha terminado, el tiempo transcurrido durante la ejecución en horas, minutos y segundos y se realiza el apagado del bombillo RGB, color de fondo de pantalla y el texto que mostraba en las dos líneas, con el fin de prolongar la vida útil de los dispositivos al evitar que quede algún valor al momento de desconectarlo.
