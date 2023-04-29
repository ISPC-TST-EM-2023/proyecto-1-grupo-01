#include <Arduino.h>
#include <WiFi.h>

// Reemplazar con el nombre y contraseña de tu red Wi-Fi
const char* ssid = "SSID";
const char* password = "password";

// Puerto para el servidor web
const uint16_t serverPort = 80;
WiFiServer server(serverPort);

// Variables para el estado de las salidas
const uint8_t output2 = 2;
String output2State = "off";

// Variables para controlar el tiempo de espera
const uint32_t timeoutTime = 2000;
uint32_t previousMillis = 0;

void setup() {
  Serial.begin(115200);

  // Configurar la salida como salida
  pinMode(output2, OUTPUT);
  // Inicializar la salida en estado bajo
  digitalWrite(output2, LOW);

  // Conectar a la red Wi-Fi
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conexión establecida.");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Escuchar a los clientes que se conecten

  if (client) {
    Serial.println("Nuevo cliente.");      // Imprimir un mensaje en el puerto serie
    String header;

    // Esperar a que se envíe la solicitud HTTP
    previousMillis = millis();
    while (client.connected() && (millis() - previousMillis) < timeoutTime) {
      if (client.available()) {
        char c = client.read();
        header += c;
        if (header.endsWith("\r\n\r\n")) {
          break;
        }
      }
    }

    // Manejar la solicitud HTTP
    if (header.startsWith("GET /2/on")) {
      Serial.println("Encendiendo LED");
      output2State = "on";
      digitalWrite(output2, HIGH);
    } else if (header.startsWith("GET /2/off")) {
      Serial.println("Apagando LED");
      output2State = "off";
      digitalWrite(output2, LOW);
    }

        // Enviar la respuesta HTTP
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<title>Control LED desde página web</title>");
    client.println("<style>html { font-family: Arial; text-align: center;}");
    client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
    client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    client.println(".button2 {background-color: #555555;}</style></head>");
    client.println("<body><h1>Control LED desde página web</h1>");
    client.print("<p>Estado LED: ");
    client.print(output2State);
    client.println("</p>");
    if (output2State == "off") {
      client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
    } else {
      client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
      client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
    }
    client.println("</body></html>");

    // Esperar un momento antes de cerrar la conexión
    delay(10);
    client.stop();
    Serial.println("Cliente desconectado.");
    Serial.println("");
  }
}
