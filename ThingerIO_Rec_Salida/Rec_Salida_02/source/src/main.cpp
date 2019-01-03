//#define _DEBUG_
// the rest of your sketch goes here
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

// Modo dormido
// http://extraconversion.com/es/tiempo/microsegundos/microsegundos-a-minutos.html
//unsigned long TIEMPO_DeepSleep = 36e8;          // Tiempo en modo deep-sleep en microsegundos 3600s
unsigned long TIEMPO_DeepSleep = 90e6;          // Tiempo en modo deep-sleep en microsegundos 90s


// Declaración de variables 
BME280 bme280;
float presion;
float presionhPa;
float temperatura;
float altitud;
int humedad;
int ad_bat;
int level_bat;

void battery_level() {
     
  // Leer la bateria segun un ejemplo de Adafruit 
  // read the battery level from the ESP8266 analog in pin.
  // analog read level is 10 bit 0-1023 (0V-1V).
  // our 1M & 220K voltage divider takes the max
  // lipo value of 4.2V and drops it to 0.758V max.
  // this means our min analog read value should be 580 (3.14V)
  // and the max analog read value should be 774 (4.2V).
  ad_bat = analogRead(A0);
  Serial.print("Battery AD: "); Serial.print(ad_bat);Serial.println(" ");
  // convert battery level to percent
  level_bat = map(ad_bat, 460, 654, 0, 100);
  Serial.print("Battery level: "); Serial.print(level_bat); Serial.println("%");

  }

// Función para leer los datos del sensor y la bateria 
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

  battery_level();
}


void setup() {  
    // Abrimos la comunicación serie y espero a que el puerto se abra:
    //delay(1000); //Calentamiento
    Serial.begin(115200); 
           
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
      out["level_bat"] = level_bat; 
      out["ad_bat"] = ad_bat; 
    };

}

void loop() {
    
  thing.handle();       // Se encarga de la conexión y siempre debe estar al principio
  Leer_bme280();
  thing.write_bucket("Bucket_01", "bme280");
  
  // Delay necesario para que escriba los datos en el data Buckets
  delay(1000);

  Serial.println("Entrar en sueño profundo zzzzzzzzzzzz");
  digitalWrite(PIN_LED, HIGH);
  ESP.deepSleep(TIEMPO_DeepSleep,WAKE_RF_DEFAULT);    // Desabilita la señal de radio después del reencendido
    
}