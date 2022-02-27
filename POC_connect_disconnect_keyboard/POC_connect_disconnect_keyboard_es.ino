// Poc conexion-desconexion de un teclado USB detras de prototipo BadUSB 
//
// CC Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
// https://creativecommons.org/licenses/by-sa/4.0/
// https://euskalhack.org/ 
//
// =============================================================================================
//            Esquema Basico Breaking Bad USB
//            x Para que funcione mejor tienes que quitar con un cuter la resistencia original de 1k5
//              situada en el punto "x" y substituirla por una de 5K6 si no la deteccion USB a veces falla al haber 2 resistencia pullup
//               +-------------------+         __________   
//               |   x  ,-----+      |        |          |\
//      +--------+      |     |  P5 O|   /----| GND      |.\ 
// Gnd  | ======        +-----+  P4 O+--------| USB+     |:|
//      |   ====     +---[R5K6]--P3 O+--------| USB-     |:|
//      |   ====     |           P2 O+--------| Vcc      |:|   
// Vcc  | ======     |           P1 O|   |    |__________|'|             
//      +--------+ 5v|           P0 O+---+     \__________\|
//               |   O O O           |     
//               +-------------------+      
//
// =============================================================================================
//
// Pasos:
// 1) Inicio teclado interno (libreria DigiKeyboard.h)
// 2) lanzo payload 
// 3) Desconecto teclado interno USB  (libreria DigiKeyboard.h)
// 4) dejo pasar las señales USB a través del cable. (patillas usb en alta impedancia)
// 5) Alimento teclado USB conectando detras del attiny patillas PB0 y PB2 (ver esquema)
//
//
#include "DigiKeyboard.h" //found in https://github.com/ArminJo/DigistumpArduino

void setup() {
  //led de depuracion
  pinMode(PB1     ,OUTPUT); //led interno placa DigiSpark Pin PB1
  digitalWrite(PB1,HIGH);   //flash de depuracion en el led.
  //1)Inicio teclado interno (libreria DigiKeyboard.h)
  usbDeviceConnect();
  DigiKeyboard.delay(900);      //mejora la deteccion(y menos datos en pila)
  DigiKeyboard.sendKeyStroke(0);//Evita perdida primer caracter
  DigiKeyboard.delay(50);       //Retardo para asegurar que se envia el caracter
  
  //2)lanzo payload 
  DigiKeyboard.sendKeyStroke(0,MOD_GUI_LEFT); //tecla windows
  DigiKeyboard.delay(500);
  DigiKeyboard.println(F("POC USB connect-disconnect!"));
  DigiKeyboard.delay(500);

  //3)Desconecto teclado interno USB  (libreria DigiKeyboard.h)
  usbDeviceDisconnect();        //desconecto dispositivo
  DigiKeyboard.delay(50);
  
  //4) dejo pasar las señales USB a través del cable. (patillas usb en alta impedancia)
  pinMode(PB3 , INPUT);   //Pin2 Pb3 USB- como entrada (Alta impedancia)
  pinMode(PB4 , INPUT);   //Pin3 Pb4 USB+ como entrada (Alta impedancia)

  //5) Alimento teclado USB conectando detras del attiny patillas PB0 y PB2 (ver esquema)
  digitalWrite(PB0,LOW   ); //Pongo alimentacion- del teclado conectada al pin PB0 a 0v/GND/LOW
  pinMode(PB0     ,OUTPUT); //Enciendo Alimentacion teclado 
  digitalWrite(PB2,HIGH  ); //Pongo alimentacion+ del teclado conectada al pin PB2 a 5v/Vcc/HIGH
  pinMode(PB2     ,OUTPUT); //Enciendo Alimentacion teclado 
  digitalWrite(PB1,LOW); //Fin flash de depuracion en el led interno Pin PB1
}

void loop() {}
