#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else
 #include <WiFi.h>
#endif
#include "ModbusIP_ESP8266.h"

const int REG0 = 0;               // Modbus dirección o Offset del holding register
const int REG1 = 1;               // Modbus dirección o Offset del holding register
const int REG2 = 2;               // Modbus dirección o Offset del holding register
const int REG3 = 3;               // Modbus dirección o Offset del holding register

IPAddress remote(192, 168, 1, 10);  // Esta es la dirección de un dispositivo servidor en este caso el PLCS7-1500
const int LOOP_COUNT = 2;

ModbusIP mb;  //Objeto MODBUS

void setup() {

  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);
  
  Serial.begin(115200);
 
  WiFi.begin("PLC_AP"); //Este es el nombre del SSID, se puede añadir el password en el segundo argumento
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mb.client(); //Inicio en modo cliente de ESP32
}

uint16_t res0 = 0;
uint16_t res1 = 0;
uint16_t res2 = 0;
uint16_t res3 = 0;

uint8_t show = LOOP_COUNT;

void loop() {
  if (mb.isConnected(remote)) {   //Chequear si hay conexión con el servidor modbus

    mb.readHreg(remote, REG0, &res0);  //Leer un holding register en la dirección asignada en la IP remota.
    mb.readHreg(remote, REG1, &res1);  //Leer un holding register en la dirección asignada en la IP remota.
    mb.readHreg(remote, REG2, &res2);  //Leer un holding register en la dirección asignada en la IP remota.
    mb.readHreg(remote, REG3, &res3);  //Leer un holding register en la dirección asignada en la IP remota.
    
  } else {
    mb.connect(remote);           //REconectar si hay problema
  }
  mb.task();                      // Ejecutar funciones de cliente modbus
  delay(100);                     // Pulling interval
  if (!show--) {                   // Se muestran los resultados de esta manera cada segundo, también se puede usar milis
    
    /*Serial.println(res0);
    Serial.println(res1);
    Serial.println(res2);
    Serial.println(res3);
    Serial.println(" "); */
    digitalWrite(4,res0);
    digitalWrite(2,res1);
    dacWrite(26,res2);
    dacWrite(25,res3);

    
    show = LOOP_COUNT;
  }
}
