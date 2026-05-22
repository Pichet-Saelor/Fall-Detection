# ระบบตรวจจับการล้ม (Fall Detection System)

ระบบนี้ใช้ ESP32 และเซนเซอร์ MPU6050 เพื่อตรวจจับการล้ม และส่งข้อมูลแจ้งเตือนผ่าน MQTT ไปยัง Node-RED Dashboard เพื่อแสดงผลและแจ้งเตือนแบบ Real-time

## ส่วนประกอบของระบบ

1.  **Hardware:** ESP32, MPU6050 (Accelerometer & Gyroscope)
2.  **Cloud:** HiveMQ Cloud (MQTT Broker)
3.  **Software:** Node-RED (Dashboard & Logic)

---

## การติดตั้งและตั้งค่า

### 1. ส่วนของ Hardware (ESP32 + MPU6050)

#### การต่อสาย (Wiring):
- **MPU6050 VCC** -> ESP32 3.3V
- **MPU6050 GND** -> ESP32 GND
- **MPU6050 SCL** -> ESP32 SCL (GPIO 22)
- **MPU6050 SDA** -> ESP32 SDA (GPIO 21)

#### การเตรียม Arduino IDE:
1.  ติดตั้ง Library ต่อไปนี้ผ่าน Library Manager:
    - `Adafruit MPU6050`
    - `Adafruit Unified Sensor`
    - `PubSubClient` (โดย Nick O'Leary)
    - `ArduinoJson`
2.  เปิดไฟล์ `iot_project/Iot_project.ino`
3.  แก้ไขข้อมูล WiFi และ MQTT ใน Code:
    ```cpp
    const char* WIFI_SSID     = "ชื่อ_WiFi_ของคุณ";
    const char* WIFI_PASSWORD = "รหัสผ่าน_WiFi_ของคุณ";

    const char* MQTT_USER     = "ชื่อผู้ใช้_MQTT";
    const char* MQTT_PASS     = "รหัสผ่าน_MQTT";
    ```
4.  เลือกบอร์ดเป็น **ESP32 Dev Module** และทำการ Upload code

### 2. ส่วนของ Software (Node-RED)

ระบบนี้ออกแบบมาให้รันบน Node.js หรือ deploy บน Render/Heroku ได้

#### ขั้นตอนการรันบนเครื่อง Local:
1.  ติดตั้ง [Node.js](https://nodejs.org/)
2.  เปิด Terminal/Command Prompt ในโฟลเดอร์โปรเจกต์
3.  ติดตั้ง dependencies:
    ```bash
    npm install
    ```
4.  (ทางเลือก) ตั้งค่า Environment Variables สำหรับ MQTT (ถ้าไม่ตั้งจะใช้ค่า Default ใน `init.js`):
    - `MQTT_USER`
    - `MQTT_PASS`
5.  รันโปรเจกต์:
    ```bash
    npm start
    ```
6.  เข้าใช้งาน Dashboard ได้ที่: `http://localhost:1880/ui`

---

## การใช้งาน

1.  เมื่อเปิดเครื่อง ESP32 จะทำการเชื่อมต่อกับ WiFi และ MQTT Broker (HiveMQ)
2.  เซนเซอร์ MPU6050 จะส่งข้อมูลแรงกระแทก (G-Force) ไปยัง Node-RED อย่างต่อเนื่อง
3.  **เมื่อเกิดการล้ม:**
    - หากตรวจพบแรงกระแทกเกินค่าที่กำหนด (`FALL_THRESHOLD`)
    - และหากไม่มีการเคลื่อนไหวหลังจากนั้น (`STILL_THRESHOLD`)
    - ระบบจะส่งสถานะ "Fall Detected!" ไปยัง Topic `fall/alert`
4.  Node-RED Dashboard จะแสดงสถานะการแจ้งเตือน พร้อมประวัติการตรวจจับ

## โครงสร้างไฟล์
- `iot_project/`: โค้ดสำหรับบอร์ด ESP32
- `flows.json`: โค้ด Logic ของ Node-RED
- `settings.js`: การตั้งค่า Node-RED
- `init.js`: สคริปต์สำหรับสร้างไฟล์ Credentials อัตโนมัติ
- `public/`: ไฟล์เว็บส่วนหน้า (Static Assets)
- `fall_detection_dashboard.html`: แดชบอร์ดแบบ Standalone (สามารถเปิดใช้งานแยกได้โดยไม่ต้องรัน Node-RED)
