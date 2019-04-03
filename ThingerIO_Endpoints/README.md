# Uso de puntos finales Endpoints.

La placa de desarrollo utilizada es la Adafruit Feather Huzzah con **ESP8266**.  
Como IDE uso **PlatformIO** que es un ecosistema open source para desarrollos IoT. 

En Thinger.io, un punto final se define como algún tipo de recurso externo al que puede acceder el dispositivo. Con la función de puntos finales, los dispositivos pueden enviar fácilmente correos electrónicos, SMS, datos de inserción a servicios web externos, interactuar con IFTTT y cualquier acción general que se pueda realizar mediante el uso de WebHooks (llamadas URL de HTTP / HTTPS).

Llamar a un punto final es muy fácil desde el código de Arduino, ya que solo se requiere llamar al método call_endpoint sobre la variable cosa.

```c
thing.call_endpoint("endpoint_id");
```

Simplemente puede llamar a un punto final para realizar alguna acción, como enviar un correo electrónico predefinido, o también puede llamar al punto final con algunos datos, lo cual es especialmente útil cuando utiliza servicios de terceros que consumen datos de sus dispositivos.

Debe prestar atención adicional al llamar a los recursos y llamarlos a una velocidad adecuada. De lo contrario, puede consumir fácilmente sus datos disponibles, recibir cientos de correos electrónicos o consumir sus llamadas a la API en servicios de terceros.


## Llamando puntos finales

En este caso, veremos un ejemplo sencillo para enviar una alerta por correo electrónico basada en un valor de temperatura. 

Para este ejemplo, hemos configurado un punto final de correo electrónico llamado high_temp_email que contiene un texto de advertencia sobre la temperatura. 

Para este caso, no queremos verificar la temperatura cada milisegundo, por lo que estamos introduciendo algunas variables para controlar la frecuencia de detección y advertencia. 

En este ejemplo, la temperatura se verifica cada hora y, si está por encima de los 30ºC, llamará al punto final llamado high_temp_email, que nos enviará un correo electrónico con el texto predefinido. Aquí es importante no agregar demoras dentro del método de bucle, ya que evitará la ejecución requerida del método thing.handle(), por lo que estamos utilizando un retardo de no bloqueo basado en la función millis ().


```c
unsigned long lastCheck = 0;
loop(){
    thing.handle(); // required thing handle
    unsigned long currentTs = millis();
    if(currentTs-lastCheck>=60*60*1000){
        lastCheck = currentTs;
        if(dht.readTemperature()>30){
            thing.call_endpoint("high_temp_email");
        }
    }
}
```

## Envío de datos a puntos finales

Enviar datos a un punto final (en formato JSON) también es bastante fácil. Necesitamos llamar también al método call_endpoint, pero en este caso agregamos información basada en el formato de datos pson, que se convertirá automáticamente a JSON. 

Por ejemplo, si queremos informar datos a un servicio de terceros como Keen.io, podemos crear este tipo de puntos finales en la consola. Una vez configurado, podemos llamar al punto final con nuestras lecturas, por ejemplo, con valores de humedad y temperatura de un sensor DHT.


```c
// ¡Tenga cuidado de enviar los datos a una velocidad apropiada!
pson data;
data["temperature"] = dht.readTemperature();
data["humidity"] = dht.readHumidity();
thing.call_endpoint("keen_endpoint", data);
```
También puede enviar datos en función de un recurso definido, es decir, suponga que tiene un recurso que ya satisface la temperatura y la humedad. 

Es posible reutilizar esta definición para enviar estos mismos datos al punto final, sin tener que redefinir la lectura del sensor, como en el siguiente ejemplo.


```c
setup(){
    // defined resource in the setup for reading a sensor value
    thing["data"] >> (pson& out){
        out["temperature"] = dht.readTemperature();
        out["humidity"] = dht.readHumidity();
    }
}

loop(){
    // be careful of sending data at an appropriate rate!
    thing.call_endpoint("endpoint", thing["data"]);
}
```


## Tipo de correo electrónico Ejemplo de punto final


Este es un ejemplo simple, aplicado al punto final de tipo de correo electrónico, con cuerpo personalizado.

```c
setup()
{
 thing["Level"] >> outputValue(actualRelative);
}
loop()
{
 if(actualLevel>UpperLevel && endpointUpperFlag)
   {
    thing.call_endpoint("endpoint_id",thing["Level"]);
    endpointUpperFlag=0;
   }
}
```

Tenga en cuenta que hay una variable que limita la ejecución de este "si" solo una vez, es importante definir cualquier condición o método para garantizar que este tipo de llamada de enpoint se ejecuta solo una vez (o a la velocidad adecuada), porque puede obtener una gran cantidad de correos electrónicos generados por el microcontrolador a través de la plataforma thinger.io.

En la configuración del punto final, en el correo electrónico del cuerpo personalizado, se deben agregar dobles corchetes "{{}}" para invocar la variable enviada por el microcontrolador, en nuestro ejemplo, usamos el siguiente cuerpo

"El nivel real es {{}}%"

Y recibiendo un correo electrónico con el texto:

El nivel actual es 80.34%.


Toda esta información se encuentra disponible en la pagina de Thinger.io. 
Yo tan sólo la he traducido para poder entenderla mejor.

[Fuente de información de los recursos de entrada.](http://docs.thinger.io/arduino/#coding-using-endpoints)

