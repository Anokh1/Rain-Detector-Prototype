#include "WiFi.h"
#include "Arduino.h"
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// Firebase Realtime Database 
const char* ssid = "Adrian";
const char* password = "";

#define API_KEY ""

#define DATABASE_URL ""

#define USER_EMAIL "nprpgurneyparagon@marc.com"
#define USER_PASSWORD "test123"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;

#define POWER_PIN 32  // Rain Sensor VCC PIN
#define DO_PIN 13     // Rain Sensor PIN

void initWiFi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void setup() {
  Serial.begin(115200);
    initWiFi(); 

    configF.api_key = API_KEY;
    configF.database_url = DATABASE_URL;

    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    configF.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

    Firebase.begin(&configF, &auth);
    Firebase.reconnectWiFi(true);

    pinMode(POWER_PIN, OUTPUT);
    pinMode(DO_PIN, INPUT); 
}

void loop() {
  digitalWrite(POWER_PIN, HIGH); 
    delay(10); 

    int rain_state = digitalRead(DO_PIN);
    digitalWrite(POWER_PIN, LOW); 

    if (rain_state == HIGH) {
        Serial.println("rainless"); 
        // Store not raining data in Realtime Database
        if (Firebase.RTDB.setString(&fbdo, "gurneyParagonCamera/rainGurneyParagon", "rainless")){
            Serial.println(); Serial.print(" - successfully saved to: " + fbdo.dataPath()); 
            Serial.println(" (" + fbdo.dataType() + ")"); 
        } else {
            Serial.println("FAILED: " + fbdo.errorReason()); 
        }
    }
    else if (rain_state == LOW) {
        Serial.println("raining");
        // Store raining data in Realtime Database
        if (Firebase.RTDB.setString(&fbdo, "gurneyParagonCamera/rainGurneyParagon", "raining")){
            Serial.println(); Serial.print(" - successfully saved to: " + fbdo.dataPath()); 
            Serial.println(" (" + fbdo.dataType() + ")"); 
        } else {
            Serial.println("FAILED: " + fbdo.errorReason()); 
        }
    }
    else {
        Serial.println("no information");
        // Store not raining data in Realtime Database
        if (Firebase.RTDB.setString(&fbdo, "gurneyParagonCamera/rainGurneyParagon", "no information")){
            Serial.println(); Serial.print(" - successfully saved to: " + fbdo.dataPath()); 
            Serial.println(" (" + fbdo.dataType() + ")"); 
        } else {
            Serial.println("FAILED: " + fbdo.errorReason()); 
        }
    }

    delay(1000);
}
