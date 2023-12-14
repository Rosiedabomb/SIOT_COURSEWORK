#define AO_PIN_sound 39 //assigning the sound sensor variable to the input of GPIO39

void setup() {
Serial.begin(9600);//starting the serial monitor

}

void loop() {
//creating an integer variable to hold sound reading
int SoundValue = analogRead(AO_PIN_sound);

//PRINTING SOUND LEVELS TO SERIAL MONITOR
  Serial.print("The MICROPHONE value: ");
  Serial.println(SoundValue);
delay(500);
}
