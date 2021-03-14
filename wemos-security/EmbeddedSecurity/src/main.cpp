#include <Arduino.h>

#include <hashing.h>
#include <aes.h>
#include <ecc.h>



// https://github.com/ARMmbed/mbedtls?utm_source=platformio&utm_medium=piohome => ESP32 Very good!

unsigned long timer = millis();



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("\nsetup finished!");
  
  setup_ecc();
  
}

void loop() {
  if ( millis() - timer > 2000 ) {
    digitalWrite( LED_BUILTIN, !digitalRead(LED_BUILTIN) );
    timer = millis();
  }
    
  testSHA256();
  testAES();
  testECC();

  //delay(10000);
}