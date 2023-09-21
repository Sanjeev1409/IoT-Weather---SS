/*
*  Weather Station App
*  CGS Semester 2
*  Task 2
*  Author: your name here
*/

#include <math.h>
#include <WiFi.h>
#include <aREST.h>
#include <DHT.h>

// DHT11 sensor pins
#define DHTPIN 21
#define DHTTYPE DHT11

// Create aREST instance
aREST rest = aREST();

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// WiFi parameters
const char* ssid = "Proxima";
const char* password = "centauri";
//Static IP address configuration
// P connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API
float temperature;
float humidity;
char* location = "Al Fresco";
int timer = 72000;

// Declare functions to be exposed to the API
int ledControl(String command);

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);
  
  // Init DHT 
  dht.begin();
  
  // Init variables and expose them to REST API
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);
  rest.variable("location",&location);
    
  // Give name and ID to device
<<<<<<< HEAD:ws-dht11/ws-dht11.ino
  rest.set_id("209");
  rest.set_name("alpha-209");
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  IPAddress ip(192, 168, 1, 209); //set static ip
 
  IPAddress gateway(192, 168, 1, 1); //set getteway
  Serial.print(F("Setting static ip to : "));
  Serial.println(ip);
  IPAddress subnet(255, 255, 255, 0);//set subnet
  WiFi.config(ip, gateway, subnet);

  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
  
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
  Serial.print("Timer: ");
  Serial.println(timer);
  delay(5000);
  timer--;

  //Check running time and reset if expired
  if (timer == 0 ) {
    delay(3000);
    Serial.println("Resetting..");
    ESP.restart();
  }
  
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
  //Serial.println("called");
}

// Custom function accessible by the API
int ledControl(String command) {

  // Get state from command
  int state = command.toInt();

  digitalWrite(6,state);
  return 1;
}