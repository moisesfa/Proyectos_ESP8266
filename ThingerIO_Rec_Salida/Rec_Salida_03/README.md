# Recursos de salida BME280 (3) Thinger.io.

La placa de desarrollo utilizada es la Adafruit Feather HUZZAH con ESP8266.  
Como IDE uso PlatformIO que es un ecosistema open source para desarrollos IoT.  

## Descripción del proyecto

TPL5110 temporizando el envío de datos del sensor BME280 a la plataforma Thinger.io. 

El temporizador de potencia TPL5110 de Adafruit es un interruptor independiente que convertirá cualquier electrónica en una electrónica de bajo consumo. Se encargará de encender / apagar su proyecto usando un temporizador incorporado que puede variar desde una vez cada 100 ms hasta una vez cada dos horas. 

Básicamente, el TPL se encenderá periódicamente, se podrá ajustar mediante un potenciómetro o una resistencia, y se activará la alimentación de su proyecto. Luego esperará hasta que se reciba una señal del proyecto para indicar al TPL que puede apagar la alimentación de manera segura.

[Adafruit TPL5110 Low Power Timer Breakout](https://www.adafruit.com/product/3435)

[Ejemplo de uso]()


## Esquema del proyecto

![Esquema proyecto](https://raw.githubusercontent.com/moisesfa/Proyectos_ESP8266/master/ThingerIO_Rec_Salida/Rec_Salida_03/img/adafruit_tpl5110.png)

## Notas del proyecto

1. Para mi fue imprescindible el poner una resistencia Pull Down en la patilla DONE ya que de otro modo no funcionaba. También he necesitado un condensado de unos 100 μF en la salida DRV.

2. También hay que tener en cuenta que la temporización solo se lee cuando se alimenta por primera vez el TPL, y si se cambia después, no se modificará hasta una nueva desconexión y conexión de la alimentación del TPL.

3. Para la lectura de la batería: [Learn de Adafruit](https://learn.adafruit.com/using-ifttt-with-adafruit-io/wiring#battery-tracking)

4. Los tantos por ciento de batería están sin calibrar exactamente y son aproximados.

5. Se puede poner esta instrucción al principio de todo el programa para ver el debug de la librería de Thinger.io. Indica muchos detalles de como se lleva a cabo la conexión. 
```c
#define _DEBUG_
```

## Consumos 

Como se puede ver, los consumos son muy bajos para un sistema que pretende alimentarse con baterías. 

Análisis de consumos:

* El consumo del TPL5110 es unos 35 nA y el intervalo de tiempo de temporización es programable entre 100 ms y 7200 segundos.
* No he conseguido ver ese consumo ya que no tengo equipos adecuados, pero lo que es seguro, es que es menor de 0μA.
* Al transmitir alcanza unos 80 mA pero es tan rápido que no se puede apreciar bien el consumo.

Considero este sistema muy eficiente. 

## Licencia

Este proyecto se presenta bajo [Creative Commons 0](https://creativecommons.org/publicdomain/zero/1.0/) (o dominio público en otras palabras). 

Todo el código está bajo [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html). (o garantizar a los usuarios finales (personas, organizaciones, compañías) la libertad de usar, estudiar, compartir (copiar) y modificar el software. Protegido de intentos de apropiación que restrinjan esas libertades a nuevos usuarios).