# Smart Helmet Monitoring System

## 📌 Overview
The Smart Helmet Monitoring System is an IoT-based embedded system designed to enhance rider safety by monitoring environmental conditions and real-time location data.

The system ensures that safety parameters are met before vehicle ignition and continuously transmits monitoring data to a remote dashboard.

---

## 🎯 Objectives
- Monitor environmental conditions (Temperature & Humidity)
- Detect hazardous gas levels (MQ2 Sensor)
- Track real-time GPS location
- Provide IoT-based remote monitoring
- Improve rider safety through real-time alerts

---

##  System Architecture

### Hardware Components
- ESP32 Microcontroller
- Gas Sensor (MQ series)
- GPS Module
- Vibration/Helmet detection sensor
- Power Supply Module

### Communication
- WiFi-based IoT communication
- Real-time data transmission
- Cloud dashboard monitoring

---

## ⚙️ Features

- 🌡 Real-time Temperature Monitoring
- 💧 Humidity Detection
- 🧪 Gas Level Monitoring
- 📍 GPS Location Tracking
- ☁ Cloud Data Transmission via Firebase
- 📊 Real-time Web Dashboard (Next.js)
- 🚨 LED-based Safety Indication System

---

##  Working Principle
1. Sensors continuously collect environmental and safety data.
2. ESP32 processes and evaluates sensor readings.
3. LED indicators display local safety status.
4. Sensor data is uploaded to Firebase every 5 seconds.
5. GPS coordinates are transmitted if a valid signal is available.
6. The Next.js dashboard displays real-time system status.
7. 
---

##  Cloud Integration

The system uses:
- Firebase Realtime Database
- Firebase Authentication
- WiFi communication via ESP32

---

## Web Dashboard (Next.js)

The web application:
- Listens to Firebase in real time
- Displays live sensor data
- Shows GPS coordinates
- Provides visual safety indicators

---

## 🧑‍💻 Author
**Amila Sampath**  
Embedded Systems & IoT Developer  
Wayamba University of Sri Lanka  

GitHub: https://github.com/amilasampath0130  
LinkedIn: https://linkedin.com/in/amilasampath0130  
