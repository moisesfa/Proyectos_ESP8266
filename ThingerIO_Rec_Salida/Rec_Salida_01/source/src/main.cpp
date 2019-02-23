#include <Arduino.h>

#include <ESP8266WiFi.h>       // Librería de conexión WiFi del módulo ESP8266
#include <ThingerESP8266.h>    // Librería de la plataforma thinger.io

#include "Seeed_BME280.h"      // Librería para el sensor BME280  
#include <Wire.h>              // Para usar el I2C 

// Parámetros de la conexión con thinger.io
#define usuario "*******************************"
#define device_Id "*******************************"
#define device_credentials "*******************************"

ThingerESP8266 thing(usuario, device_Id, device_credentials);

// Parámetros de conexión WiFi
const char WiFi_ssid[]="*******************************";            // Nombre de red WiFi 
const char WiFi_password[]="*******************************";  // Password de red WiFi

// Led indicador de funcionamiento
#define PIN_LED 0

// Declaración de variables 
BME280 bme280;
float presion;
float presionhPa;
float temperatura;
float altitud;
int humedad;

// Variable de tiempo 
unsigned long currentMillis;
unsigned long previousMillis = 0;
unsigned long interval = 0; 
int Var_inter_medir;

// Función para leer los datos del sensor
void Leer_bme280(){
  
  // Obtener y mostrar la temperatura 
  Serial.print("Temp: ");
  Serial.print(temperatura = bme280.getTemperature());
  Serial.println(" ºC");
  
  // Obtener y mostrar la humedad   
  Serial.print("Humidity: ");
  Serial.print(humedad = bme280.getHumidity());  
  Serial.println(" %");

  // Obtener y mostrar la presión 
  Serial.print("Pressure: ");
  Serial.print(presion = bme280.getPressure());  
  Serial.println(" Pa");

  // Obtener y mostar la altitud
  Serial.print("Altitude: ");
  Serial.print(altitud = bme280.calcAltitude(presion));
  Serial.println(" m");
}

void setup() {  
    // Abrimos la comunicación serie y espero a que el puerto se abra:
    delay(1000);
    Serial.begin(9600); 
           
    if(!bme280.init()){
      Serial.println("Error en el dispositivo!");
    }
    
    // Salida
    pinMode(PIN_LED, OUTPUT);
    // Estado iniciale
    digitalWrite(PIN_LED, LOW);
    
    // Inicialización de la WiFi para comunicarse con la API
    thing.add_wifi(WiFi_ssid, WiFi_password);
    Serial.println(" ");
    Serial.println("Modo normal de funcionamiento");

    //Leer_bme280();
    // ----------------------------------------
    // Creamos los recursos DE SALIDA para la API
    // ----------------------------------------
    thing["bme280"] >> [](pson & out) {
      out["bme280_temperatura"] = temperatura;
      out["bme280_humedad"] = humedad;
      out["bme280_presion"] = presion/100;   // En hectopascales   
      out["bme280_altitud"] = altitud; 
    };

    Var_inter_medir= 10;

}

void loop() {
  
  thing.handle();

  //Leer los datos del sensor cada Var_inter_medir, en este caso cada 10 segundos
  currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    interval=Var_inter_medir*1000;
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));
    Leer_bme280();
    thing.stream(thing["bme280"]);
  }
     
  
}