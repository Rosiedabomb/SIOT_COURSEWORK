void setup() {
  // put your setup code here, to run once:
  pinMode (18, OUTPUT);
  pinMode (19, OUTPUT);
  pinMode (16, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(18, HIGH); 
  digitalWrite(19, HIGH); 
  digitalWrite(16, HIGH); 
  delay(1000);
  digitalWrite(18, LOW); 
  digitalWrite(19, LOW); 
  digitalWrite(16, LOW); 
  delay(1000);
  Serial.println("ONE CYCLE");
  // put your main code here, to run repeatedly:
}
