#include <Arduino.h>

#include <ESP8266WiFi.h>       // Librería de conexión WiFi del módulo ESP8266
#include <ThingerESP8266.h>    // Librería de la plataforma thinger.io

// Parámetros de la conexión con thinger.io
#define usuario "************"
#define device_Id "************"
#define device_credentials "************"

ThingerESP8266 thing(usuario, device_Id, device_credentials);

// Parámetros de conexión WiFi
const char WiFi_ssid[]="************";          //Nombre de red
const char WiFi_password[]="************";  	//Clave de red

// Definir las variales de color 
int Var_rojo;
int Var_verde;
int Var_azul;
String Var_color_hex;

#define PIN_ROJO 12
#define PIN_VERDE 13
#define PIN_AZUL 14

// Función para establecer un color
void setColor(String hexcolor) {
    // Get rid of '#' and convert it to integer
    int number = (int) strtol( &hexcolor[1], NULL, 16);
    // Split them up into r, g, b values
    int r = (number >> 16) *4;
    int g = (number >> 8 & 0xFF) *4;
    int b = (number & 0xFF) * 4;
    analogWrite(PIN_ROJO, r);
    analogWrite(PIN_VERDE, g);
    analogWrite(PIN_AZUL, b);
}

// Función para testetear el funcionamiento
void inicio() {
    setColor("#FF0000");  // red
    delay(1000);    
    setColor("#00FF00");  // green
    delay(1000);
    setColor("#0000FF");  // blue
    delay(1000);
    setColor("#FFFF00");  // yellow
    delay(1000);  
    setColor("#500050");  // purple
    delay(1000);
    setColor("#00FFFF");  // aqua
    delay(1000);    
}

void setup() {
    // put your setup code here, to run once:
    // Open serial communications and wait for port to open:
    Serial.begin(9600); 
    
    // Salidas
    pinMode(PIN_ROJO, OUTPUT);
    pinMode(PIN_VERDE, OUTPUT);
    pinMode(PIN_AZUL, OUTPUT);

    // Estados iniciales 
    digitalWrite(PIN_ROJO, LOW);
    digitalWrite(PIN_VERDE, LOW);
    digitalWrite(PIN_AZUL, LOW);

    Var_rojo = 0;
    Var_verde = 0;
    Var_azul = 0;

    // Inicialización de la WiFi para comunicarse con la API
    thing.add_wifi(WiFi_ssid, WiFi_password);

    // ----------------------------------------
    // Creamos los recursos DE ENTRADA para la API
    // ----------------------------------------
    
    /* Easier Resources (declarar recursos con mayor facilidad)
    La ventaja de utilizar este tipo de definiciones es que sus recursos podrán gestionar 
    el estado cuando los consulte desde la API. Por ejemplo, si tiene un pin digital habilitado 
    o deshabilitado, podrá ver su estado actual tanto en el explorador de API como en el Dashboard.
    */

    // Ejemplo para controlar un LED RGB mediante 3 Sliders en el DASHBOARD
    // Podremos controlar el color con unos deslizadores pero lo más importante aquí es que podemos 
    // modificar el valor de una variable desde Internet

    thing["Color_Rojo"] << inputValue(Var_rojo, {
    // se ejecuta cuando el valor cambia execute some code when the value change
    Serial.print("El color Rojo a cambiado desde thingerio a: ");
    Serial.println(Var_rojo);
    analogWrite(PIN_ROJO,Var_rojo);
    });

    thing["Color_Verde"] << inputValue(Var_verde, {
    // se ejecuta cuando el valor cambia execute some code when the value change
    Serial.print("El color Verde a cambiado desde thingerio a: ");
    Serial.println(Var_verde);
    analogWrite(PIN_VERDE,Var_verde);
    });
    
    thing["Color_Azul"] << inputValue(Var_azul, {
    // se ejecuta cuando el valor cambia execute some code when the value change
    Serial.print("El color Azul a cambiado desde thingerio a: ");
    Serial.println(Var_azul);
    analogWrite(PIN_AZUL,Var_azul);
    });

    thing["Color_Hexadecimal"] << inputValue(Var_color_hex, {
    // se ejecuta cuando el valor cambia execute some code when the value change
    Serial.print("El color a cambiado desde thingerio a: ");
    Serial.println(Var_color_hex);
    setColor(Var_color_hex);  // color en Hexadeimal 
    });


    inicio();
}    

void loop() {
    // put your main code here, to run repeatedly:
    thing.handle();
}