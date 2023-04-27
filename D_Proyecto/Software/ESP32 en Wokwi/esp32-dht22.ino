#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "ESP32";
const char* password = "12345678";

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

// Pines
int led1 = 25;
int led2 = 26;
int led3 = 27;
int led4 = 14;

int pot1 = 34;
int pot2 = 35;
int pot3 = 32;
int pot4 = 33;

void handleRoot() {
  String html = "<html><head><title>ESP32 LED Control</title></head><body>";
  html += "<h1>ESP32 LED Control</h1>";
  html += "<h2>LED 1</h2>";
  html += "<input type='range' min='0' max='255' value='0' oninput='led(1, this.value)'>";
  html += "<button onclick='led(1, 0)'>Off</button>";
  html += "<button onclick='led(1, 255)'>On</button>";
  html += "<h2>LED 2</h2>";
  html += "<input type='range' min='0' max='255' value='0' oninput='led(2, this.value)'>";
  html += "<button onclick='led(2, 0)'>Off</button>";
  html += "<button onclick='led(2, 255)'>On</button>";
  html += "<h2>LED 3</h2>";
  html += "<input type='range' min='0' max='255' value='0' oninput='led(3, this.value)'>";
  html += "<button onclick='led(3, 0)'>Off</button>";
  html += "<button onclick='led(3, 255)'>On</button>";
  html += "<h2>LED 4</h2>";
  html += "<input type='range' min='0' max='255' value='0' oninput='led(4, this.value)'>";
  html += "<button onclick='led(4, 0)'>Off</button>";
  html += "<button onclick='led(4, 255)'>On</button>";
  html += "<script>function led(led, value) {var xhr = new XMLHttpRequest();xhr.open('GET', '/?led=' + led + '&value=' + value, true);xhr.send(); ";
  server.send(200, "text/html", html);
}

void led(int led, int value) {
  analogWrite(led, value);
}

void setup() {
Serial.begin(9600);
pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
pinMode(led3, OUTPUT);
pinMode(led4, OUTPUT);
pinMode(pot1, INPUT);
pinMode(pot2, INPUT);
pinMode(pot3, INPUT);
pinMode(pot4, INPUT);

// Configuración de la conexión WiFi
WiFi.softAP(ssid, password);
WiFi.softAPConfig(local_ip, gateway, subnet);

// Mensaje de inicio en la consola
Serial.println("Punto de acceso WiFi creado");

// Configuración del servidor web
server.on("/", handleRoot);
server.begin();

// Mensaje de inicio en la consola
Serial.println("Servidor web iniciado");
}

void loop() {
server.handleClient();
}
