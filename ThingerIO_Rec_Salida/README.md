# Proyectos con RECURSOS DE SALIDA de Thingerio.

La placa de desarrollo utilizada es la Adafruit Feather Huzzah con **ESP8266**.  
Como IDE uso **PlatformIO** que es un ecosistema open source para desarrollos IoT. 

## Recursos de salida de Thinger.io

Los recursos de salida se deben usar en general cuando necesite detectar o leer un valor del sensor, como temperatura, humedad, etc. Por lo tanto, los recursos de salida son bastante útiles para extraer información del dispositivo.

Para definir un recurso de salida, se usa el operador **>>** que señala el nombre del recurso y utiliza una función Lambda de C++11 para definir la función de salida.

La función de recursos de salida toma un parámetro del tipo **pson** que es un tipo de variable que puede contener booleanos, números, flotantes, cadenas o incluso información estructurada, como en un documento **JSON**.

Las siguientes subsecciones mostrarán cómo definir diferentes recursos de salida para casos de uso típicos.

### Lee un valor del sensor

La definición de un recurso de salida es bastante similar a la definición de un recurso de entrada, pero en este caso se usa el operador **>>**. En la función de devolución de llamada podemos rellenar el valor de salida con cualquier valor que deseemos, como en este caso el resultado de una lectura de sensor.

```c
thing["temperature"] >> [](pson& out){
      out = dht.readTemperature();
};
```

### Leer datos múltiples

De la misma manera que los recursos de entrada pueden recibir múltiples valores al mismo tiempo, los recursos de salida también pueden proporcionar datos múltiples. Este es un ejemplo para proporcionar tanto la latitud como la longitud desde un GPS.

```c
thing["location"] >> [](pson& out){
      out["lat"] = gps.getLatitude();
      out["lon"] = gps.getLongitude();
};
```

### Leer variables del programa

Si su boceto no puede proporcionar una sola lectura del sensor, ya que está realizando algún tipo de integración de datos, también se puede usar un recurso de salida para leer sus variables de boceto, donde el resultado calculado se actualiza con frecuencia.

```c
float yaw = 0; // defined as a global variable
thing["yaw"] >> [](pson& out){
      out = yaw;
};
```

## Recursos de salida más fáciles

Este tipo de recursos permitirá definir un recurso para declarar un recurso de solo lectura, como un valor obtenido de un sensor, o una variable dada en nuestro boceto.

En este ejemplo, estamos definiendo un recurso que expone una lectura del sensor, como la temperatura del sensor DHT11.

```c
thing["temperature"] >> outputValue(dht.readTemperature());
```

Pero también es posible definir un recurso de salida para cualquier variable global en nuestro boceto.

```c
thing["temperature"] >> outputValue(dht.readTemperature());
```

## Recursos de entrada / salida

El último tipo de recurso es un recurso que no solo toma una entrada o una salida, sino que toma ambos parámetros. Esto es bastante útil cuando quiere leer una salida que depende de una entrada, es decir, cuando necesita proporcionar un cambio de referencia a un sensor.

Este tipo de recursos se definen con operator **=**. En este caso, la función toma dos parámetros **pson** diferentes. Uno para datos de entrada y otro para datos de salida. Este ejemplo proporciona una lectura de altitud utilizando el sensor BMP180. Toma la altitud de referencia como entrada y proporciona la altitud actual como salida.

```c
thing["altitude"] = [](pson& in, pson& out){
    out = bmp.readAltitude(in);
};
```

También puede definir recursos de entrada/salida más complejos, que toman varios valores de entrada, para proporcionar también múltiples valores de salida, como en este ejemplo que toma value1 y value2 para proporcionar la suma y los valores mult.

```c
thing["in_out"] = [](pson& in, pson& out){
    out["sum"] = (long)in["value1"] + (long)in["value2"];
    out["mult"] = (long)in["value1"] * (long)in["value2"];
};
```

## Recursos sin parámetros

También es posible definir recursos que no requieren ninguna entrada ni generan ningún resultado. Son como devoluciones de llamada que se pueden ejecutar como desee, por ejemplo para reiniciar el dispositivo o realizar alguna acción requerida.

En este caso, el recurso se define como una función sin ningún parámetro de entrada o salida.

```c
thing["resource"] = [](){
    // write here your execution code
};
```


Toda esta información se encuentra disponible en la pagina de Thinger.io.
Yo tan sólo la he traducido para poder entenderla mejor.

[Fuente de información de los recursos de salida.](http://docs.thinger.io/arduino/#coding-adding-resources-output-resources)

