#include <Arduino.h>
#include <ESP8266WiFi.h>       //Librería de conexión WiFi del módulo ESP8266
#include <ThingerESP8266.h>    //Librería de la plataforma thinger.io

// Parámetros de la conexión con thinger.io
#define usuario "************"
#define device_Id "************"
#define device_credentials "************"

ThingerESP8266 thing(usuario, device_Id, device_credentials);

// Parámetros de conexión WiFi
const char WiFi_ssid[]="************";          //Nombre de red
const char WiFi_password[]="************";  	//Clave de red

#define PIN_LED 0
#define PIN_RELE 16

void setup() {
    // put your setup code here, to run once:
    
    // Salida
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_RELE, OUTPUT);
    
    // Inicialización de la WiFi para comunicarse con la API
    thing.add_wifi(WiFi_ssid, WiFi_password);

    // ----------------------------------------
    // Creamos los recursos para la API
    // ----------------------------------------
    
    /*
    thing["led_onoff"] << [](pson& in){
      digitalWrite(PIN_LED, in ? LOW : HIGH);
    };
    */
   
   // Modificación para que funcione bien el tablero
   thing["led_onoff"] << [](pson& in){
    if(in.is_empty()){
        in = !(bool) digitalRead(PIN_LED);
    }
    else{
        digitalWrite(PIN_LED, in ?  LOW : HIGH);
    }   
    };
    
    // pin control example (i.e. turning on/off a light, a relay, etc)
    thing["rele"] << digitalPin(PIN_RELE);

}    

void loop() {
    // put your main code here, to run repeatedly:
    thing.handle();
}