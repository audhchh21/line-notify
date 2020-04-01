#include <WiFi.h>
#include <WiFiClientSecure.h>

//กำหนดขาที่เชื่อมต่อกับเซ็นเซอร์
#define SENSOR_INPUT 4;

char ssid[] = "Wifi name"; // SSID หรือชื่อ Wifi 
char pass[] = "wifi Password"; // รหัสผ่าน Wifi

// Line token
#define TokenLine "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

// ข้อความ ที่จะแสดงใน Line
String message = "มีผู้บุกรุก!!! ที่อาคาร 5";

void setup() {

    // Set ชนิดของขา เป็น Input
    pinMode(SENSOR_INPUT, INPUT);

    // ตั้งค่าคอนโซล
    Serial.begin(9600);

    // เริ่มการเชื่อมต่อกับวายฟายแม่ข่าย
    WiFi.begin(ssid, pass);

    // แสดง "WiFi Connecting" ในคอนโซล
    Serial.print("WiFi Connecting");

    //ตรวจเช็คสถานะการเขื่อมต่อวายฟาย
    //ถ้าไม่สำเร็จให้แสดง "." ในคอนโซลจนกว่าจะเขื่อมต่อได้
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    //แสดงสถานะการวายฟายเชื่อมต่อแล้ว
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
}

void loop() {

    // อ่านค่าจาก Sensor
    byte value = analogRead(SENSOR_INPUT);

    if (value == 1) {
        Serial.println("Somebody is in this area!");
        NotifyLine(message);
    }
    else if (value == 0) {
        Serial.println("No one!");
    }

    delay(5000);

}

void NotifyLine(String msg) {
    WiFiClientSecure client;
    if (!client.connect("notify-api.line.me", 443)) {
        Serial.println("Connection failed");
        return;
    }
    String req = "";
    req += "POST /api/notify HTTP/1.1\r\n";
    req += "Host: notify-api.line.me\r\n";
    req += "Authorization: Bearer " + String(TokenLine) + "\r\n";
    req += "Cache-Control: no-cache\r\n";
    req += "User-Agent: ESP32\r\n";
    req += "Content-Type: application/x-www-form-urlencoded\r\n";
    req += "Content-Length: " + String(String("message=" + msg).length()) + "\r\n";
    req += "\r\n";
    req += "message=" + msg;
    Serial.println(req);
    client.print(req);
    delay(20);
    Serial.println("-------------");
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
            break;
        }
    } //while
} //loop

