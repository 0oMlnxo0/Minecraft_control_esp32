  #include <WiFi.h>
  #include <WebServer.h>
  #include <Arduino.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <Adafruit_Sensor.h>
  #include <DHT.h>
  #include <DHT_U.h>
  #include <time.h>

  const char* ssid = "Home747_2G";
  const char* password = "024769451";

  WebServer server(80); // HTTP server

  // GPIO ที่ใช้ควบคุม 
  #define BUTTONPIN1 4 //เลื่อน
  #define BUTTONPIN2 16//Mode+
  #define BUTTONPIN3 17//Mode-
  #define RELAYPIN1 25  
  #define RELAYPIN2 26
  #define RELAYPIN3 32
  #define RELAYPIN4 33
  #define DHTPIN 27

  #define SCREEN_WIDTH 128 // OLED display width, in pixels
  #define SCREEN_HEIGHT 64 // OLED display height, in pixels
  #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
  #define SCREEN_ADDRESS 0x3C
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  #define DHTTYPE    DHT22
  DHT_Unified dht(DHTPIN, DHTTYPE);
  uint32_t delayMS;

  int buttonState1 = 0;
  int buttonState2 = 0;
  int buttonState3 = 0;
  int selectedRelay = 1;
  int state = 1;
  int y = 5;  

  //================================================================================================
  // คำสั่งสำหรับ LED1
  void handleLED1On() {
    digitalWrite(RELAYPIN1, LOW);
    Serial.println("Relay1:ON");
    server.send(200, "text/plain", "LED1 ON");
  };

  void handleLED1Off() {
    digitalWrite(RELAYPIN1, HIGH);
    Serial.println("Relay1:OFF");
    server.send(200, "text/plain", "LED1 OFF");
  };

  // คำสั่งสำหรับ LED2
  void handleLED2On() {
    digitalWrite(RELAYPIN2, LOW);
    Serial.println("Relay2:ON");
    server.send(200, "text/plain", "LED2 ON");
  };

  void handleLED2Off() {
    digitalWrite(RELAYPIN2, HIGH);
    Serial.println("Relay2:OFF");
    server.send(200, "text/plain", "LED2 OFF");
  };

  // คำสั่งสำหรับ LED3
  void handleLED3On() {
    digitalWrite(RELAYPIN3, LOW);
    Serial.println("Relay3:ON");
    server.send(200, "text/plain", "LED3 ON");
  };

  void handleLED3Off() {
    digitalWrite(RELAYPIN3, HIGH);
    Serial.println("Relay3:OFF");
    server.send(200, "text/plain", "LED3 OFF");
  };

  // คำสั่งสำหรับ LED4
  void handleLED4On() {
    digitalWrite(RELAYPIN4, LOW);
    Serial.println("Relay4:ON");
    server.send(200, "text/plain", "LED4 ON");
  };

  void handleLED4Off() {
    digitalWrite(RELAYPIN4, HIGH);
    Serial.println("Relay4:OFF");
    server.send(200, "text/plain", "LED4 OFF");
  };

  // คำสั่งสำหรับ Button1
  void Button1On() {
    selectedRelay += 1;
    if (selectedRelay > 4)
    selectedRelay = 1;
    delay(500);
    Serial.println("Button1:ON");
    server.send(200, "text/plain", "Button1 ON");
  };

  void Button1Off() {
    selectedRelay += 0;
    delay(500);
    Serial.println("Button1:OFF");
    server.send(200, "text/plain", "Button1 OFF");
  };

  // คำสั่งสำหรับ Button2
  void Button2On() {
  switch(state){
    case 1: 
      switch (selectedRelay) {
        case 1: digitalWrite(RELAYPIN1, LOW); break;
        case 2: digitalWrite(RELAYPIN2, LOW); break;
        case 3: digitalWrite(RELAYPIN3, LOW); break;
        case 4: digitalWrite(RELAYPIN4, LOW); break;
      }
      state += 1;
    break;
    case 2:
      switch (selectedRelay) {
        case 1: digitalWrite(RELAYPIN1, HIGH); break;
        case 2: digitalWrite(RELAYPIN2, HIGH); break;
        case 3: digitalWrite(RELAYPIN3, HIGH); break;
        case 4: digitalWrite(RELAYPIN4, HIGH); break;
      }
    state -= 1;
  break;
  };
    delay(500);
    Serial.println("Button2:ON");
    server.send(200, "text/plain", "Button2 ON");
  };

  void Button2Off() {
    state += 0;
    delay(500);
    Serial.println("Button2:OFF");
    server.send(200, "text/plain", "Button2 OFF");
  };

  // คำสั่งสำหรับ Button3
  void Button3On() {
    digitalWrite(RELAYPIN1, HIGH);
    digitalWrite(RELAYPIN2, HIGH);
    digitalWrite(RELAYPIN3, HIGH);
    digitalWrite(RELAYPIN4, HIGH);
    delay(500);
    Serial.println("Button3:ON");
    server.send(200, "text/plain", "Button3 ON");
  };

  void Button3Off() {
    Serial.println("Stop");
    delay(500);
    Serial.println("Button3:OFF");
    server.send(200, "text/plain", "Button3 OFF");
  };

  //================================================================================================

  void setup() {
    Serial.begin(115200);
    dht.begin();
    sensor_t sensor;  
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  delay(1000); // Pause for 1 seconds
    pinMode(BUTTONPIN1, INPUT_PULLUP); //ไม่กดปุ่มค่าเป็น1กดเป็น0
    pinMode(BUTTONPIN2, INPUT_PULLUP);
    pinMode(BUTTONPIN3, INPUT_PULLUP);

    pinMode(RELAYPIN1, OUTPUT);
    digitalWrite(RELAYPIN1, HIGH);
    pinMode(RELAYPIN2, OUTPUT);
    digitalWrite(RELAYPIN2, HIGH);
    pinMode(RELAYPIN3, OUTPUT);
    digitalWrite(RELAYPIN3, HIGH);
    pinMode(RELAYPIN4, OUTPUT);
    digitalWrite(RELAYPIN4, HIGH);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\n✅ WiFi connected. IP Address:");
    Serial.println(WiFi.localIP());

    // ตั้ง route สำหรับแต่ละคำสั่ง
    server.on("/led1/on", handleLED1On);
    server.on("/led1/off", handleLED1Off);

    server.on("/led2/on", handleLED2On);
    server.on("/led2/off", handleLED2Off);

    server.on("/led3/on", handleLED3On);
    server.on("/led3/off", handleLED3Off);

    server.on("/led4/on", handleLED4On);
    server.on("/led4/off", handleLED4Off);

    server.on("/button1/on", Button1On);  
    server.on("/button1/off", Button1Off);

    server.on("/button2/on", Button2On);  
    server.on("/button2/off", Button2Off);

    server.on("/button3/on", Button3On);  
    server.on("/button3/off", Button3Off);

    server.begin();
    
    // ตั้งค่าเวลา NTP
    configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov"); // GMT+7 สำหรับไทย

    // รอจนเวลาพร้อม
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
      Serial.println("⏳ Waiting for NTP time...");
      delay(1000);
    }
    Serial.println("✅ Time synchronized");
    Serial.println("🚀 HTTP server started");
  };

  void loop() {
    server.handleClient();
    sensors_event_t event;
    display.clearDisplay();

    buttonState1 = digitalRead(BUTTONPIN1);
    buttonState2 = digitalRead(BUTTONPIN2);
    buttonState3 = digitalRead(BUTTONPIN3);
    delay(100);
    
    //รดน้ำทุกวันเวลา6.00น.
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      int hour = timeinfo.tm_hour;
      int minute = timeinfo.tm_min;
      int second = timeinfo.tm_sec;

      // Debug log แสดงเวลา
      Serial.printf("🕒 Time: %02d:%02d:%02d\n", hour, minute, second);

      // ถ้าเวลา 06:00:00 ให้เปิด LED
      if (hour == 6 && minute == 0 && second == 0) {
        dht.humidity().getEvent(&event);
        if(event.relative_humidity < 40)
        {
        digitalWrite(RELAYPIN1, LOW);
        }
        else if(event.relative_humidity >60)
        {
          digitalWrite(RELAYPIN1, HIGH);
        }
        digitalWrite(RELAYPIN1, LOW);
        digitalWrite(RELAYPIN2, LOW);
        digitalWrite(RELAYPIN3, LOW);
        digitalWrite(RELAYPIN4, LOW);
        delay(1000);
        digitalWrite(RELAYPIN1, HIGH);
        digitalWrite(RELAYPIN2, HIGH);
        digitalWrite(RELAYPIN3, HIGH);
        digitalWrite(RELAYPIN4, HIGH);
        Serial.println("💡 LEDs turned ON (6 AM)");
        delay(1000);  // ป้องกันการ trigger ซ้ำใน 1 วินาที
      }
      
  }


  // ปุ่ม 1: เลือกช่องถัดไป
  if (buttonState1 == LOW) { 
  selectedRelay += 1;
  if (selectedRelay > 4)
    selectedRelay = 1;
  delay(300);
}

// ปุ่ม 2: เปิด relay ที่เลือก
if (buttonState2 == LOW) { 
  switch(state){
    case 1: 
      switch (selectedRelay) {
        case 1: digitalWrite(RELAYPIN1, LOW); break;
        case 2: digitalWrite(RELAYPIN2, LOW); break;
        case 3: digitalWrite(RELAYPIN3, LOW); break;
        case 4: digitalWrite(RELAYPIN4, LOW); break;
      }
      state += 1;
      delay(300);
    break;
    case 2:
      switch (selectedRelay) {
        case 1: digitalWrite(RELAYPIN1, HIGH); break;
        case 2: digitalWrite(RELAYPIN2, HIGH); break;
        case 3: digitalWrite(RELAYPIN3, HIGH); break;
        case 4: digitalWrite(RELAYPIN4, HIGH); break;
      }
    state -= 1;
    delay(300);
    break;

    Serial.println(selectedRelay);
  }
}

// ปุ่ม 3: ปิด relay ที่เลือก
if (buttonState3 == LOW) {
  digitalWrite(RELAYPIN1, HIGH);
  digitalWrite(RELAYPIN2, HIGH);
  digitalWrite(RELAYPIN3, HIGH);
  digitalWrite(RELAYPIN4, HIGH);
}
  delay(200);
  };