const int REED_SWT0 = 23;
const int LED_BUILTIN = 2;

void setup() {
  Serial.begin(115200);

  pinMode(REED_SWT0, INPUT_PULLDOWN);
  pinMode(LED_BUILTIN, OUTPUT);

  // initially led is on
  digitalWrite(LED_BUILTIN, HIGH);

  while(digitalRead(REED_SWT0)==1){
    Serial.println("Box not opened..");
    delay(1000);
  }

  while(digitalRead(REED_SWT0)==0){
    Serial.println("Box opened..");
    delay(1000);
  }

  if(digitalRead(REED_SWT0)==1){
    Serial.println("Box again closed..");
    // initially led is on
    digitalWrite(LED_BUILTIN, LOW);
  }







}

void loop() {
  // put your main code here, to run repeatedly:

}
