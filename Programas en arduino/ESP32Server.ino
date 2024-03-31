#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include "ModbusIP_ESP8266.h"

//Offsets o direcciones de registros (Holding registers) para modbus
const int HREG0 = 0;
const int HREG1 = 1;
const int HREG2 = 2;
const int HREG3 = 3;

//ModbusIP object
ModbusIP mb;

long ts;

void setup() {

    pinMode(18,INPUT);
    pinMode(19,INPUT);
    
    Serial.begin(115200);
    //Conectar con SSID de red local
    WiFi.begin("PLC_AP");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP()); //Mostrar IP asignada

    mb.server();    //ESP32 en modo servidor

    mb.addHreg(HREG0);//Crear Holding registers en la dirección seleccionada
    mb.addHreg(HREG1);
    mb.addHreg(HREG2);
    mb.addHreg(HREG3);

    ts = millis();
}


void loop() {
   //Llamar a librerias y funciones para protocolo MODBUS
   mb.task();

   //Leer temperatura e incrementar variable cada 0.5 segundos
   if (millis() > ts + 500) {
       ts = millis();

       int DIG0 = digitalRead(18); //Leer Entada Digital 0
       int DIG1 = digitalRead(19); //Leer Entada Digital 1
       int ANG0 = analogRead(35); //Leer Entada Analogica 0
       int ANG1 = analogRead(34); //Leer Entada Analogica 1

       mb.Hreg(HREG0, DIG0); //Escribir valor de Entrada Digital en los holding registers
       mb.Hreg(HREG1, DIG1);
       mb.Hreg(HREG2, ANG0); //Escribir valor de Entrada Analogica en los holding registers
       mb.Hreg(HREG3, ANG1);

       Serial.println(DIG0);
       Serial.println(DIG1);
       Serial.println(ANG0);
       Serial.println(ANG1);
       Serial.println(" ");
      
   }  
   delay(10);
}