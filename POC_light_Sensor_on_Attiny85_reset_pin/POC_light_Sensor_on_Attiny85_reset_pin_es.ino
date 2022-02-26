// POC light Sensor on Attiny85 reset pin 
// POC Sensor de luz aprovechando la patilla de reset del Attiny85
//
// CC Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
// https://creativecommons.org/licenses/by-sa/4.0/
// https://euskalhack.org/ 
//
//
#include <DigiUSB.h>
// NOTAS: DigiUSB.h Cambio #define RING_BUFFER_SIZE 128 por #define RING_BUFFER_SIZE 64
// Asi ahorro RAM. El chip va muy justo de RAM. (no deberia pasar de 50%)
// por experiencia cuando hay poca RAM libre (<256bytes)
// Se desborda la pila y se puede perder el bootloader. (vamos que te lo cargas)
// =============================================================================================
//
//                 +------------------+
//                 |     ,-----+      |
//        +--------+     |     |  P5 O+---------[]                  PB5 (D5) RESET/ADC0
// Vcc(+) | ======       +-----+  P4 O|         []!  LDR            PB4 (D4) USB+ ADC2
//        |   ====             "  P3 O|      ---[]                  PB3 (D3) USB- ADC3 
//        |   ====      ______    P2 O|      |                      PB2 (D2) SCK/SCL/INT0/ADC1
// Gnd(-) | ======                P1 O|     [0]                     PB1 (D1) LED" MISO/DO/OC0B/OC1A/AIN1/PCINT1
//        +--------+ 5 GND Vin    P0 O|     [1] 100K(Resistencia)   MOSI/DI/SDA/OC0A/AIN0
//                 |  O O O "         |     [M] 
//                 +----+-------------+      |
//                      |____________________|
//
// =============================================================================================
void setup(){
  DigiUSB.begin();
  pinMode(PB1, OUTPUT);
  digitalWrite(PB1, HIGH); //enciendo led depuracion
  pinMode(PB5, INPUT);     //PB5/RESET como entrada Para sensor de luz
  analogRead(0);           //descarto primera lectura //Ojo PB5/RESET es ADC0 no ADC5);
  digitalWrite(PB0, LOW);  //compatibilitad prototipo sniff
  pinMode(PB0, OUTPUT);    //compatibilitad prototipo sniff
  DigiUSB.delay(1000);     //mejora la deteccion del dispositivo USB (DigiUSB.begin();)
}
void loop(){
  DigiUSB.print(F("Sensor_luz:"));    //con la macro F( para usar memoria programa ahorro 30bytes
  DigiUSB.delay(100);                 //retardo para asegurar la transmision
   
  // Sensor luz con una LDR y una resistencia en serie de 100k entre patilla PB5/RESET/ADC0 y GND
  int ldr=analogRead(0); //Ojo PB5/RESET es ADC0 no ADC5);  

  //POC enciedo el led interno si no hay luz (a lo fotocelula)
  if(ldr>730){ //900 valor por observacion voy tapando la LDR y observo
    digitalWrite(PB1, HIGH);    
  }else{
    digitalWrite(PB1, LOW);
  }
  // Depuracion Observo el valor en tiempo real.
  DigiUSB.print(F(" PB5="));
  DigiUSB.println(int(ldr));
  DigiUSB.delay(250);   //retardo para asegurar la transmision
}
