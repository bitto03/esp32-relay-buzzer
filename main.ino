//BHB

#include <WiFi.h>
#include <WebServer.h>

// ====== WiFi credentials =======
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// GPIO pins
const int relayPin = 25;
const int buzzerPin = 26;

// Web server on port 80
WebServer server(80);

// Relay and buzzer states
bool relayState = false;
unsigned long buzzerTimer = 0;
bool buzzerOn = false;

// HTML page content (Single Page Application style)
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Relay & Buzzer Control</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; }
    button { padding: 20px; font-size: 24px; cursor: pointer; }
    #status { margin-top: 20px; font-size: 18px; }
  </style>
</head>
<body>
  <h1>Relay & Buzzer Control</h1>
  <button id="toggleBtn">Loading...</button>
  <div id="status"></div>

<script>
  async function fetchState() {
    const resp = await fetch('/state');
    const data = await resp.json();
    document.getElementById('toggleBtn').innerText = data.relay ? 'Turn OFF' : 'Turn ON';
    document.getElementById('status').innerText = data.relay ? 'Relay is ON. Buzzer beeping...' : 'Relay is OFF. Buzzer silent.';
  }

  async function toggleRelay() {
    const resp = await fetch('/toggle', { method: 'POST' });
    const data = await resp.json();
    document.getElementById('toggleBtn').innerText = data.relay ? 'Turn OFF' : 'Turn ON';
    document.getElementById('status').innerText = data.relay ? 'Relay is ON. Buzzer beeping...' : 'Relay is OFF. Buzzer silent.';
  }

  document.getElementById('toggleBtn').addEventListener('click', toggleRelay);
  window.onload = fetchState;
</script>

</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", htmlPage);
}

// Send current relay state as JSON
void handleState() {
  String json = "{\"relay\":";
  json += relayState ? "true" : "false";
  json += "}";
  server.send(200, "application/json", json);
}

// Toggle relay state
void handleToggle() {
  relayState = !relayState;
  digitalWrite(relayPin, relayState ? HIGH : LOW);
  // Reset buzzer state when toggled
  buzzerOn = false;
  buzzerTimer = millis();

  String json = "{\"relay\":";
  json += relayState ? "true" : "false";
  json += "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  digitalWrite(buzzerPin, LOW);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected! IP: " + WiFi.localIP().toString());

  // Setup server routes
  server.on("/", handleRoot);
  server.on("/state", handleState);
  server.on("/toggle", HTTP_POST, handleToggle);

  server.begin();
}

void loop() {
  server.handleClient();

  // Buzzer beep logic when relay ON
  if (relayState) {
    unsigned long currentMillis = millis();
    if (currentMillis - buzzerTimer >= 500) { // 500 ms beep cycle
      buzzerOn = !buzzerOn;
      digitalWrite(buzzerPin, buzzerOn ? HIGH : LOW);
      buzzerTimer = currentMillis;
    }
  } else {
    digitalWrite(buzzerPin, LOW);
  }
}
