//library

// static var
#define LED_1 9
#define BTN_1 12

// global var
int analogPin=A0;
int analogValue=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_1, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int digitalBtn = digitalRead(BTN_1);
  analogValue = analogRead(analogPin);
  Serial.println("9");

  if (!digitalBtn){
    analogWrite(LED_1, analogValue/4);
  }else{
    analogWrite(LED_1, 0);
  }
}
