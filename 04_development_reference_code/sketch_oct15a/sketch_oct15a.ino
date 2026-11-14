int led_yellow = 9;
int led_red = 10;

void setup() {
  pinMode(led_yellow, OUTPUT);
  pinMode(led_red, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(led_yellow, HIGH); 
  delay(1000);                   
  digitalWrite(led_yellow, LOW);  
  digitalWrite(led_red, HIGH);
  delay(1000);                                    
  digitalWrite(led_red, LOW); 
}
