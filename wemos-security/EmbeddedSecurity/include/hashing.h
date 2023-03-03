#pragma once

#include <Arduino.h>
#include <Crypto.h> // https://github.com/intrbiz/arduino-crypto

void printSHA256 (byte hash[SHA256_SIZE]);
void getSHA256(String text, byte hash[SHA256_SIZE]);
void testSHA256();


