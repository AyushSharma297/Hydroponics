/*******************************
  IOT MINI PROJECT CODE
    PG50 Ayush Sharma

    Thinkspeak Dashbord Link : https://thingspeak.com/channels/2133991
    Link: "QV8BN5"
**********************************/

// Include the necessary libraries
#include <OneWire.h>            // Library for communicating with 1-wire devices
#include <DallasTemperature.h>  // Library for communicating with DS18B20 temperature sensor
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>

/******************____Thinkspeak___*****************/
const char* ssid = "Ayush";
const char* password = "11111111";
const char* server = "api.thingspeak.com";
const String apiKey = "43J5DPPRS2";

/******************____PH Sensor___*****************/
const int sensorPin = 36;  // Analog pin for pH sensor
float voltage, pH;

/******************____LDR___*****************/
const int ldrPin = 34; // Define the analog input pin for the LDR sensor

/******************____LED___*****************/
const int LED_PIN = 13 ; //Pin no.
#define NUM_LEDS 35 //no of leds 
        Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


/******************____DHT11___*****************/
#define DHTPIN 5 
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22
        DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor.
        

/******************____Relay___*****************/
#define RELAY_PIN 2  // Replace with the actual GPIO pin number that is connected to the relay
#define ON_TIME 10000 // Time (in milliseconds) the relay should be on
#define OFF_TIME 10000  // Time (in milliseconds) the relay should be off


/******************____OLED___*****************/
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // OLED reset pin (if your display has one)
        Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Initialize the OLED display using the SSD1306 driver
        

/******************____DS18B20___*****************/
#define ONE_WIRE_BUS 4 // Define the GPIO pin connected to the DS18B20 sensor
        OneWire oneWire(ONE_WIRE_BUS); // Create an instance of the OneWire class to communicate with the DS18B20 sensor 
        DallasTemperature sensors(&oneWire); // Create an instance of the DallasTemperature class to read temperatures from the DS18B20 sensor
        

TaskHandle_t Task1; //Using task handle to use second core

void loop2(void * parameter ){ //second loop working paralle to main
  for(;;){
 /******************____Relay___*****************/
  digitalWrite(RELAY_PIN, HIGH);  // Turn on the relay
  delay(ON_TIME);  // Wait for the specified ON_TIME duration
  digitalWrite(RELAY_PIN, LOW);  // Turn off the relay
  delay(OFF_TIME);  // Wait for the specified OFF_TIME duration

  }
}


// Setup function - runs once when the device is first powered on or reset
void setup() {

 xTaskCreatePinnedToCore( // initialising the core
  loop2,
  "buttonCheck",
  1000,
  NULL,
  1,
  &Task1,
  0
  ); 
 
  Serial.begin(115200);  // Start serial communication at 115200 baud
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  

/******************____LED___*****************/
  strip.begin();
  strip.setBrightness(255); // Set brightness to full (100%)
  strip.show(); // Initialize all pixels to 'on'

/******************____DHT11___*****************/
  dht.begin();

/******************____Relay___*****************/
  pinMode(RELAY_PIN, OUTPUT);

/******************____DS18B20___*****************/
  sensors.begin(); // Initialize the DallasTemperature library
  
/******************____OLED___*****************/
   Wire.begin(21, 22); //Initialise SLC and SDA Pins
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Initialize the OLED display
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Infinite loop
    }
    display.clearDisplay(); // Clear the OLED display buffer
    display.setTextSize(1); // Set the text size and color
    display.setTextColor(SSD1306_WHITE);



}

// Loop function - runs continuously after setup() is complete
void loop() {  
delay(1000);

/******************____PH Sensor___*****************/
// Read pH sensor voltage
  voltage = analogRead(sensorPin) * 5.0 / 1024.0;

  // Convert voltage to pH value (for this specific sensor)
  pH = 7 - (voltage - 2.5) / 0.18;

/******************____LDR___*****************/  
  int rawValue = analogRead(ldrPin); // Read the raw analog value from the LDR sensor
  int percentage = map(rawValue, 0, 4095, 0, 100); // Convert the raw value to a percentage (0-100)
  int brightness = map(percentage, 0, 100, 255, 0); // Calculate the LED brightness based on the LDR reading

/******************____LED___*****************/
  strip.setBrightness(brightness); // Set brightness to Light Intensity 
  for (int i = 0; i < NUM_LEDS; i++) {  // Set all pixels to ultraviolet color
    strip.setPixelColor(i, 75, 0, 130);
  }
  strip.show();
  
/******************____DHT11___*****************/
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Read temperature as Celsius (the default)

  


/******************____DS18B20___*****************/
  sensors.requestTemperatures(); // Request a temperature reading from the DS18B20 sensor
  float temperatureC = sensors.getTempCByIndex(0); // Read the temperature in Celsius from the DS18B20 sensor

/******************____SerialPrint___*****************/  
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  
  
  Serial.print(" WaterTemp: "); // Print the temperature reading to the serial monitor
  Serial.print(temperatureC);
  Serial.print("°C ");

  Serial.print("LightIntensity: ");
  Serial.println(percentage);

  delay(1000);  // Delay for 1 second to avoid spamming the serial port with too many readings

 /******************____OLEDDISPLAY___*****************/ 
  display.setCursor(0, 0); // Draw the text "WaterTemp:" on the OLED display
  display.print("WTemp   :");
  display.println(temperatureC);
  display.setCursor(0, 10);
  display.print("Humidity:");
  display.print(h);
  display.setCursor(0, 20);
  display.print("ATemp   :");
  display.print(t);
  display.setCursor(0, 30);
  display.print("Light   :");
  display.print(percentage);
  display.setCursor(0, 40);
  display.print("PH      :");
  display.println(pH);
  display.display();
  
/******************____Thinkspeak___*****************/
 
  HTTPClient http;
  int value2 = 1;
  // create the URL with the API key and values to send
  String url = "http://";
  url += server;
  url += "/update?api_key=";
  url += apiKey;
  url += "&field1=";
  url += String(temperatureC);
  url += "&field2=";
  url += String(value2);
  url += "&field3=";
  url += String(h);
  url += "&field4=";
  url += String(t);

  // send the HTTP POST request
  http.begin(url);
  int httpCode = http.POST("");
  if (httpCode > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.println("Error sending request");
  }

  http.end();

}