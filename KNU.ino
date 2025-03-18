//#include <SoftwareSerial.h>
//SoftwareSerial kvu(7, 8);

int prevButtonState = 0;
int buttonState = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  waitLedState();
}

int readStateButton(int buttonId){
  Serial.print("Check button:");
  Serial.println(buttonId);

  delay(10);
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

String stringAnalyze(){
  Serial1.print("Got data:");
  String data = Serial1.readStringUntil('\n');
  data.trim();
  Serial1.print(data);
  int index = data.indexOf(':');
  return data.substring(index+1); 
}

void loop() {
    prevButtonState = buttonState;
    buttonState = readStateButton(0);
    
    Serial1.print("B:");
    if (buttonState == 0 && prevButtonState == 1){
      workingLedState();
      Serial1.print(1);
    }
    else{
      Serial1.print(0);
    }
    Serial1.println("#");

    if (Serial1.available() > 0){
//      String state = byteAnalyze();
      String state = stringAnalyze();
      Serial1.println(state);

      if (state.equals("Wait")){  
          waitLedState();    
      }
      else if (state.equals("Complete")){
          endWorkLedState();  
      }
      else{
        Serial1.print("E:Wrong state - ");
        Serial1.println(state);
      }
    }
}
