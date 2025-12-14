#include "DHT.h"
#include <WiFi.h>
#include <TinyGPSPlus.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID ""
#define WIFI_PASSWORD "123123456"
#define API_KEY "AIzaSyBSjD-YEVOw0uyE-ae5DYgaIr2uZaKP6AU"
#define DATABASE_URL "https://smart-helmet-system-c95f1-default-rtdb.asia-southeast1.firebasedatabase.app/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int ldrValue = 0;
float humidity = 0.0;
float temperature = 0.0;
int sensorValue = 0;

/* ------------------- SENSOR PINS ------------------- */
#define DHTPIN 13
#define DHTTYPE DHT22
#define MQ2_PIN 33

// Temperature LEDs
int Temp1ledPin = 14;
int Temp2ledPin = 25;
int Temp3ledPin = 26;

// Gas LEDs
int gas1ledPin = 2;
int gas2ledPin = 4;
int gas3ledPin = 5;

// LDR
const int ldrPin = 34;
const int LDRledPin = 18;
int threshold = 3500;

DHT dht(DHTPIN, DHTTYPE);

/* ------------------- GPS SETUP ------------------- */
TinyGPSPlus gps;
HardwareSerial GPSSerial(2);
const uint32_t GPS_BAUD = 9600;
unsigned long lastGPSprint = 0;
const unsigned long GPS_PRINT_INTERVAL = 2000;

String gpsIsoTimestamp() {
  if (!gps.date.isValid() || !gps.time.isValid()) return "";
  char buf[32];
  sprintf(buf, "%04u-%02u-%02uT%02u:%02u:%02uZ",
          gps.date.year(), gps.date.month(), gps.date.day(),
          gps.time.hour(), gps.time.minute(), gps.time.second());
  return String(buf);
}

void setupGPS() {
  GPSSerial.begin(GPS_BAUD, SERIAL_8N1, 16, 17);
  Serial.println("GPS started on RX=16 TX=17");
}

void updateGPS() {
  while (GPSSerial.available()) {
    gps.encode(GPSSerial.read());
  }

  unsigned long now = millis();
  if (now - lastGPSprint >= GPS_PRINT_INTERVAL) {
    lastGPSprint = now;
    printGPS();
  }
}

void printGPS() {
  Serial.println("------ GPS ------");

  if (gps.location.isValid()) {
    Serial.print("Lat: "); Serial.println(gps.location.lat(), 6);
    Serial.print("Lng: "); Serial.println(gps.location.lng(), 6);
  } else {
    Serial.println("No GPS fix yet...");
  }
}

/* ------------------- SETUP ------------------- */
void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); delay(300);
  }

  Serial.println();
  Serial.print("connect with IP: ");
  Serial.println(WiFi.localIP());

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("SignUp ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  dht.begin();

  pinMode(Temp1ledPin, OUTPUT);
  pinMode(Temp2ledPin, OUTPUT);
  pinMode(Temp3ledPin, OUTPUT);

  pinMode(gas1ledPin, OUTPUT);
  pinMode(gas2ledPin, OUTPUT);
  pinMode(gas3ledPin, OUTPUT);

  pinMode(LDRledPin, OUTPUT);

  setupGPS();
}

/* ========================= LOOP ========================= */
void loop() {

  updateGPS();
  Humidity_Temperature();
  gas_Sensor();
  LDR_Sensor();

  if (Firebase.ready() && signupOK &&
      (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {

    sendDataPrevMillis = millis();

    // HUMIDITY
    Firebase.RTDB.setFloat(&fbdo, "Sensor/Humidity", humidity);

    // TEMPERATURE
    Firebase.RTDB.setFloat(&fbdo, "Sensor/Temperature", temperature);

    //  GAS
    Firebase.RTDB.setInt(&fbdo, "Sensor/Gas", sensorValue);

    //  GPS UPLOAD
    if (gps.location.isValid()) {
      Firebase.RTDB.setFloat(&fbdo, "Sensor/GPS/Latitude", gps.location.lat());
      Firebase.RTDB.setFloat(&fbdo, "Sensor/GPS/Longitude", gps.location.lng());
      Firebase.RTDB.setString(&fbdo, "Sensor/GPS/Time", gpsIsoTimestamp());
      Serial.println("GPS Sent!");
    } else {
      Serial.println("GPS Not Valid");
    }
  }

  delay(2000);
}

/* ---------------- TEMPERATURE & HUMIDITY ---------------- */
void Humidity_Temperature() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT22!");
    return;
  }

  digitalWrite(Temp1ledPin, LOW);
  digitalWrite(Temp2ledPin, LOW);
  digitalWrite(Temp3ledPin, LOW);

  if (temperature <= 25) {
    digitalWrite(Temp1ledPin, HIGH);
  }
  else if (temperature <= 35) {
    digitalWrite(Temp1ledPin, HIGH);
    digitalWrite(Temp2ledPin, HIGH);
  }
  else {
    digitalWrite(Temp1ledPin, HIGH);
    digitalWrite(Temp2ledPin, HIGH);
    digitalWrite(Temp3ledPin, HIGH);
  }
}

/* ---------------- GAS SENSOR ---------------- */
void gas_Sensor() {

  sensorValue = analogRead(MQ2_PIN);

  digitalWrite(gas1ledPin, LOW);
  digitalWrite(gas2ledPin, LOW);
  digitalWrite(gas3ledPin, LOW);

  if (sensorValue < 500) {
    digitalWrite(gas1ledPin, HIGH);
  }
  else if (sensorValue < 1000) {
    digitalWrite(gas1ledPin, HIGH);
    digitalWrite(gas2ledPin, HIGH);
  }
  else {
    digitalWrite(gas1ledPin, HIGH);
    digitalWrite(gas2ledPin, HIGH);
    digitalWrite(gas3ledPin, HIGH);
  }
}

/* ---------------- LDR SENSOR ---------------- */
void LDR_Sensor() {

  ldrValue = analogRead(ldrPin);  

  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  digitalWrite(LDRledPin, ldrValue < threshold ? LOW : HIGH);
}
