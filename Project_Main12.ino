

#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
SoftwareSerial SerialGPS(4, 5);
String longitude;
String latitude;
#define trigger_pin 12
#define Echo_pin 14
#define buzzer 4
long duration;
int distance;
int F;
int v;
String apiKey= "L6J28VS5AVVXAT9A";
/* Set these to your desired credentials. */
const char *ssid = "do";
const char *password = "12345678";
const char *server = "api.thingspeak.com";
//String Nun = "WL CONECTED";
WiFiClient client;
void setup() {
// put your setup code here, to run once:
pinMode(trigger_pin, OUTPUT); // configure the trigger_pin(D9) as an Output
pinMode(buzzer, OUTPUT); // Set the LED (D13) pin as a digital output
pinMode(Echo_pin, INPUT); // configure the Echo_pin(D11) as an Input
pinMode(A0, INPUT);
Serial.begin(115200);
delay(500);
SerialGPS.begin(9600);
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid,password);
while(WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.print("");
Serial.println("WiFi Connected");
}

void loop() {
// put your main code here, to run repeatedly:
v = analogRead(A0);
F = 0;
digitalWrite(trigger_pin, LOW); //set trigger signal low for 2us
delayMicroseconds(2);
/*send 10 microsecond pulse to trigger pin of HC-SR04 */
digitalWrite(trigger_pin, HIGH); // make trigger pin active high
delayMicroseconds(10); // wait for 10 microseconds
digitalWrite(trigger_pin, LOW); // make trigger pin active low
/*Measure the Echo output signal duration or pulss width */
duration = pulseIn(Echo_pin, HIGH); // save time duration value in "duration variable
distance= duration*0.034/2; //Convert pulse duration into distance
/* if distance greater than 10cm, turn on LED */
if ( distance < 10){
digitalWrite(buzzer, HIGH);
F = 79;
}else{
digitalWrite(buzzer, LOW);
}
// print measured distance value on Arduino serial monitor
Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");
showData();
delay(1000);
latitude = String (gps.location.lat(), 6);
longitude = String (gps.location.lng(), 6);
if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
{
String postStr = apiKey;
postStr += "&field1=";
postStr += String(latitude);
postStr += "&field2=";
postStr += String(latitude);
postStr += "&field3=";
postStr += String(F);
postStr += "&field4=";
postStr += String(v);
postStr += "\r\n\r\n\r\n";
client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);
delay(1000);
}
client.stop();
}
void showData()
{
while (SerialGPS.available())
if (gps.encode(SerialGPS.read()))
//showData();
if (millis() > 5000 && gps.charsProcessed() < 10)
{
Serial.println("GPS NOT DETECTED!");
while(true);
}
if (gps.location.isValid())
{
Serial.print("Latitude: ");
Serial.println(gps.location.lat(), 6);
Serial.print("Longitude: ");
Serial.println(gps.location.lng(), 6);
Serial.print("Altitude: ");
Serial.println(gps.altitude.meters());
}
else
{
Serial.println("Location is not available");
}
Serial.print("Date: ");
if (gps.date.isValid())
{
Serial.print(gps.date.month());
Serial.print("/");
Serial.print(gps.date.day());
Serial.print("/");
Serial.println(gps.date.year());}
else
{
Serial.println("Not Available");
}
Serial.print("Time: ");
if (gps.time.isValid())
{
if (gps.time.hour() < 10) Serial.print(F("0"));
Serial.print(gps.time.hour());
Serial.print(":");
if (gps.time.minute() < 10) Serial.print(F("0"));
Serial.print(gps.time.minute());
Serial.print(":");
if (gps.time.second() < 10) Serial.print(F("0"));
Serial.print(gps.time.second());
Serial.print(".");
if (gps.time.centisecond() < 10) Serial.print(F("0"));
Serial.println(gps.time.centisecond());
}
else
{
Serial.println("Not Available");
}
}
