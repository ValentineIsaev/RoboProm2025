#include <SoftwareSerial.h>
SoftwareSerial kvu(7, 8);

int prevButtonState = 0;
int buttonState = 0;

void setup() {
  Serial.begin(9600);
  kvu.begin(9600);

  waitLedState();
}

int readStateButton(int buttonId){
  Serial.print("Check button:");
  Serial.println(buttonId);

  if (Serial.available() > 0) {
    String charData = Serial.readStringUntil('\n');
    charData.trim();
    return charData.toInt();
  }

  return -1;
}

void writeLed(int ledId, int state){
  // 0 - red, 1 - green, 2 - blue
  
  Serial.print("Write Led:");
  Serial.print(ledId);
  Serial.print(":");
  Serial.println(state);
}

void waitLedState(){
  writeLed(0, 1);
  writeLed(1, 0);
  writeLed(2, 0);
}

void workingLedState(){
  writeLed(0, 0);
  writeLed(1, 0);
  writeLed(2, 1);
}

void endWorkLedState(){
  writeLed(0, 0);
  writeLed(1, 1);
  writeLed(2, 0);
}

String byteAnalyze(){
  int byteCount = 0;
  while (kvu.available() > 0) {
    byte data = kvu.read();
    if (data != 0x0D && data != 0x0A) {
      byteCount++; 
    }
  }
  String state = "";
  switch (byteCount){
    case 1:
      state = "Complete";
      break;
    case 2:
      state = "Wait";
      break;
  }
  
  return state;
}

void loop() {
    prevButtonState = buttonState;
    buttonState = readStateButton(0);
    
    kvu.print("B:");
    if (buttonState == 1 && prevButtonState == 0){
      workingLedState();
      kvu.print(1);
    }
    else{
      kvu.print(0);
    }
    kvu.println("#");

    if (kvu.available() > 0){
      kvu.print("Got data:");
      String state = byteAnalyze();
      kvu.println(state);

      if (state.equals("Wait")){  
          waitLedState();    
      }
      else if (state.equals("Complete")){
          endWorkLedState();  
      }
      else{
        kvu.print("E:Wrong state - ");
        kvu.println(state);
      }
    }

    delay(10);
}
