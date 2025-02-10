// Relay control on a WeMos D1 R2 (ESP8266)
// This sketch turns the relay on and off every 1 second.
// Ensure your relay circuit is properly interfaced with the board.

#define RELAY_PIN D5  // Define the relay pin (D5 corresponds to a specific GPIO on the ESP8266)

void setup() {
  // Initialize the relay pin as an output.
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  // Turn the relay on (set the pin HIGH)
  digitalWrite(RELAY_PIN, HIGH);
  delay(1000);  // Wait for 1 second

  // Turn the relay off (set the pin LOW)
  digitalWrite(RELAY_PIN, LOW);
  delay(1000);  // Wait for 1 second
}
