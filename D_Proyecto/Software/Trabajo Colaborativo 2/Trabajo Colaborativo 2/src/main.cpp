#include <Arduino.h>
#include <WiFi.h>


// defino los pines como entradas Analógicas

#define adc_1 3
#define adc_2 4
#define adc_3 5
#define adc_4 6

// defino los pines como entradas Digitales
#define dcl_1 15
#define dcl_2 16
#define dcl_3 17
#define dcl_4 18

// defino Led de verificación de entradas Digitales
#define Led 2


// creo una función para el estado del led
void encender_led(){
  digitalWrite(Led,!digitalRead(Led));
  Serial.println("Pin digital presionado");
}
void setup() {

  // Iniciamos la comunicación serial a 115200 b/s
  
  Serial.begin(115200);

  // Mensaje por puerto serial

  Serial.println("Proceso exitoso !!!");

  // Configuramos Led como salida

  pinMode(Led,OUTPUT);

  // Configuramos las entradas digitales como interrupciones

  pinMode(dcl_1,INPUT_PULLDOWN);
  pinMode(dcl_2,INPUT_PULLDOWN);
  pinMode(dcl_3,INPUT_PULLDOWN);
  pinMode(dcl_4,INPUT_PULLDOWN);

  //llamo a la función led con cada interrupción

  attachInterrupt(dcl_1,encender_led,FALLING);
  attachInterrupt(dcl_2,encender_led,FALLING);
  attachInterrupt(dcl_3,encender_led,FALLING);
  attachInterrupt(dcl_4,encender_led,FALLING);

}

void loop(){

  Serial.print("ADC_1 Value: ");
  Serial.println(analogRead(adc_1));

  Serial.print("ADC_2 Value: ");
  Serial.println(analogRead(adc_2));

  Serial.print("ADC_3 Value: ");
  Serial.println(analogRead(adc_3));

  Serial.print("ADC_4 Value: ");
  Serial.println(analogRead(adc_4));

  delay(1000);
}