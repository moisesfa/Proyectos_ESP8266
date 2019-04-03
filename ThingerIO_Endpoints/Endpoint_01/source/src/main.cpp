//#define _DEBUG_
#include <Arduino.h>

#include <ESP8266WiFi.h>       // Librería de conexión WiFi del módulo ESP8266
#include <ThingerESP8266.h>    // Librería de la plataforma thinger.io

#include "Seeed_BME280.h"      // Librería para el sensor BME280  
#include <Wire.h>              // Para usar el I2C 

// Parámetros de la conexión con thinger.io
#define usuario "***********"
#define device_Id "***********"
#define device_credentials "***********"

ThingerESP8266 thing(usuario, device_Id, device_credentials);

// Parámetros de conexión WiFi
const char WiFi_ssid[]="***********";            // Nombre de red WiFi 
const char WiFi_password[]="***********";  // Password de red WiFi

// Led indicador de funcionamiento
#define PIN_LED 0

// Declaración de variables 
BME280 bme280;
float presion;
float presionhPa;
float temperatura;
int humedad;

// Variables de tiempo 
unsigned long currentMillis;
unsigned long previousMillis_medir = 0;
unsigned long previousMillis_aviso = 0;
unsigned long interval_medir = 0; 
unsigned long interval_aviso = 0; 

// Para indicar si ya se realizo un aviso y no volver a hacerlo
bool Est_Aviso = true;
byte Temp_Limite = 25; 

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
  
  if (temperatura < Temp_Limite){
    Est_Aviso = true;
    Serial.println("Avisos ON");
  }    

  Serial.println(" ");

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
    Serial.println("Puesta en marcha");
    Serial.println(" ");
    
    // ----------------------------------------
    // Creamos los recursos DE SALIDA para la API
    // ----------------------------------------
    thing["bme280"] >> [](pson & out) {
      out["bme280_temperatura"] = temperatura;
      out["bme280_humedad"] = humedad;
      out["bme280_presion"] = presion/100;   // En hectopascales         
    };    

    // ----------------------------------------
    // Creamos los recursos DE ENTRADA para la API
    // ----------------------------------------
    thing["Temp_Lim"] << inputValue(Temp_Limite, {
    // se ejecuta cuando el valor cambia execute some code when the value change
    Serial.print("El valor de Temp_Limite a cambiado desde thingerio a: ");
    Serial.println(Temp_Limite);    
    });
    
    // Temporizaciones para medir y para enviar avisos 
    interval_medir= 10*1000; // 10 segundos
    interval_aviso= 60*1000; // 60 segundos

    // Primer estado 
    previousMillis_medir = millis();
    previousMillis_aviso = millis();

}

void loop() {
  
  thing.handle();

  // Leer los datos del sensor cada Var_inter_medir, en este caso cada 10 segundos
  currentMillis = millis();
  
  // Temporizacion para las medidas de datos 
  if(currentMillis - previousMillis_medir > interval_medir) {
    previousMillis_medir = currentMillis;
    //interval_medir = Var_inter_medir*1000;
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));
    Leer_bme280();    
    thing.stream(thing["bme280"]);
  }    
  
  
  // Temporizacion para el aviso
  if(currentMillis - previousMillis_aviso > interval_aviso) {
    previousMillis_aviso = currentMillis;
    
    // Si la temperatura es mayor de Temp_Limite y no se ha enviado aviso, envio email    
    // Dejo de enviar Emails hasta que la temperatura baje de Temp_Limite
    if (temperatura>Temp_Limite && Est_Aviso == true){
      Serial.println("Envío Email!");
      pson data;
      data["temperature"] = temperatura;
      thing.call_endpoint("high_temp_email", data);
      
      Est_Aviso = false;       
      Serial.println("Avisos OFF");
      Serial.println(" ");
    }     
  }    
}