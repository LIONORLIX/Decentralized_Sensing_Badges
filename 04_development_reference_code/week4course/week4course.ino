#define ARRAYSIZE 3

String myStrings[ARRAYSIZE] = {"un","dex","trois"};
int myPins[ARRAYSIZE] = {7,8,9};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i=0; i<sizeof(myPins); i++){
    pinMode(myPins[i], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println(sizeof(myStrings)/sizeof(myStrings[0]));
  for (int i=0; i<sizeof(myStrings)/sizeof(myStrings[0]); i++){
    Serial.println(myStrings[i]);
  }
  delay(1000);
}
