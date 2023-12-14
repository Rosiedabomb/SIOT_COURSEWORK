#include <PubSubClient.h>
#include <WiFi.h> //including the Wifi library
#include <DHT.h> //including library for the temperature and humidity sensor.

//Wifi setup
const char* ssid = "CommunityFibreXXXXXXXX"; // Wifi network
const char* password = "XXXXXXXX"; // Wifi Password
const char* mqtt_server = "XXX.XXX.XXX.XXX"; //IP Address for MQTT Broker (on my laptop)
float low_threshold_temp = 18.5; //Below this temperatue actuation is triggered
float high_threshold_temp = 20.0; //above this temperature actuation is triggered
float threshold_light = 3500; // below this value (brighter than) actuation is triggered

DHT dht(26, DHT11); //setting up DHT (digital humidity & temperature) sensor. first argument is pin and second is sensor type
const int AO_PIN_light = 36; //defining the light sensor variable for GPIO32 

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
float temperature = 0;

void setup() {
 Serial.begin(9600);
 pinMode (18, OUTPUT); //Green LED
 pinMode (19, OUTPUT); //blue LED
 pinMode (16, OUTPUT); //Red LED
 dht.begin(); //TEMPERATURE SENSOR SETUP
 setup_wifi();
 client.setServer(mqtt_server, 1883);
  
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void sleep() {
  //write if statement to turn on LED if either the light level is too high or the temperatue is too high/low.
  float temp = dht.readTemperature();
  int lightValue = analogRead(AO_PIN_light);
  
  if (temp < low_threshold_temp) {
    digitalWrite(19,HIGH);//LOWTEMP (BLUE)
  } else {
      digitalWrite(19,LOW); //LOWTEMP (BLUE)
  }
  
  if (lightValue < threshold_light) {
    digitalWrite(18,HIGH);// LIGHT (GREEN)
  } else {
      digitalWrite(18,LOW); //LIGHT (GREEN)
  }
  
  if (temp > high_threshold_temp) {
    digitalWrite(16,HIGH);//HIGHTEMP (RED)
  } else {
      digitalWrite(16,LOW); //HIGHTEMP (RED)
    }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("temperature");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  
  //reconnect if wifi disconnects
  delay(1000);
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //If temperature is too warm/cold or light levels are too high turn on respective LED.
  sleep();

  //Publishing data at intervals through MQTT Protocol
  long now = millis();
  if (now - lastMsg > 600000) {
    lastMsg = now;
    // Temperature reading
    float temp = dht.readTemperature();
    //Light level reading
    int lightValue = analogRead(AO_PIN_light);
    delay(2000);
    // Convert the value to a char array
    char tempStringa[6];
    char tempStringb[6];
    dtostrf(temp, 5, 2, tempStringa);
    itoa(lightValue,tempStringb,10);
    Serial.println(lightValue);
    Serial.println(temp);
    Serial.print("Light Level: ");
    Serial.println(tempStringb);
    Serial.print("Temperature: ");
    Serial.println(tempStringa);
    client.publish("temperature", tempStringa);
    client.publish("light", tempStringb);

  }
}
