#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>

#define DHTPIN4 4  //Puerto para LED

const char* ssid = "DESKTOP-GUSO5N0";
const char* password = "12345678";

String serverName = "http://utsapicar.somee.com/api/ldrs/1";

WiFiServer server(80); // Inicializa el servidor en puerto 80

int valorled = 0;  // Variable para led apagado

void setup() {
  Serial.begin(115200); // Baudios
  
  pinMode(DHTPIN4, OUTPUT);  // Inicia LED apagado
  digitalWrite(DHTPIN4, LOW); // Apaga LED al iniciar

  Serial.println();
  Serial.println("El puerto serie iniciado a 115200");
  Serial.println();
  Serial.print(F("Conectando a "));
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  // Ciclo para esperar la conexión
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectada");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    String serverPath = serverName;
    http.begin(client, serverPath.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();
    //http.addHeader("Content-Type", "application/json");

    // Validación de respuesta del servidor
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      String payload = http.getString();
      Serial.println(payload);
      
      JSONVar objetos = JSON.parse(payload);
      valorled = (int)objetos["value"];
      
      Serial.println("VALOR LED :" + (String)valorled);
      valorled == 1 ? digitalWrite(DHTPIN4, HIGH) : digitalWrite(DHTPIN4, LOW);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  
  delay(500);
}