#include <ecc.h>
#include <Arduino.h>
#include <Crypto.h> // https://github.com/intrbiz/arduino-crypto
#include <uECC.h>

// https://primalcortex.wordpress.com/2020/05/05/esp8266nodejs-ecdh/
// https://github.com/kmackay/micro-ecc?utm_source=platformio&utm_medium=piohome

static int RNG(uint8_t *dest, unsigned size) {
  RNG::fill(dest, size);
  return 1;
}

void setup_ecc() {
    uECC_set_rng(&RNG);
}

void testECC() {
  //const struct uECC_Curve_t * curve1 = uECC_secp160r1();
  //const struct uECC_Curve_t * curve2 = uECC_secp160r1();
  const struct uECC_Curve_t * curve1 = uECC_secp256r1();
  const struct uECC_Curve_t * curve2 = uECC_secp256r1();
  uint8_t private1[uECC_curve_private_key_size(curve1)];
  uint8_t private2[uECC_curve_private_key_size(curve2)];
  
  uint8_t public1[uECC_curve_public_key_size(curve1)];
  uint8_t public2[uECC_curve_public_key_size(curve2)];
  
  uint8_t secret1[32];
  uint8_t secret2[32];
  
  unsigned long a = millis();
  uECC_make_key(public1, private1, curve1);
  unsigned long b = millis();
  
  Serial.print("Made key 1 in "); Serial.print(b-a); Serial.println("ms");
  a = millis();
  uECC_make_key(public2, private2, curve2);
  b = millis();
  Serial.print("Made key 2 in "); Serial.print(b-a); Serial.println("ms");

  a = millis();
  int r = uECC_shared_secret(public2, private1, secret1, curve1);
  b = millis();
  Serial.print("Shared secret 1 in "); Serial.print(b-a); Serial.println("ms");
  if (!r) {
    Serial.print("shared_secret() failed (1)\n");
    return;
  }

  a = millis();
  r = uECC_shared_secret(public1, private2, secret2, curve2);
  b = millis();
  Serial.print("Shared secret 2 in "); Serial.print(b-a); Serial.println("ms");
  if (!r) {
    Serial.print("shared_secret() failed (2)\n");
    return;
  }
    
  if (memcmp(secret1, secret2, 20) != 0) {
    Serial.print("Shared secrets are not identical!\n");
  } else {
    Serial.print("Shared secrets are identical\n");
  }

  //uECC_sign(private1, , ))
}
