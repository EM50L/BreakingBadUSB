// POC light Sensor on Attiny85 using a led in reverse mode 
// POC Sensor de luz usando un led en polarizacion inversa
//
// CC Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
// https://creativecommons.org/licenses/by-sa/4.0/
// https://euskalhack.org/ 
//
//
#include <DigiUSB.h> //debug info
// NOTAS: DigiUSB.h Cambio #define RING_BUFFER_SIZE 128 por #define RING_BUFFER_SIZE 64
// asi ahorro RAM. El chip va muy justo de RAM. (no deberia pasar de 50%) Cuando hay poca RAM libre 
// Se desborda la pila y se puede perder el bootloader. (vamos que te lo cargas)
//
//                 +------------------+
//                 |     ,-----+      |
//        +--------+     |     |  P5 O|                                   PB5 (D5) RESET/ADC0
// Gnd(-) | ======       +-----+  P4 O|                                   PB4 (D4) USB+ ADC2
//        |   ====             "  P3 O|                                   PB3 (D3) USB- ADC3     
//        |   ====      ______    P2 O+----------+                        PB2 (D2) SCK/SCL/INT0/ADC1
// Vcc(+) | ======                P1 O|        __|__                      PB1 (D1) LED" MISO/DO/AIN1/PCINT1
//        +--------+ 5 GND Vin    P0 O|        \   / =>"  LED             PB0 (D0) MOSI/DI/SDA/OC0A/AIN0
//                 |  O O O "         |         \ /  =>   En inversa
//                 +--+---------------+       -------
//                    |__________________________|
void setup(){
  DigiUSB.begin();
  pinMode(PB1, OUTPUT);
  digitalWrite(PB1, HIGH);
  DigiUSB.delay(1000);   //mejora la deteccion del dispositivo USB
}
void loop(){
  DigiUSB.print("Sensor_luz:  ");
  DigiUSB.delay(100);   //retardo para asegurar la transmision

  // Sensor luz con Diodo LED en inversa entre PB2 y Vcc
  pinMode(PB2, OUTPUT);          //Cargo el LED luego PB2=0
  digitalWrite(PB2, LOW);        //Explicacion el led en inversa se comporta como un condensador
  DigiUSB.delay(100);            //luego almacena un poco de carga electrica
  
  pinMode(PB2, INPUT);           //Patilla PB2 en alta impedancia (muy poco consumo)
  
  int i = 0;                     //Explicacion:
  for(i=0; i<254 ; i++){         //el Tiempo que tarda el LED 
      if(digitalRead(PB2))break; //en descargarse depende
      DigiUSB.delay(1);          //de la cantidad de LUZ 
  }                              //que incida en la superficie del mismo

  //POC enciedo el led interno si no hay luz (a lo fotocelula)
  if(i>253){
      digitalWrite(PB1, HIGH);    
  }else{
      digitalWrite(PB1, LOW);
  }
  // Depuracion Observo el valor en tiempo real.
  DigiUSB.print(" t=");
  DigiUSB.println(int(i));
  DigiUSB.delay(250);//retardo para asegurar la transmision del dato
}
