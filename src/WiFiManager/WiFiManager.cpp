#include "WiFiManager.h"

ESP8266WebServer server(80);
String wifi_ssid = "";
String wifi_password = "";

// 📌 EEPROM'a Wi-Fi Bilgilerini Kaydet
void saveWiFiCredentials(String ssid, String password) {
    Serial.println("💾 Wi-Fi Bilgileri EEPROM'a Kaydediliyor...");
    
    EEPROM.begin(EEPROM_SIZE);
    char ssidArray[32] = {0};
    char passArray[32] = {0};

    ssid.toCharArray(ssidArray, 32);
    password.toCharArray(passArray, 32);

    EEPROM.put(0, ssidArray);
    EEPROM.put(32, passArray);
    
    EEPROM.commit();
    Serial.println("✅ EEPROM'a Yazıldı!");
    EEPROM.end();
}

// 📌 EEPROM'dan Wi-Fi Bilgilerini Oku
void readWiFiCredentials() {
    Serial.println("🔍 EEPROM'dan Wi-Fi Bilgileri Okunuyor...");
    
    EEPROM.begin(EEPROM_SIZE);

    char stored_ssid[32] = {0};
    char stored_password[32] = {0};

    EEPROM.get(0, stored_ssid);
    EEPROM.get(32, stored_password);

    wifi_ssid = String(stored_ssid);
    wifi_password = String(stored_password);

    Serial.println("📶 EEPROM SSID: '" + wifi_ssid + "'");
    Serial.println("🔑 EEPROM Şifre: '" + wifi_password + "'");

    EEPROM.end();

    if (wifi_ssid.length() == 0 || wifi_ssid[0] == 255) {
        Serial.println("⚠ EEPROM'da geçerli SSID bulunamadı!");
        wifi_ssid = "";
        wifi_password = "";
    }
}

// 📌 Wi-Fi'ye Bağlan
bool connectToWiFi() {
    Serial.println("🌐 Wi-Fi'ye Bağlanılıyor...");
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n✅ Wi-Fi Bağlantısı Başarılı!");
        Serial.print("📡 IP Adresi: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("\n❌ Bağlantı Başarısız! AP Moduna Geçiliyor...");
        startAPMode();
        return false;
    }
}

// Wi-Fi Bilgilerini Alıp EEPROM’a Kaydetme ve Bağlanma
void handleForm() {
    wifi_ssid = server.arg("name");
    wifi_password = server.arg("password");

    Serial.println("Yeni Wi-Fi Bilgileri Alındı:");
    Serial.println("SSID: " + wifi_ssid);
    Serial.println("Şifre: " + wifi_password);

    saveWiFiCredentials(wifi_ssid, wifi_password);  // EEPROM'a kaydet

    String response = "<h2>Bilgiler Alındı!</h2>";
    response += "<p><b>SSID:</b> " + wifi_ssid + "</p>";
    response += "<p><b>Şifre:</b> " + wifi_password + "</p>";
    response += "<button onclick=\"location.href='/connect'\">TAMAM</button>";

    server.send(200, "text/html", response);
}

// 📌 AP Modu Başlat (Wi-Fi Bilgisi Almak İçin)
void startAPMode() {
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP("NodeMCU_AP", "12345678");

    Serial.println("📡 AP Modu Aktif!");
    Serial.print("🌍 IP Adresi: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", []() { server.send(200, "text/html", htmlPage); });
    server.on("/submit", handleForm);
    server.on("/connect", connectToWiFi);
    
    server.begin();
    Serial.println("Web Sunucu Çalışıyor...");
}

// 📌 Kullanıcıdan Gelen Wi-Fi Bilgilerini İşle
void handleWiFiSubmission() {
    String new_ssid = server.arg("ssid");
    String new_password = server.arg("password");

    if (new_ssid.length() > 0 && new_password.length() > 0) {
        saveWiFiCredentials(new_ssid, new_password);
        server.send(200, "text/html", "<h2>Bilgiler Kaydedildi! Cihazı Yeniden Başlatın.</h2>");
    } else {
        server.send(200, "text/html", "<h2>Geçersiz Bilgiler!</h2>");
    }
}

// 📌 Reset Butonuna Basıldığında AP Moduna Geç
void checkResetButton() {
    pinMode(RESET_BUTTON, INPUT_PULLUP);
    if (digitalRead(RESET_BUTTON) == LOW) {
        Serial.println("🛑 Reset Butonuna Basıldı! AP Moduna Geçiliyor...");
        startAPMode();
    }
}
