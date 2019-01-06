# Recursos de salida BME280 (2) Thinger.io.

La placa de desarrollo utilizada es la Adafruit Feather HUZZAH con ESP8266.  
Como IDE uso PlatformIO que es un ecosistema open source para desarrollos IoT.  

## Descripción del proyecto

Sensor BME280 en modo sueño profundo, cada aproximadamente una hora se despierta envia datos a Thinger.io y los almacena en una base de datos.

## Esquema del proyecto

![Esquema proyecto](https://raw.githubusercontent.com/moisesfa/Proyectos_ESP8266/master/ThingerIO_Rec_Salida/Rec_Salida_02/img/adafruit_deep_sleep.png)

## Notas del proyecto

1. Hay algunos problemas de funcionamiento si se producen fluctuaciones en la alimentación inicial, se ha comprobado al conectar el polímetro con las pinzas para ver las medidas.

2. Se puede poner esta instrucción al principio de todo el programa para ver el debug de la librería de Thinger.io.
```c
#define _DEBUG_
```
Indica muchos detalles de como se lleva a cabo la conexión. 

3. Para la lectura de la batería: [Learn de Adafruit](https://learn.adafruit.com/using-ifttt-with-adafruit-io/wiring#battery-tracking)

4. Union que hay que realizar para que funcione el Deep Sleep [Blog de Adafruit](https://blog.adafruit.com/2018/07/17/put-the-adafruit-huzzah-feather-esp8266-to-deep-sleep-with-one-wire-and-one-line-of-code/)


## Consumos 

Como se puede ver, los consumos no son nada bajos para un sistema que pretende alimentarse con baterías. 

Análisis de consumos:

* Consumo en Deep Sleep del sistema: 6,94 mA.
* Deep Sleep del ESP8266 según características: 20 μA, los componentes del la placa Adafruit Feather consumen el resto.
* Sin medición de batería: 6,86 (0,08 mA menos).
* Sin Sensor BME280 : 6,49 (0,45 mA menos).
* Al transmitir alcanza unos 80 mA pero es tan rápido que no se puede apreciar bien el consumo. 

El Deep Sleep del microcontrolador no es suficiente para alcanzar un bajo consumo. Hay que pensar en apagar/dormir todos los componentes que intervienen en el circuito. Siguiente meta. 


## Licencia

Este proyecto se presenta bajo [Creative Commons 0](https://creativecommons.org/publicdomain/zero/1.0/) (o dominio público en otras palabras). 

Todo el código está bajo [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html). (o garantizar a los usuarios finales (personas, organizaciones, compañías) la libertad de usar, estudiar, compartir (copiar) y modificar el software. Protegido de intentos de apropiación que restrinjan esas libertades a nuevos usuarios).