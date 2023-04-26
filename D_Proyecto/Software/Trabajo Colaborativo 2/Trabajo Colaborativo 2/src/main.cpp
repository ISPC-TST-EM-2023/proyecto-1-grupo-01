#include <Arduino.h>
#include <WiFi.h>

// Reemplace con su nombre de red y contraseña  
const char* ssid = "SSID"; // Nombre de la red
const char* password = "Pasword"; // Contraseña de la red 

// Establece el número de puerto para el servidor web
WiFiServer server(80);

// Variable para almacenar el estado actual de la salida
String header;

// Variable auxiliar para almacenar el estado actual de la salida
String output2State = "off";

// Asigna el número de pin de la salida pin con led incorporado
const int output2 = 2;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define el tiempo de espera en 2000ms (2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Ajusta GPIO2 como salida
  pinMode(output2, OUTPUT);
  // Lo inicializa en estado bajo
  digitalWrite(output2, LOW);

  // Conecta a la red Wi-Fi
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Imprime la dirección IP asignada por el router
  // Esta es la IP que debe usar para acceder al servidor web
  Serial.println("");
  Serial.println("Conexión establecida.");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Escucha a los clientes que se conecten

  if (client) {                             // Si un nuevo cliente se conecta,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("Nuevo cliente.");          // imprime un mensaje en el puerto serie
    String currentLine = "";                // crea una variable para almacenar los datos entrantes del cliente
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // mientras el cliente esté conectado
      currentTime = millis();
      if (client.available())
      {                         // si hay bytes para leer del cliente,
        char c = client.read(); // lee un byte, y luego
        Serial.write(c);        // imprímelo en el monitor serie
        header += c;
        if (c == '\n')
        { // si el byte es un carácter de nueva línea
          // si la línea actual está vacía, tienes dos caracteres de nueva línea seguidos.
          // ese es el final de la solicitud HTTP del cliente, así que envía una respuesta:
          if (currentLine.length() == 0)
          {
            // Las cabeceras HTTP siempre comienzan con un código de respuesta (por ejemplo, HTTP/1.1 200 OK)
            // y un tipo de contenido para que el cliente sepa lo que viene, luego una línea en blanco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // enciende o apaga el LED
            if (header.indexOf("GET /2/on") >= 0)
            {
              Serial.println("Encendiendo LED");
              output2State = "on";
              digitalWrite(output2, HIGH);
            }
            else if (header.indexOf("GET /2/off") >= 0)
            {
              Serial.println("Apagando LED");
              output2State = "off";
              digitalWrite(output2, LOW);
            }

            // Ahora agregamos la parte de la página web
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println(".button2 {background-color: #555555;}</style></head>");
            // Web Page Heading
            client.println("<body><h1>Control LED desde página web</h1>");

            // Display current state, and ON/OFF buttons for GPIO 2
            client.println("<p>Estado LED - " + output2State + "</p>");
            if (output2State == "off")
            {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Cliente desconectado.");
    Serial.println("");
  }
}