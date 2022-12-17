#include <WiFi.h>
#include <PubSubClient.h>
#include <Keypad.h>


int latchPin = 2;
int clockPin = 4;
int dataPin = 15;

long lastMsg = 0;
char msg[50];
int value = 0;

WiFiClient espClient;
PubSubClient client(espClient);

char keys[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[4] = { 14, 27, 26, 25 };
byte colPins[4] = { 13, 21, 22, 23 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

const int tictactoe[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // " | | "

  // Rows
  0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // "X| | "
  0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00,  // " |X| "
  0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00,  // " | |X"
};


// WiFi credentials
const char* ssid = "viPhone";
const char* password = "12345678";

// Mosquitto MQTT broker information
const char* broker_host = "test.mosquitto.org";
const int broker_port = 1883;


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(broker_host, broker_port);
  client.setCallback(callback);

  // Set up pins to the led matrix
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  char key = keypad.getKey();
  if (key != NO_KEY) {
    String str = String(key);
    const char* msg = str.c_str();

    // Publish the message
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("ESP32/input", msg);
  }

  // Connect to the Mosquitto MQTT broker
  // // Subscribe to a topic
  // client.subscribe("ESP32/input");

  // int cols = 0x01;                                     // Assign binary 00000001. Means the first column is selected.
  // for (int j = 0; j < 8; j++) {                        // display image of each frame
  //   matrixRowsVal(pgm_read_word_near(tictactoe + j));  // display the data in this column
  //   matrixColsVal(~cols);                              // select this column
  //   cols <<= 1;                                        // shift"cols" 1 bit left to select the next column
  // }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe("computer/input");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void matrixRowsVal(int value) {
  // make latchPin output low level
  digitalWrite(latchPin, LOW);
  // Send serial data to 74HC595
  shiftOut(dataPin, clockPin, LSBFIRST, value);
  // make latchPin output high level, then 74HC595 will update the data to parallel output
  digitalWrite(latchPin, HIGH);
}

void matrixColsVal(int value) {
  // make latchPin output low level
  digitalWrite(latchPin, LOW);
  // Send serial data to 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, value);
  // make latchPin output high level, then 74HC595 will update the data to parallel output
  digitalWrite(latchPin, HIGH);
}

