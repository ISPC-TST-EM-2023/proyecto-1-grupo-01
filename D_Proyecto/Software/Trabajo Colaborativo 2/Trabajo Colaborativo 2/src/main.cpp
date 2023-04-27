#include <Arduino.h>
#include <WiFi.h>


// defino los pines como entradas Analógicas

#define adc_1 2
#define adc_2 4
#define adc_3 13
#define adc_4 15

// defino los pines como entradas Digitales
#define btn1 34
#define btn2 35
#define btn3 32
#define btn4 33

// defino Led de verificación de entradas Digitales
#define Led_1 25
#define Led_2 26
#define Led_3 27
#define Led_4 14


#define DELAY_BTN 600   // 500ms

int i = 0;
int btnPress = 0;
long lastTime = 0;

// creo una función para el estado del led
void ISR_1(){
  if (!digitalRead(btn1)){
    digitalWrite(Led_1,!digitalRead(Led_1));
    btnPress = 1;
  } 
  else btnPress = 0;
}

void ISR_2(){
  if (!digitalRead(btn2)){
    digitalWrite(Led_2,!digitalRead(Led_2));
    btnPress = 2;
  } 
  else btnPress = 0;
}

void ISR_3(){
  if (!digitalRead(btn3)){
    digitalWrite(Led_3,!digitalRead(Led_3));
    btnPress = 3;
  } 
  else btnPress = 0;
}

void ISR_4(){
  if (!digitalRead(btn4)){
    digitalWrite(Led_4,!digitalRead(Led_4));
    btnPress = 4;
  } 
  else btnPress = 0;
}

void setup() {

  // Iniciamos la comunicación serial a 115200 b/s

  Serial.begin(115200);

  // Configuramos Led como salida

  pinMode(Led_1,OUTPUT);
  pinMode(Led_2,OUTPUT);
  pinMode(Led_3,OUTPUT);
  pinMode(Led_4,OUTPUT);

  // Configuramos las entradas digitales como interrupciones

  pinMode(btn1, INPUT_PULLDOWN);       // INPUT_PULLDOWN/INPUT_PULLUP
  pinMode(btn2, INPUT_PULLDOWN);       // INPUT_PULLDOWN/INPUT_PULLUP
  pinMode(btn3, INPUT_PULLDOWN);       // INPUT_PULLDOWN/INPUT_PULLUP
  pinMode(btn4, INPUT_PULLDOWN);       // INPUT_PULLDOWN/INPUT_PULLUP

  //llamo a la función led con cada interrupción

  
  // RISING: cambia de LOW a HIGT cuando apretamos el boton
  // FALLING: cambia de HIGT a LOW cuando apretamos el boton
  // CHANGE: Cambia como apretar el boton como al soltarlo

  attachInterrupt(btn1, ISR_1, RISING);  // LOW/HIGH/FALLING/RISING/CHANGE
  attachInterrupt(btn2, ISR_2, RISING);  // LOW/HIGH/FALLING/RISING/CHANGE  
  attachInterrupt(btn3, ISR_3, RISING);  // LOW/HIGH/FALLING/RISING/CHANGE
  attachInterrupt(btn4, ISR_4, RISING);  // LOW/HIGH/FALLING/RISING/CHANGE 

  // Mensaje por puerto serial

  Serial.println("Proceso exitoso !!!");
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

  

    if (btnPress)
  {
    if (millis()-lastTime > DELAY_BTN)
    {
      Serial.print("¡Interrupción ");
      Serial.print(btnPress);
      Serial.println("!");

      lastTime = millis();
    }
    btnPress = 0;
  }
  Serial.println(i);
  i++;
delay(600);
}
