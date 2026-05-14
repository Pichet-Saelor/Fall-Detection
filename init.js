const fs = require('fs');

// รหัส Node ID ของ MQTT Broker ใน flows.json
const brokerNodeId = "544766541f685886";

// สร้างข้อมูลรหัสผ่าน
const credentials = {
    [brokerNodeId]: {
        "user": process.env.MQTT_USER || "iotproject",
        "password": process.env.MQTT_PASS || "Admin123"
    }
};

// เขียนลงไฟล์ flows_cred.json
try {
    fs.writeFileSync('flows_cred.json', JSON.stringify(credentials));
    console.log('✅ Successfully generated flows_cred.json');
} catch (err) {
    console.error('❌ Failed to generate flows_cred.json:', err);
}