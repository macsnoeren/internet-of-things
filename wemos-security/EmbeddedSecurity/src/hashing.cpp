#include <hashing.h>

#include <Crypto.h> // https://github.com/intrbiz/arduino-crypto

SHA256 hasher;

void printSHA256 (byte hash[SHA256_SIZE]) {
  for (byte i=0; i < SHA256_SIZE; i++) {
      if (hash[i]<0x10) { Serial.print('0'); }
      Serial.print(hash[i], HEX);
  }
  Serial.println();
}

void getSHA256(String text, byte hash[SHA256_SIZE]) {
  hasher.doUpdate(text.c_str(), text.length());  
  hasher.doFinal(hash); // REQUIRED BUG FIX: https://github.com/intrbiz/arduino-crypto/pull/28
}

void testSHA256() {
  String text1 = "Maurice Snoeren";   // 4CCB5A7BF865C0F3AB446C6512B5B264F4BFBAF4A96802C36D41C69F51BF7057
  String text2 = "Embedded Security"; // 2589C599DFC1731E427AA3CF2D6A6757AA9E4E7D4E35B97DEB0D6E2799DC8814

  byte hash[SHA256_SIZE];

  unsigned long timing = micros();
  getSHA256(text1, hash);
  Serial.println("Timing: " + String(micros() - timing) + "us");
  Serial.print("HASH:  ");
  printSHA256(hash);
  Serial.println("CHECK: 4CCB5A7BF865C0F3AB446C6512B5B264F4BFBAF4A96802C36D41C69F51BF7057");

  timing = micros();
  getSHA256(text2, hash);
  Serial.println("Timing: " + String(micros() - timing) + "us");
  Serial.print("HASH:  ");
  printSHA256(hash);
  Serial.println("CHECK: 2589C599DFC1731E427AA3CF2D6A6757AA9E4E7D4E35B97DEB0D6E2799DC8814");
}