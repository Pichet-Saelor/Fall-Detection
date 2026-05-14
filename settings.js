module.exports = {
    flowFile: 'flows.json',
    userDir: __dirname,
    httpStatic: __dirname + '/public',
    functionGlobalContext: {
        // สามารถดึงค่าไปใช้ใน Function Node ได้
    },
    // ตั้งค่ารหัสผ่าน MQTT ผ่าน Environment Variables
    mqttConnectOptions: {
        user: process.env.MQTT_USER || "iotproject",
        password: process.env.MQTT_PASS || "Admin123"
    },
    exportGlobalContextKeys: false,
    editorTheme: {
        projects: {
            enabled: false
        }
    }
}