# Hydroponics
This is an automation project on hydroponics using ESP32.
The development on this project is still going on.
This is an IoT mini-project code written by Ayush Sharma (PG50). The code includes several functionalities and components:

    1. WiFi and ThingSpeak: The code connects to a WiFi network using the provided SSID and password. It sends sensor data (temperature, humidity, and other fields) to the ThingSpeak IoT platform using HTTP requests.

    2. pH Sensor: The code reads the pH value from an analog pH sensor connected to pin 36.

    3. LDR (Light Dependent Resistor): The code reads the light intensity using an LDR connected to pin 34.

    4. LED Control: The code controls an LED strip with 35 NeoPixels connected to pin 13. The LED brightness is adjusted based on the light intensity measured by the LDR.

    5. DHT11 Sensor: The code reads temperature and humidity data from a DHT11 sensor connected to pin 5.

    6. Relay Control: The code controls a relay connected to pin 2. It turns the relay on for a specified time (ON_TIME) and then off for another specified time (OFF_TIME) in an infinite loop.

    7. OLED Display: The code uses an SSD1306-based OLED display with a resolution of 128x64 pixels. It displays various sensor readings including water temperature, humidity, air temperature, light intensity, and pH value.

    8. DS18B20 Temperature Sensor: The code reads temperature data from a DS18B20 temperature sensor connected to pin 4 using the OneWire and DallasTemperature libraries.

The code sets up the necessary components in the setup() function and then continuously reads sensor data, controls the LED strip and relay, updates the OLED display, and sends data to ThingSpeak in the loop() function. Additionally, there is a separate loop2() function that runs on a second core, toggling the relay on and off at regular intervals.
Overall, this code integrates multiple sensors, actuators, and displays, and demonstrates the collection and transmission of sensor data in an IoT project.
