#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ===== ตั้งค่าตรงนี้ =====
const char* WIFI_SSID     = "zeitlram_2.4G";
const char* WIFI_PASSWORD = "Zeitlram2547";

// ข้อมูลจาก HiveMQ Cloud
const char* MQTT_SERVER   = "6e33277df1aa4e1793971087c3026307.s1.eu.hivemq.cloud"; 
const int   MQTT_PORT     = 8883;
const char* MQTT_USER     = "iotproject";
const char* MQTT_PASS     = "Admin123";

// Topic
const char* TOPIC_SENSOR  = "fall_detection/data"; 
const char* TOPIC_ALERT   = "fall/alert";
// =========================

#include <WiFiClientSecure.h>
Adafruit_MPU6050 mpu;
WiFiClientSecure wifiClient;
PubSubClient mqtt(wifiClient);

void connectMQTT() {
  wifiClient.setInsecure(); // สำหรับ HiveMQ Cloud แบบเริ่มต้น
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32_Fall-";
    clientId += String(random(0xffff), HEX);
    
    if (mqtt.connect(clientId.c_str(), MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) delay(10);
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("MPU6050 Initialized!");

  connectWiFi();
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
  if (!mqtt.connected()) {
    connectMQTT();
  }
  mqtt.loop();

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  float az = a.acceleration.z;
  
  // คำนวณแรงลัพธ์ (Resultant Force)
  float gForce = sqrt(ax*ax + ay*ay + az*az) / 9.80665;

  // --- Logic ตรวจจับการล้ม ---
  if (gForce > FALL_THRESHOLD) {
    highGSeen = true;
    stillCount = 0;
    Serial.println("⚠ High G detected: " + String(gForce));
  } 
  
  if (highGSeen) {
    if (gForce < STILL_THRESHOLD) {
      stillCount++;
      if (stillCount >= STILL_COUNT_MAX) {
        // แจ้งเตือนเมื่อล้มแล้วนิ่ง
        mqtt.publish(TOPIC_ALERT, "{\"fall\":true}");
        Serial.println("🚨 FALL CONFIRMED!");
        highGSeen = false;
        stillCount = 0;
      }
    } else if (gForce > 1.5) { // ถ้ากลับมาเคลื่อนไหวแรงๆ ให้ยกเลิกการตรวจจับนิ่ง
       // stillCount = 0; 
    }
  }

  // --- ส่งค่าไป Node-RED Dashboard ---
  StaticJsonDocument<200> doc;
  
  doc["x"] = ax;
  doc["y"] = ay;
  doc["z"] = az;
  doc["g"]  = gForce;

  char buf[256];
  serializeJson(doc, buf);
  mqtt.publish(TOPIC_SENSOR, buf);

  delay(100); 
}