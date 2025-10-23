// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <LiquidCrystal.h>

// // Thông tin WiFi
// #define WIFI_SSID "NHISM_3104"
// #define WIFI_PASSWORD "cocaicoincard"

// // ================= FIREBASE =================
// const String DATABASE_URL = "https://parking-e55c4-default-rtdb.asia-southeast1.firebasedatabase.app/";


// // ============ LCD PINS (4-bit mode) ============
// LiquidCrystal lcd(26, 27, 14, 12, 13, 15);

// // ============ IR SENSOR PINS ============
// #define IR1 32
// #define IR2 33
// #define IR3 25
// #define IR4 4

// void setup() {
//   Serial.begin(115200);
//   delay(700); 
//   lcd.begin(16, 2);   // Khởi tạo LCD 16x2

//   lcd.command(0x33);
//   lcd.command(0x32);
//   lcd.command(0x28);
//   lcd.command(0x0C);
//   lcd.command(0x06);

//   lcd.clear();
//   lcd.print("ESP32 Starting");
//   delay(1000);

//   // Khai báo input
//   pinMode(IR1, INPUT);
//   pinMode(IR2, INPUT);
//   pinMode(IR3, INPUT);
//   pinMode(IR4, INPUT);

//   // Kết nối WiFi
//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//   Serial.print("Connecting to WiFi");
//   lcd.clear();
//   lcd.print("WiFi Connecting");

//   int retry = 0;
//   while (WiFi.status() != WL_CONNECTED && retry < 30) {
//     delay(500);
//     Serial.print(".");
//     retry++;
//   }

//   lcd.clear();
//   if (WiFi.status() == WL_CONNECTED) {
//     lcd.print("WiFi Connected");
//     Serial.println("\n✅ WiFi Connected!");
//   } else {
//     lcd.print("WiFi Failed");
//     Serial.println("\n❌ WiFi Connection Failed!");
//   }

//   delay(1000);
//   lcd.clear();
// }

// void loop() {
//   // Đọc cảm biến
//   int s1 = digitalRead(IR1);
//   int s2 = digitalRead(IR2);
//   int s3 = digitalRead(IR3);
//   int s4 = digitalRead(IR4);

//   // Debug ra Serial
//   Serial.printf("S1=%d | S2=%d | S3=%d | S4=%d\n", s1, s2, s3, s4);

//   // Hiển thị LCD
//   lcd.setCursor(0, 0);
//   lcd.print("S1:");
//   lcd.print(s1 == 0 ? "FILL " : "EMPTY");
//   lcd.setCursor(8, 0);
//   lcd.print("S2:");
//   lcd.print(s2 == 0 ? "FILL " : "EMPTY");

//   lcd.setCursor(0, 1);
//   lcd.print("S3:");
//   lcd.print(s3 == 0 ? "FILL " : "EMPTY");
//   lcd.setCursor(8, 1);
//   lcd.print("S4:");
//   lcd.print(s4 == 0 ? "FILL " : "EMPTY");

//   // Gửi dữ liệu lên Firebase
//   if (WiFi.status() == WL_CONNECTED) {
//     HTTPClient http;
//     String url = DATABASE_URL + "/sensors.json";

//     String jsonData = "{";
//     jsonData += "\"sensor1\":" + String(!s1) + ",";
//     jsonData += "\"sensor2\":" + String(!s2) + ",";
//     jsonData += "\"sensor3\":" + String(!s3) + ",";
//     jsonData += "\"sensor4\":" + String(!s4);
//     jsonData += "}";

//     http.begin(url);
//     http.addHeader("Content-Type", "application/json");
//     int httpResponseCode = http.PATCH(jsonData);

//     if (httpResponseCode > 0) {
//       Serial.printf("✅ Firebase update OK (%d)\n", httpResponseCode);
//     } else {
//       Serial.printf("❌ Firebase Error: %s\n", http.errorToString(httpResponseCode).c_str());
//     }
//     http.end(); // đóng kết nối để tránh treo
//   } else {
//     Serial.println("⚠️ WiFi Disconnected, skipping send");
//   }

//   delay(2000); // Gửi mỗi 2 giây
// }


#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal.h>

// ================= WIFI =================
#define WIFI_SSID "NHISM_3104"
#define WIFI_PASSWORD "cocaicoincard"

// ================= FIREBASE =================
const String DATABASE_URL = "https://parking-e55c4-default-rtdb.asia-southeast1.firebasedatabase.app/";

// ================= LCD (RS, EN, D4, D5, D6, D7) =================
LiquidCrystal lcd(26, 27, 14, 12, 13, 15);

// ================= IR SENSOR PINS =================
#define IR1 32
#define IR2 33
#define IR3 25
#define IR4 4

// ================= UART PINS (Gửi sang PIC) =================
#define RX2_PIN 16  // ESP32 RX2
#define TX2_PIN 17  // ESP32 TX2
HardwareSerial SerialPIC(2);

// ================= STATE TRACKING =================
int lastS1 = -1, lastS2 = -1, lastS3 = -1, lastS4 = -1;
unsigned long lastSendTime = 0;
const unsigned long MIN_INTERVAL = 500; // ms

void setup() {
  Serial.begin(115200);
  SerialPIC.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("ESP32 Starting");
  delay(1000);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);

  // ----- Kết nối WiFi -----
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  lcd.clear();
  lcd.print("WiFi Connecting");

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 30) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  lcd.clear();
  if (WiFi.status() == WL_CONNECTED) {
    lcd.print("WiFi Connected");
    Serial.println("\n✅ WiFi Connected!");
  } else {
    lcd.print("WiFi Failed");
    Serial.println("\n❌ WiFi Connection Failed!");
  }

  delay(1000);
  lcd.clear();
}

void loop() {
  // ----- Đọc cảm biến -----
  int s1 = digitalRead(IR1);
  int s2 = digitalRead(IR2);
  int s3 = digitalRead(IR3);
  int s4 = digitalRead(IR4);

  // ----- Hiển thị LCD -----
  lcd.setCursor(0, 0);
  lcd.print("S1:");
  lcd.print(s1 == 0 ? "FILL " : "EMPTY");
  lcd.setCursor(8, 0);
  lcd.print("S2:");
  lcd.print(s2 == 0 ? "FILL " : "EMPTY");

  lcd.setCursor(0, 1);
  lcd.print("S3:");
  lcd.print(s3 == 0 ? "FILL " : "EMPTY");
  lcd.setCursor(8, 1);
  lcd.print("S4:");
  lcd.print(s4 == 0 ? "FILL " : "EMPTY");

  // ----- Kiểm tra thay đổi -----
  bool changed = (s1 != lastS1) || (s2 != lastS2) || (s3 != lastS3) || (s4 != lastS4);

  if (changed && millis() - lastSendTime > MIN_INTERVAL) {
    Serial.printf("S1=%d | S2=%d | S3=%d | S4=%d\n", !s1, !s2, !s3, !s4);

    // ====== Gửi qua UART cho PIC ======
    String uartData = String(!s1) + "," + String(!s2) + "," + String(!s3) + "," + String(!s4) + "\n";
    SerialPIC.print(uartData);
    Serial.print("UART Sent to PIC -> ");
    Serial.println(uartData);

    // ====== Gửi lên Firebase ======
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = DATABASE_URL + "/sensors.json";

      String jsonData = "{";
      jsonData += "\"sensor1\":" + String(!s1) + ",";
      jsonData += "\"sensor2\":" + String(!s2) + ",";
      jsonData += "\"sensor3\":" + String(!s3) + ",";
      jsonData += "\"sensor4\":" + String(!s4);
      jsonData += "}";

      http.begin(url);
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.PATCH(jsonData);

      if (httpResponseCode > 0) {
        Serial.printf("✅ Firebase update OK (%d)\n", httpResponseCode);
      } else {
        Serial.printf("❌ Firebase Error: %s\n", http.errorToString(httpResponseCode).c_str());
      }
      http.end();
    } else {
      Serial.println("⚠️ WiFi Disconnected, skipping Firebase");
    }

    // ----- Lưu lại trạng thái -----
    lastS1 = s1;
    lastS2 = s2;
    lastS3 = s3;
    lastS4 = s4;
    lastSendTime = millis();
  }

  delay(100); // đọc cảm biến mỗi 100ms
}
