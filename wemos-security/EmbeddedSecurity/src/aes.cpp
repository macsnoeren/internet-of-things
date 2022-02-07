#include <aes.h>

#include <base64.hpp>

byte key[AES_BLOCK_SIZE] = { 0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41, 0x41, 0x41, 0x41, 0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41, 0x41, 0x41 };
//byte iv[AES_BLOCK_SIZE]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void bufferSize(const char* text, int &length) {
  int i = strlen(text);
  int buf = round(i / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
  length = (buf <= i) ? buf + AES_BLOCK_SIZE : length = buf;
}

String encrypt(String plainText) {
  int length = 0;
  bufferSize(plainText.c_str(), length); // Calculate the length of the cipher!
  byte enciphered[length];

  byte iv[AES_BLOCK_SIZE];
  RNG::fill(iv, AES_BLOCK_SIZE); // Trusted Platform Hardware Random Generator!

  AES aesEncryptor(key, iv, AES::AES_MODE_256, AES::CIPHER_ENCRYPT);
  aesEncryptor.process((uint8_t*)plainText.c_str(), enciphered, length);

  int encrypted_size = sizeof(enciphered);
  char encoded[encrypted_size];
  encode_base64(enciphered, encrypted_size, (unsigned char*)encoded);

  char iv_encoded[sizeof(iv)];
  encode_base64(iv, sizeof(iv), (unsigned char*)iv_encoded);

  Serial.println("EN: " + String(encoded));
  Serial.println("IV: " + String(iv_encoded));
  Serial.println("CT: " + String(encoded) + String(iv_encoded));

  return String(encoded) + String(iv_encoded);
}

String decrypt(String ciphertext) {
  String _iv_decrypt_base64 = ciphertext.substring(ciphertext.length()-44);
  Serial.println("Extracted IV: " + _iv_decrypt_base64);

  byte _iv[AES_BLOCK_SIZE]; // Get the bytes!
  decode_base64((unsigned char*)_iv_decrypt_base64.c_str(), (unsigned char*)_iv);

  String _ciphertext_base64 = ciphertext.substring(0, ciphertext.length()-44);
  char decoded[ciphertext.length()-44]; // get the bytes/char
  decode_base64((unsigned char*)ciphertext.c_str(), (unsigned char*)decoded);

  int length = 0;
  bufferSize(ciphertext.c_str(), length);
  char deciphered[length - AES_BLOCK_SIZE]; // Remove the iv!

  AES aesDecryptor(key, _iv, AES::AES_MODE_256, AES::CIPHER_DECRYPT);
  aesDecryptor.process((uint8_t*)decoded, (uint8_t*) deciphered, length);
  
  return String(deciphered);
}

void testAES() {
  String text1 = "Maurice Snoeren";
  String text2 = "Embedded security is difficult but so cool when things are going well!!";

  unsigned long timing = micros();
  String ciphertext = encrypt(text1);
  Serial.println("Timing encrypt: " + String(micros() - timing) + "us");
    
  timing = micros();
  String plaintext = decrypt(ciphertext);
  Serial.println("Timing Decrypt: " + String(micros() - timing) + "us");
  Serial.println("Decrypted: " + plaintext);

  timing = micros();
  ciphertext = encrypt(text2);
  Serial.println("Timing encrypt: " + String(micros() - timing) + "us");
    
  timing = micros();
  plaintext = decrypt(ciphertext);
  Serial.println("Timing Decrypt: " + String(micros() - timing) + "us");
  Serial.println("Decrypted: " + plaintext);

  Serial.print("KEY: ");
  for ( uint8_t i=0; i < AES_BLOCK_SIZE; i++ ) {
      Serial.print(String((char)key[i]));
  }
  Serial.println();
}
