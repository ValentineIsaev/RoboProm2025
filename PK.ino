const int ledRed = 2;  // RED
const int ledGreen = 3;  // GREEN
const int ledBlue = 4;  // BLUE
const int button = 8;

void setup() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  Serial.begin(115200);
}

int extractNumber(String str){
  String number = "";

  for (int i = 0; i<str.length(); i++){
    char element = str[i];

    if (isDigit(element))
       number += element;
  }

  if (number.length() > 0)
      return number.toInt();
  return -1;
}

int checkButtonState(int buttonId){
  int myButton;
  
  switch (buttonId){
    case 0:
      myButton = button;
      break;
    default:
      myButton = 0;
      break;
  }
  int buttonState = digitalRead(myButton); 

  return ! buttonState;
} 

void writeLed(int ledId, int power){
  int pinLed;
  
  switch (ledId){
    case 0:
      pinLed = ledRed;
      break;
    case 1:
      pinLed = ledGreen;
      break;
    case 2:
      pinLed = ledBlue;
      break;
    default:
      pinLed = ledRed;
      break; 
  }

  digitalWrite(pinLed, power);
}

void loop() {
  if (Serial.available() > 0){
    String command = Serial.readStringUntil('\n');

    if (command.startsWith("Check button")){
      int buttonId = extractNumber(command);

      Serial.println(checkButtonState(buttonId));
    }
    else if (command.startsWith("Write Led")){
      int firstColonIndex = command.indexOf(':');
      int secondColonIndex = command.indexOf(':', firstColonIndex + 1);

      int led = extractNumber(command.substring(0, secondColonIndex + 1));
      int state = extractNumber(command.substring(secondColonIndex + 1));

      writeLed(led, state);
    }
    
    }
}
