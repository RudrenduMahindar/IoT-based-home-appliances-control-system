//Code uploaded to NodeMCU (Esp8266 12E) through arduino IDE
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <NewPing.h>
#define LDRPIN D0
#define TRIGGERPIN D1
#define ECHOPIN D2
#defineMAX_DISTANCE 200
#define DHTPIN D3 //What digital pinwe're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT22 // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21 // DHT 21, AM2301
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
NewPing sonar(TRIGGER_PIN, ECHO_PIN,MAX_DISTANCE);
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";
// YourWiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";
// This function sends Arduino's up time every second toVirtual Pin (5).
// In the app,Widget's reading frequency should be set to PUSH. Thismeans
// that you define how often to send data to Blynk App.
void sendSensor()
{
float h = dht.readHumidity();
float t = dht.readTemperature(); //or dht.readTemperature(true) for Fahrenheit
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read fromDHT sensor!");
    return;
    }
  else{
    Serial.println("Humidity=");
    Serial.print(h);
    Serial.println("Temperature=");
    Serial.print(t);
    }
// You can send any value at any time.
// Please don't sendmore that 10 values per second.
Blynk.virtualWrite(V5, h);
Blynk.virtualWrite(V6, t);
}    

void setup()
{
// Debug console
Serial.begin(115200);
pinMode(LDRPIN, INPUT);
Blynk.begin(auth, ssid, pass);
// You can also specify server:
//Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
//Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);
dht.begin();
// Setup a function to be called every second
timer.setInterval(1000L, sendSensor);
}

void loop()
{
int distance;
delay(50);
Serial.print("Ping: ");
distance=sonar.ping_cm();
Serial.print(distance);
Serial.print("cm");
  if(distance<=6)
  { // emergency notification and email to turn off pump when water is about to overflow
    Blynk.notify("Hey,waterabout to overflow");
    Blynk.email("rudrendumahindar@gmail.com", "Subject:waterpump","waterwill overflow");
   //you can also turn off water-pump by writing code in this block to send a low signal from NodeMCU to water-pump
  }
int sensorValue=digitalRead(LDRPIN); // read the input on digital pin 0
Blynk.run();
timer.run();
Blynk.virtualWrite(V1, distance);// set virtual pins in the Blynk app to receive sensor data from NodeMCU to Blynk server 
if(sensorValue==1){
  Serial.println("brightness is high");
  Blynk.virtualWrite(V0, "HIGH");
}
else
{ 
  Serial.println("brightness is low");
  Blynk.virtualWrite(V0, "LOW");
}
delay(3500);
}
