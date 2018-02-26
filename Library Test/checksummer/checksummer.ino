void setup() {
  Serial.begin(115200);

  uint8_t checksum = '!' ^ 'L' ^ 0x00 ^ 150 ^ '~' ^ 0xFF;
  Serial.println(checksum);
}

void loop() {
  // put your main code here, to run repeatedly:

}
