#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define WIFI_SSID "k"  
#define WIFI_PASSWORD "1234567891011"  

#define DHTPIN 17     
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to WiFi!");

  dht.begin();
}
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temperature = dht.readTemperature(); 
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C | Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    HTTPClient http;
    String serverUrl = "http://Secretmunaangpearson/temperature_logger/insert_data.php?temperature=" + 
                        String(temperature) + "&humidity=" + String(humidity);
    http.begin(serverUrl);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.println("Data Sent Successfully!");
    } else {
      Serial.print("Error Sending Data. HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected!");
  }
  delay(5000); 
}

