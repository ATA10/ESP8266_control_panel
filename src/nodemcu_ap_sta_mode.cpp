// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
// #include <EEPROM.h>

// #define EEPROM_SIZE 96
// #define RESET_BUTTON D1  // Reset butonu için D1 pini (GPIO5)

// ESP8266WebServer server(80);
// String wifi_ssid = "";
// String wifi_password = "";

// // AP modu bilgileri
// const char *ap_ssid = "NodeMCU_AP";
// const char *ap_password = "12345678";

// // EEPROM'dan SSID ve Şifreyi Okuma
// void readWiFiCredentials() {
//     Serial.println("🔍 EEPROM'dan Wi-Fi Bilgileri Okunuyor...");
    
//     EEPROM.begin(EEPROM_SIZE);

//     char stored_ssid[32] = {0};      // SSID için buffer
//     char stored_password[32] = {0};  // Şifre için buffer

//     EEPROM.get(0, stored_ssid);
//     EEPROM.get(32, stored_password);

//     wifi_ssid = String(stored_ssid);        
//     wifi_password = String(stored_password);

//     Serial.println("📶 EEPROM'dan Okunan SSID: '" + wifi_ssid + "'");
//     Serial.println("🔑 EEPROM'dan Okunan Şifre: '" + wifi_password + "'");

//     EEPROM.end();

//     // Eğer SSID boş ise, verilerin geçersiz olduğunu belirt
//     if (wifi_ssid.length() == 0 || wifi_ssid == "0" || wifi_ssid == " " || wifi_ssid[0] == 255) {
//         Serial.println("⚠ EEPROM'da geçerli bir SSID bulunamadı!");
//         wifi_ssid = "";
//         wifi_password = "";
//     }
// }



// // Wi-Fi SSID ve Şifreyi EEPROM’a Kaydetme
// void saveWiFiCredentials(String ssid, String password) {
//     Serial.println("💾 Wi-Fi Bilgileri EEPROM'a Kaydediliyor...");
    
//     EEPROM.begin(EEPROM_SIZE);
    
//     // SSID ve Şifreyi char dizisine çevirip, EEPROM'a yazıyoruz
//     char ssidArray[32] = {0};
//     char passArray[32] = {0};

//     ssid.toCharArray(ssidArray, 32);
//     password.toCharArray(passArray, 32);

//     EEPROM.put(0, ssidArray);
//     EEPROM.put(32, passArray);
    
//     EEPROM.commit(); // EEPROM’a verileri kalıcı olarak yaz
    
//     Serial.println("✅ EEPROM'a yazma tamamlandı!");
//     EEPROM.end();
// }


// // HTML Formu
// const char htmlPage[] PROGMEM = R"rawliteral(
// <!DOCTYPE html>
// <html>
// <head>
//     <meta name="viewport" content="width=device-width, initial-scale=1">
//     <style>
//         body { font-family: Arial, sans-serif; text-align: center; padding: 20px; }
//         form { max-width: 300px; margin: auto; padding: 20px; background: #f4f4f4; border-radius: 10px; }
//         input { width: 100%; padding: 10px; margin: 10px 0; border-radius: 5px; border: 1px solid #ccc; }
//         button { background: #28a745; color: white; border: none; padding: 10px; width: 100%; border-radius: 5px; }
//     </style>
// </head>
// <body>
//     <h2>Wi-Fi Bağlantı Formu</h2>
//     <form action="/submit" method="GET">
//         <label for="name">Wi-Fi Adı (SSID):</label>
//         <input type="text" id="name" name="name" required>
        
//         <label for="password">Wi-Fi Şifre:</label>
//         <input type="text" id="password" name="password" required>
        
//         <button type="submit">Gönder</button>
//     </form>
// </body>
// </html>
// )rawliteral";

// // Wi-Fi Bilgilerini Alıp EEPROM’a Kaydetme ve Bağlanma
// void handleForm() {
//     wifi_ssid = server.arg("name");
//     wifi_password = server.arg("password");

//     Serial.println("Yeni Wi-Fi Bilgileri Alındı:");
//     Serial.println("SSID: " + wifi_ssid);
//     Serial.println("Şifre: " + wifi_password);

//     saveWiFiCredentials(wifi_ssid, wifi_password);  // EEPROM'a kaydet

//     String response = "<h2>Bilgiler Alındı!</h2>";
//     response += "<p><b>SSID:</b> " + wifi_ssid + "</p>";
//     response += "<p><b>Şifre:</b> " + wifi_password + "</p>";
//     response += "<button onclick=\"location.href='/connect'\">TAMAM</button>";

//     server.send(200, "text/html", response);
// }

// // STA Moduna Geçip Wi-Fi'ye Bağlanma
// void connectToWiFi() {
//     Serial.println("STA Moduna Geçiliyor...");
    
//     WiFi.softAPdisconnect(true);
//     WiFi.mode(WIFI_STA);
//     WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

//     int timeout = 0;
//     while (WiFi.status() != WL_CONNECTED && timeout < 20) {
//         delay(1000);
//         Serial.print(".");
//         timeout++;
//     }

//     if (WiFi.status() == WL_CONNECTED) {
//         Serial.println("\nWi-Fi Bağlantısı Başarılı!");
//         Serial.print("IP Adresi: ");
//         Serial.println(WiFi.localIP());

//         server.send(200, "text/html", "<h2>Bağlantı Başarılı!</h2><p>Bağlı IP: " + WiFi.localIP().toString() + "</p>");
//     } else {
//         Serial.println("\nBağlantı Başarısız!");
//         server.send(200, "text/html", "<h2>Bağlantı Başarısız!</h2><a href='/'>Tekrar Dene</a>");
//     }
// }

// // Reset Butonuna Basılınca AP Modunda Başlatma
// void checkResetButton() {
//     pinMode(RESET_BUTTON, INPUT_PULLUP);
//     if (digitalRead(RESET_BUTTON) == LOW) {
//         Serial.println("Reset Butonuna Basıldı! AP Modunda Başlatılıyor...");
//         WiFi.disconnect();
//         WiFi.mode(WIFI_AP);
//         WiFi.softAP(ap_ssid, ap_password);
//     }
// }

// // **Setup Fonksiyonu**
// void setup() {
//     Serial.begin(115200);
//     readWiFiCredentials();  // EEPROM’dan SSID & Şifre oku

//     checkResetButton();  // Reset butonu basıldı mı kontrol et

//     if (wifi_ssid.length() > 0) {
//         WiFi.mode(WIFI_STA);
//         WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

//         Serial.println("Wi-Fi'ye bağlanılıyor...");
//         int timeout = 0;
//         while (WiFi.status() != WL_CONNECTED && timeout < 20) {
//             delay(1000);
//             Serial.print(".");
//             timeout++;
//         }

//         if (WiFi.status() == WL_CONNECTED) {
//             Serial.println("\nWi-Fi Bağlantısı Başarılı!");
//             Serial.print("IP Adresi: ");
//             Serial.println(WiFi.localIP());
//         } else {
//             Serial.println("\nBağlantı Başarısız! AP Moduna Geçiliyor...");
//             WiFi.mode(WIFI_AP);
//             WiFi.softAP(ap_ssid, ap_password);
//             Serial.print("IP Adresi: ");
//             Serial.println(WiFi.softAPIP());
//         }
//     } else {
//         WiFi.mode(WIFI_AP);
//         WiFi.softAP(ap_ssid, ap_password);
//         Serial.print("IP Adresi: ");
//         Serial.println(WiFi.softAPIP());
//     }

//     server.on("/", []() { server.send(200, "text/html", htmlPage); });
//     server.on("/submit", handleForm);
//     server.on("/connect", connectToWiFi);

//     server.begin();
//     Serial.println("Web Sunucu Çalışıyor...");
// }

// // **Loop Fonksiyonu**
// void loop() {
//     server.handleClient();
//     checkResetButton();  // Reset butonu dinlemeye devam et
// }