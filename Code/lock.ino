#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Servo.h>

bool locked = true;
int buttonLeft = 6;
int buttonMiddle = 10;
int buttonRight = 8;
int buttonState;
int prevStateLeft;
int prevStateMiddle;
int prevStateRight; 
int eeAddress = 0;
const int EEPROMSIZE=1024;
int lockCode[3] = { 2, 3, 2}; 
int userInput[3] = { 0, 0, 0}; 
bool comboStatus;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//int pos = 0;

Servo myservo;
Servo myservo2;

void setup() {
  // put your setup code here, to run once:
  //clearEeprom();
  Serial.begin(9600);
  myservo.attach(9);
  myservo2.attach(7);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonMiddle, INPUT);
  pinMode(buttonRight, INPUT);
  prevStateLeft = digitalRead(buttonLeft);
  prevStateMiddle = digitalRead(buttonMiddle);
  prevStateRight = digitalRead(buttonRight);
  Serial.println(prevStateLeft);
  lcd.begin(16, 2);
  lcd.print("hello!");
  lcdclear();
  myservo.write(180);
  myservo2.write(0);

  Serial.println("Starting");
  clearEeprom();
}

void loop() {
  // put your main code here, to run repeatedly:
    
    
    if (eeAddress >= 3){
      //getCombination();
      comboStatus = validateCombination();
      if (comboStatus == true){
        Serial.println("Combo Worked");
        lcdclear();
       
        lcd.print("Correct Combo!");

        if (locked == true) {
        
            Serial.println("In Locked");  
            Serial.println(myservo.read());
            Serial.println(myservo2.read());
            myservo.write(0);
            /*
            for (int pos2 = 90; pos2 >= 0; pos2 -= 1) { // goes from 180 degrees to 0 degrees
              myservo.write(pos2);// tell servo to go to position in variable 'pos'
              
              delay(5); 
          }
          */
          myservo2.write(180);
          locked = false;
        }
        else {
          Serial.println("Not Locked");
          Serial.println(myservo.read());
          Serial.println(myservo2.read());
           /*
           for (int pos = 0; pos <= 90; pos -= 1) { // goes from 0 degrees to 180 degrees
              myservo.write(pos);   
              
              delay(5); 
          }
          */
          myservo.write(180);
             
          myservo2.write(0);
          locked = true;
        }
      }
      else{
        Serial.println("Combo Failed");
        lcdclear();
        lcd.print("Combo Failed");
        
        }
      clearEeprom();
      eeAddress = 0;
      }
    
    int currentStateLeft = digitalRead(buttonLeft);
    if (currentStateLeft != prevStateLeft && currentStateLeft == HIGH) {
      Serial.println("Left Button Clicked");
      //EEPROM.put(eeAddress, 1);
      userInput[eeAddress] = 1;
      lcdclear();
      lcd.print("1");
      eeAddress = eeAddress + 1;
      delay(50);
    }
    prevStateLeft = currentStateLeft;
    
    //Serial.println("In Loop");
    
    int currentStateMiddle = digitalRead(buttonMiddle);
    //Serial.println(currentStateMiddle);
    if (currentStateMiddle != prevStateMiddle && currentStateMiddle == HIGH) {
      Serial.println("Middle Button Clicked");
      //EEPROM.put(eeAddress, 2);
      userInput[eeAddress] = 2;
      lcdclear();
      lcd.print("2");
      eeAddress = eeAddress + 1;
      delay(50);
    }
    prevStateMiddle = currentStateMiddle;
    
  
    
    int currentStateRight = digitalRead(buttonRight);
    
    if (currentStateRight != prevStateRight && currentStateRight == HIGH) {
      Serial.println("Right Button Clicked");
      //EEPROM.put(eeAddress, 3);
      userInput[eeAddress] = 3;
      lcdclear();
      lcd.print("3");
      eeAddress = eeAddress + 1;
      delay(50);
    }
    prevStateRight = currentStateRight;
  
}

void clearEeprom() {
  for (int i = 0; i < EEPROMSIZE; i++) {
    EEPROM.write(i, 0);
  }
}

bool validateCombination(){


   
   
   for (int i = 0; i < 3; i++) {
    Serial.println(lockCode[i]);
    if (userInput[i] != lockCode[i]) {
      return false;
      }
    }
    return true;  
  }

void lcdclear(){
      lcd.clear();
      lcd.setCursor(0, 1);
  }
