#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>

// Change the credentials below, so your ESP8266 connects to your router
const char *ssid = "WIFI_SSID";
const char *password = "WIFI_PASSWORD";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char *mqtt_server = "MQTT_Broker_IP";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);

// DHT Sensor - D2&D3 on ESP-12E NodeMCU board
int const DHTPin = D2;

// Initialize DHT sensor. change the line below whatever DHT type you're using DHT11, DHT21 (AM2301), DHT22 (AM2302, AM2321)
#define DHTTYPE DHT11
DHT dht(DHTPin, DHTTYPE);
// Timers auxiliar variables
unsigned long now = millis();
unsigned long lastMeasure = 0;

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
    if (client.connect("greenhouse_sensors"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  // DHTs setup
  dht.begin();
}

// For this project, you don't need to change anything in the loop function. Basically it ensures that you ESP is connected to your broker
void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  if (!client.loop())
    client.connect("greenhouse_sensors");

  now = millis();
  // Publishes new temperature and humidity every 30 seconds
  if (now - lastMeasure > 3000)
  {
    lastMeasure = now;

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float humid = dht.readHumidity();
    // Read temperature as Celsius
    float temp = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(humid) || isnan(temp))
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Convert float to string
    static char temperature[7];
    static char humidity[7];
    dtostrf(temp, 6, 2, temperature);
    dtostrf(humid, 6, 2, humidity);
    

    // Publishes Temperature and Humidity values in string type
    client.publish("local/temp", temperature);
    client.publish("local/humid", humidity);

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C ");
  }
}