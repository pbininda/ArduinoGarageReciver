#include "RFControl.h"

const int RELAY_PIN = 2;
const int RECEIVER_PIN = 3;
const int RECEIVER_INTERRUPT_PIN = digitalPinToInterrupt(RECEIVER_PIN);


void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RECEIVER_PIN, INPUT);
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  Serial.print("Hello 433\n");

  RFControl::startReceiving(RECEIVER_INTERRUPT_PIN);
}

bool in_raw_mode = false;

const char s00[] PROGMEM = "001011001010101010110010101010101011010011001011001010101010101102";  // remote 0, 1
const char s01[] PROGMEM = "001010101011010101001100110100101011001100110100101010101010101102";  // remote 2
const char s02[] PROGMEM = "001010101011010101001101001010110010110010110101001010101010101102";  // remote 3

const char *const signatures [] PROGMEM = {
  s00, s01, s02
};

int num_sigs = sizeof(signatures) / sizeof(const char *);

void toggle_relay() {
  Serial.println("on");
  digitalWrite(RELAY_PIN, HIGH);
  delay(500);                     // press relay for half a second
  Serial.println("off");
  digitalWrite(RELAY_PIN, LOW);
  delay(3000);                    // cooldown of 3 seconds  
}


void loop() {
  if(RFControl::hasData()) {
    unsigned int *timings;
    unsigned int timings_size;
    RFControl::getRaw(&timings, &timings_size);
    unsigned int buckets[8];
    buckets[0] = 200;
    buckets[1] = 400;
    buckets[2] = 6000;
    RFControl::compressTimings(buckets, timings, timings_size);
    int found = -1;
    String signature = "";
    for(unsigned int i=0; i < timings_size; i++) {
      char c = '0' + timings[i];
      signature += c;
    }
    for (int s = 0; s < num_sigs; s++) {
      static char buffer[128];
      strcpy_P(buffer, (char*)pgm_read_word(&(signatures[s])));
      String check(buffer);
      if (signature == check) {
        found = s;
      }
    }
    for (int t = 0; t < 8; t++) {
      Serial.print(buckets[t]);
      Serial.print(" ");
    }
    if (found != -1) {
      Serial.print(signature);
      Serial.print(" ");
      Serial.println(found);
      toggle_relay();
    }
    else {
      Serial.print(signature);
      Serial.println(" not found");
    }
    RFControl::continueReceiving();
  }
}
