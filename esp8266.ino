

#include <ESP8266WiFi.h>
 
String apiKey = "WX6L5476WD7ZYWIT";     //  Enter your Write API key from ThingSpeak
 
const char *ssid =  "SHANTO";     // replace with your wifi ssid and wpa2 key
const char *pass =  "abcdefgh";
const char* server = "api.thingspeak.com";

WiFiClient client;

String alldata;
void setup() // put your setup code here, to run once:
{
     Serial.begin(115200);
       delay(10);
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500); 
     }
}
void loop() { // run over and over
if (Serial.available()) {
alldata = Serial.readString();

}
String data = alldata;

int batteryStart = data.indexOf(":") + 2;       // find position of colon for battery voltage
int batteryEnd = data.indexOf("V", batteryStart);  // find position of units for battery voltage
int solarStart = data.indexOf(":", batteryEnd) + 2;  // find position of colon for solar voltage
int solarEnd = data.indexOf("V", solarStart);         // find position of units for solar voltage
int currentStart = data.indexOf(":", solarEnd) + 2;   // find position of colon for solar current
int currentEnd = data.indexOf("A", currentStart);      // find position of units for solar current
int powerStart = data.indexOf(":", currentEnd) + 2;   // find position of colon for power
int powerEnd = data.indexOf("W", powerStart);          // find position of units for power

String batteryString = data.substring(batteryStart, batteryEnd);   // extract battery voltage data as string
String solarString = data.substring(solarStart, solarEnd);         // extract solar voltage data as string
String currentString = data.substring(currentStart, currentEnd);   // extract solar current data as string
String powerString = data.substring(powerStart, powerEnd);         // extract power data as string

float batteryVoltage = batteryString.toFloat();  // convert battery voltage data to float
float solarVoltage = solarString.toFloat();      // convert solar voltage data to float
float solarCurrent = currentString.toFloat();    // convert solar current data to float
float power = powerString.toFloat(); // convert power data to float
//Serial.print("bat volt: ");
//Serial.println(batteryVoltage);
//Serial.print("solar volt: ");
//Serial.println(solarVoltage);
//Serial.print("solar current: ");
//Serial.println(solarCurrent);
//Serial.print("power: ");
//Serial.println(power);

 
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(batteryVoltage);
                             postStr +="&field2=";
                             postStr += String(solarVoltage);
                             postStr +="&field3=";
                             postStr += String(solarCurrent);
                             postStr +="&field4=";
                             postStr += String(power);
                             postStr += "\r\n\r\n\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                          
                        }
          client.stop();
 
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);

}
