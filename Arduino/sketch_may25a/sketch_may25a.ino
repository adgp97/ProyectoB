void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int d = 0; d<360; d++){

    Serial.println(sin(DEG_TO_RAD * d));
    
  }
}
