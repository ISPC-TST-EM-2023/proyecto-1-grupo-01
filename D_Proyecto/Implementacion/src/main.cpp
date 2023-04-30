#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

// defino los pines como entradas Analógicas
#define adc_1 32
#define adc_2 35
#define adc_3 34
#define adc_4 39

// defino los pines como entradas Digitales
// pines del 34 al 39 no tienen resistencia PULLUP
#define btn1 16
#define btn2 17
#define btn3 5
#define btn4 18

// Variables auxiliares para almacenar el estado de salida actual
String estadoLed1 = "off";
String estadoLed2 = "off";
String estadoLed3 = "off";
String estadoLed4 = "off";

#define DELAY_BTN 600   // 500ms

// Asigne variables de salida a los pines GPIO
const int Led1 = 25;
const int Led2 = 33;
const int Led3 = 26;
const int Led4 = 27;

int i = 0;
int btnPress = 0;
long lastTime = 0;

// creo una función para el estado del led
void ISR_1(){
  if (!digitalRead(btn1)){
    if (estadoLed1 == "off") {
      estadoLed1 = "on";
      digitalWrite(Led1, HIGH);
    } else{
      estadoLed1 = "off";
      digitalWrite(Led1, LOW);
    }
  } 
}

void ISR_2(){
    if (!digitalRead(btn2)){
      if (estadoLed2 == "off") {
        estadoLed2 = "on";
        digitalWrite(Led2, HIGH);
      } else{
        estadoLed2 = "off";
        digitalWrite(Led2, LOW);
      }
    } 
  } 

void ISR_3(){
  if (!digitalRead(btn3)){
      if (estadoLed3 == "off") {
        estadoLed3 = "on";
        digitalWrite(Led3, HIGH);
      } else{
        estadoLed3 = "off";
        digitalWrite(Led3, LOW);
      }
    } 
}

void ISR_4(){
  if (!digitalRead(btn4)){
      if (estadoLed4 == "off") {
        estadoLed4 = "on";
        digitalWrite(Led4, HIGH);
      } else{
        estadoLed4 = "off";
        digitalWrite(Led4, LOW);
      }
    } 
}
// Reemplace con sus credenciales de red
const char* ssid = "TP-Link GASTON";
const char* password = "gastonloco";

/* Detalles de las IP */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// Establezca el número de puerto del servidor web en 80
WiFiServer server(80);

// Variable para almacenar la solicitud HTTP
String header;

// Tiempo actual
unsigned long currentTime = millis();
// Momento anterior
unsigned long previousTime = 0; 
// Definir el tiempo de espera en milisegundos (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  //Inicializar las variables de salida como salidas
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  // Establecer salidas en BAJO
  digitalWrite(Led1, LOW);
  digitalWrite(Led2, LOW);
  digitalWrite(Led3, LOW);
  digitalWrite(Led4, LOW);
  // Configuramos las entradas digitales como interrupciones

  pinMode(btn1, INPUT_PULLUP);       // INPUT_PULLDOWN/INPUT_PULLUP
  pinMode(btn2, INPUT_PULLUP);       // INPUT_PULLDOWN/INPUT_PULLUP
  pinMode(btn3, INPUT_PULLUP);       // INPUT_PULLDOWN/INPUT_PULLUP
  pinMode(btn4, INPUT_PULLUP);       // INPUT_PULLDOWN/INPUT_PULLUP

  //llamo a la función led con cada interrupción
  // RISING: cambia de LOW a HIGH cuando apretamos el boton
  // FALLING: cambia de HIGH a LOW cuando apretamos el boton
  // CHANGE: Cambia como apretar el boton como al soltarlo

  attachInterrupt(btn1, ISR_1, RISING);  // LOW/HIGH/FALLING/RISING/CHANGE
  attachInterrupt(btn2, ISR_2, RISING);  // LOW/HIGH/FALLING/RISING/CHANGE  
  attachInterrupt(btn3, ISR_3, RISING);  // LOW/HIGH/FALLING/RISING/CHANGE
  attachInterrupt(btn4, ISR_4, RISING);  // LOW/HIGH/FALLING/RISING/CHANGE 

  // Conéctese a la red Wi-Fi con SSID y contraseña
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Imprima la dirección IP local e inicie el servidor web
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("Dirección IP:");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){


// en esta linea iria la lectura de los 4 potes que se imprimen por serial

  WiFiClient client = server.available();   // Escuche los clientes entrantes

  if (client) {                             // Si un nuevo cliente se conecta,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("Cliente nuevo.");          // imprimir un mensaje en el puerto serie
    String currentLine = "";                // hacer una cadena para contener los datos entrantes del cliente
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // Bucle mientras el cliente está conectado
      currentTime = millis();
      if (client.available()) {             // Si hay bytes para leer del cliente,
        char c = client.read();             // leer un byte, entonces
        Serial.write(c);                    // imprimirlo en el monitor serie
        header += c;
        if (c == '\n') {                    // si el byte es un carácter de nueva línea
          // si la línea actual está en blanco, tiene dos caracteres de nueva línea seguidos.
          // ese es el final de la solicitud HTTP del cliente, así que envíe una respuesta:
          if (currentLine.length() == 0) {
            // Los encabezados HTTP siempre comienzan con un código de respuesta (e.g. HTTP/1.1 200 OK)
            // y un tipo de contenido para que el cliente sepa lo que viene, luego una línea en blanco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Conexión: cerrar");
            client.println();
            
            // enciende y apaga los GPIO
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("LED Nº1 on");
              estadoLed1 = "on";
              digitalWrite(Led1, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("LED Nº1 off");
              estadoLed1 = "off";
              digitalWrite(Led1, LOW);

            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("LED Nº2 on");
              estadoLed2 = "on";
              digitalWrite(Led2, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("LED Nº2 off");
              estadoLed2 = "off";
              digitalWrite(Led2, LOW);

            }else if (header.indexOf("GET /28/on") >= 0) {
              Serial.println("LED Nº3 on");
              estadoLed3 = "on";
              digitalWrite(Led3, HIGH);
            } else if (header.indexOf("GET /28/off") >= 0) {
              Serial.println("LED Nº3 off");
              estadoLed3 = "off";
              digitalWrite(Led3, LOW);

            }else if (header.indexOf("GET /29/on") >= 0) {
              Serial.println("LED Nº4 on");
              estadoLed4 = "on";
              digitalWrite(Led4, HIGH);
            } else if (header.indexOf("GET /29/off") >= 0) {
              Serial.println("LED Nº4 off");
              estadoLed4 = "off";
              digitalWrite(Led4, LOW);
            }
            
            // Mostrar la página web HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS para dar estilo a los botones de encendido/apagado
            // Siéntase libre de cambiar los atributos de color de fondo y tamaño de fuente para que se ajusten a sus preferencias
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Encabezado de página web
            client.println("<body><h1>ESP32 Web Server Proyecto01Grupo01</h1>");
            
            // Muestra el estado actual y los botones ON/OFF para GPIO 25 
            client.println("<p>LED 1 - Estado " + estadoLed1 + "</p>");
            // Si el estado de salida 25 está apagado, muestra el botón ENCENDIDO  
            if (estadoLed1=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            }                
            // Muestra el estado actual y los botones ON/OFF para GPIO 26 
            client.println("<p>LED 2 - Estado " + estadoLed2 + "</p>");
            // Si el estado de salida 26 está apagado, muestra el botón ENCENDIDO       
            if (estadoLed2=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

           // Muestra el estado actual y los botones ON/OFF para GPIO 27 
            client.println("<p>LED 3 - Estado " + estadoLed3 + "</p>");
            // Si el estado de salida 27 está apagado, muestra el botón ENCENDIDO       
            if (estadoLed3=="off") {
              client.println("<p><a href=\"/28/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/28/off\"><button class=\"button button2\">OFF</button></a></p>");
            }                
            // Muestra el estado actual y los botones ON/OFF para GPIO 14 
            client.println("<p>LED 4 - Estado " + estadoLed4 + "</p>");
            // Si el estado de salida 14 está apagado, muestra el botón ENCENDIDO  
            if (estadoLed4=="off") {
              client.println("<p><a href=\"/29/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/29/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // La respuesta HTTP termina con otra línea en blanco
            client.println();
            // Break out of the while loop
            break;
          } else { // si tiene una nueva línea, borre currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si tiene algo más que un carácter de retorno de carro,
          currentLine += c;      // agréguelo al final de la línea actual
        }
      }
    }
    // Borrar la variable de encabezado
    header = "";
    // Cerrar la conexión
    client.stop();
    Serial.println("Cliente desconectado.");
    Serial.println("");
  }
}