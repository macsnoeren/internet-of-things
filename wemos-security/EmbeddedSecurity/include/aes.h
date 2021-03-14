#pragma once

#include <Arduino.h>
#include <Crypto.h> // https://github.com/intrbiz/arduino-crypto

#define AES_BLOCK_SIZE 32 // 16:128, 32:256

void bufferSize(const char* text, int &length);

String encrypt(String plainText);
void decrypt(String ciphertext, char* output, int length);
void testAES();
