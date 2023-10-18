#include <math.h>
#include <WiFi.h>
#include <aREST.h>
#include <DHT.h>

// DHT11 sensor pins
#define DHTPIN 21
#define DHTTYPE DHT11

// LED pin (D18 on the ESP32)
#define LED_PIN 18

// Create aREST instance
aREST rest = aREST();

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// WiFi parameters
const char* ssid = "Proxima";
const char* password = "centauri";
//Static IP address configuration
IPAddress local_IP(192, 168, 1, 231);  // Set the desired IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

#define LISTEN_PORT 80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API
float temperature;
float humidity;
char* location = "Al Fresco";
int timer = 72000;
int ledState = LOW;

// Declare functions to be exposed to the API
int ledControl(String command);

void setup(void) {
  // Start Serial
  Serial.begin(115200);

  // Connect to WiFi with static IP
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  // Connect to the WiFi network
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Init DHT
  dht.begin();

  // Init variables and expose them to REST API
  rest.variable("temperature", &temperature);
  rest.variable("humidity", &humidity);
  rest.variable("location", location);

  pinMode(LED_PIN, OUTPUT); // Set LED pin as an output
  digitalWrite(LED_PIN, ledState); // Initialize LED state
}

void loop() {
  // Reading temperature and humidity
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Prints the temperature in celsius
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);

  if (!isnan(temperature) && !isnan(humidity)) {
    // The data retrieval was successful
    // Turn on the LED
    digitalWrite(LED_PIN, LOW); // Turn on the LED
    delay(1000); // Keep the LED on for 1 second
    digitalWrite(LED_PIN, HIGH); // Turn off the LED
  }

  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  rest.handle(client);
}

// Custom function accessible by the API to control the LED
int ledControl(String command) {
  if (command == "ON") {
    ledState = HIGH; // Turn on LED
    digitalWrite(LED_PIN, ledState);
  } else if (command == "OFF") {
    ledState = LOW; // Turn off LED
    digitalWrite(LED_PIN, ledState);
  }
  return ledState;
}
