# Proyectos con RECURSOS DE ENTRADA de Thingerio.

La placa de desarrollo utilizada es la Adafruit Feather Huzzah con **ESP8266**.  
Como IDE uso **PlatformIO** que es un ecosistema open source para desarrollos IoT. 

## Recursos de entrada de Thinger.io

Si necesita controlar o activar su dispositivo de IoT, es necesario definir un recurso de entrada. De esta forma, un recurso de entrada es cualquier cosa que pueda proporcionar información a su dispositivo. Por ejemplo, puede ser un recurso para encender y apagar una luz o un relé, cambiar una posición de servo, ajustar un parámetro de dispositivo, etc.

Para definir un recurso de entrada, se utiliza el operador **<<** que apunta al nombre del recurso, y utiliza una función Lambda C++11 para definir la función.

La función de recursos de entrada toma un parámetro de tipo **pson** que es un tipo de variable que puede contener booleanos, números, flotantes, cadenas o incluso información estructurada como en un documento **JSON**.

Las siguientes subsecciones mostrarán cómo definir diferentes recursos de entrada para casos de uso típicos.

### Enciende / apaga un led, un relé, etc.

Este tipo de recursos solo requiere un estado de encendido / apagado para que pueda habilitarse o deshabilitarse según sea necesario. Como el tipo **pson** contiene múltiples tipos de datos, podemos pensar que el parámetro **pson** de la función de entrada es como un booleano.

Entonces, dentro de la función de **setup** puede colocar un recurso llamado **led** (pero puede usar cualquier otro nombre), de tipo de entrada (usando el operador **<<**), que toma una referencia a un parámetro **pson**. Este ejemplo activará / desactivará el pin digital 10 utilizando un operador ternario sobre el parámetro **in**.


```c
thing["led"] << [](pson& in){
	digitalWrite(10, in ? HIGH : LOW);
};
```

### Modificar una posición de servo

La modificación de una posición de servo es bastante similar a encender / apagar un led. En este caso, sin embargo, es necesario usar un valor entero. Como el tipo **pson** puede contener múltiples tipos de datos, podemos usar el tipo pson como un valor entero.

```c
thing["servo"] << [](pson& in){
    myServo.write(in);
};
```
### Actualizar variables del programa

Puede usar los recursos de entrada también para actualizar las variables del programa, para que pueda cambiar el comportamiento de su dispositivo de forma dinámica. Esto es bastante útil en algunas situaciones en las que desea deshabilitar temporalmente una alarma, cambiar los intervalos de informe, actualizar un valor de histéresis, y así sucesivamente. De esta manera, puede definir recursos adicionales para cambiar sus variables.

```c
float hysteresis = 0; 	// defined as a global variable
thing["hysteresis"] << [](pson& in){
    hysteresis = in;
};
```

### Pasar datos múltiples

El tipo de datos **pson** puede contener no solo diferentes tipos de datos, sino que también es totalmente compatible con documentos **JSON**. Entonces puede usar el tipo de datos pson para recibir múltiples valores al mismo tiempo. Este ejemplo recibirá dos floats diferentes que se almacenan con las llaves lat y lon.

```c
thing["location"] << [](pson& in){
    float lat = in["lat"];
    float lon = in["lon"];
};
```

### Mostrar estado de recursos de entrada en Dashboards y API

Los Dashboards o API funcionan de forma tal que cuando los abre, consultan los recursos asociados para imprimir correctamente su estado actual, es decir, el interruptor está activado o desactivado. De esta forma, cuando la API o un Dashboard están abiertos, se llama a cada recurso de entrada asociado, recibiendo datos vacíos en la llamada, ya que no hay intención de controlar el recurso (la entrada de pson estará vacía).

Entonces, ¿cómo saben los Dashboards o la API cuál es el estado actual de un recurso de entrada? El recurso debe establecer su estado actual en el parámetro de entrada, si está vacío, o usar el valor de entrada si hay uno. De esta forma, podemos obtener tres cosas diferentes: consultar el estado del recurso actual (sin modificarlo), modificar el estado actual del recurso y obtener la entrada esperada en el recurso (así es como funciona el explorador API en el dispositivo).

Por lo tanto, una definición de recurso de entrada correcta que realmente permita mostrar el estado actual del recurso en un Dashboard o en la API será como este código de ejemplo.

```c
thing["resource"] << [](pson& in){
    if(in.is_empty()){
        in = currentState;
    }
    else{
        currentState = in;
    }
}
```

Este código de ejemplo básicamente devuelve el estado actual (como un booleano, un número, etc.) si no hay control de entrada, o utiliza los datos entrantes para actualizar el estado actual. Esto se puede adaptar fácilmente para controlar un led, al tiempo que muestra su estado actual en el dashboard una vez abierto o actualizado.

```c
thing["led"] << [](pson& in){
    if(in.is_empty()){
        in = (bool) digitalRead(pin);
    }
    else{
        digitalWrite(pin, in ? HIGH : LOW);
    }
}
```

Nota: para controlar un pin digital simplemente use el método explicado en la siguiente sección.

## Recursos de entrada más fáciles

La biblioteca del cliente también incluye algunas definiciones sintácticas útiles para declarar recursos con mayor facilidad sin tener que pensar en recursos de entrada o salida. Estas características sintácticas son macros que se expanden automáticamente para definir los recursos de la manera estándar.

La ventaja de utilizar este tipo de definiciones es que sus recursos podrán gestionar el estado cuando los consulte desde la API. Por ejemplo, si tiene un pin digital habilitado o deshabilitado, podrá ver su estado actual tanto en el explorador de API como en el Dashboard.

### Controla un pin digital

Este tipo de recursos permitirá definir un recurso para declarar un control sobre un pin digital, para que pueda alternar entre estados on/off, que se pueden usar para controlar un led, un relé, una luz, etc.

Se requiere definir el pin digital como OUTPUT en su código de configuración, o el recurso no funcionará correctamente.

```c
thing["relay"] << digitalPin(PIN_NUMBER);
thing["relay"] << invertedDigitalPin(PIN_NUMBER);
```


Toda esta información se encuentra disponible en la pagina de Thinger.io. 
Yo tan sólo la he traducido para poder entenderla mejor.

[Fuente de información de los recursos de entrada.](http://docs.thinger.io/arduino/#coding-adding-resources-input-resources)

