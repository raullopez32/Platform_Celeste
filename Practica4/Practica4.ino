#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

const char* ssid = "wifi"; // Nombre de tu red WiFi
const char* password = "contraseña"; // Contraseña de tu red WiFi

const int DHTPin = 5; // Pin al que está conectado el DHT11
const int DHTType = DHT11; // Tipo de sensor DHT (DHT11, DHT21, DHT22)

DHT dht(DHTPin, DHTType);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Conectando a la red WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }
  Serial.println("Conectado a la red WiFi");

  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  enviarDatos(temperature, humidity);

  delay(60000); // Espera 1 minuto antes de enviar nuevamente los datos
}

void enviarDatos(float temperatura, float humedad) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // URL de la página web alojada en 000webhost que recibe los datos
    String url = "https..." + String(temperatura) + "&humedad=" + String(humedad);

    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String respuesta = http.getString();
      Serial.print("Respuesta del servidor: ");
      Serial.println(respuesta);
    } else {
      Serial.print("Error en la solicitud HTTP: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
}
