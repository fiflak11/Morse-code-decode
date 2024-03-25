#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
String morseCode(String code){
  if(code=="._")
    return "A";
  if(code=="_...")
    return "B";
  if(code=="_._.")
    return "C";
  if(code=="_..")
    return "D";
  if(code==".")
    return "E";
  if(code==".._.")
    return "F";
  if(code=="__.")
    return "G";
  if(code=="....")
    return "H";
  if(code=="..")
    return "I";
  if(code==".___")
    return "J";
  if(code=="_._")
    return "K";
  if(code=="._..")
    return "L";
  if(code=="__")
    return "M";
  if(code=="_.")
    return "N";
  if(code=="___")
    return "O";
  if(code==".__.")
    return "P";
  if(code=="__._")
    return "Q";
  if(code=="._.")
    return "R";
  if(code=="...")
    return "S";
  if(code=="_")
    return "T";
  if(code==".._")
    return "U";
  if(code=="..._")
    return "V";
  if(code==".__")
    return "W";
  if(code=="_.._")
    return "X";
  if(code=="_.__")
    return "Y";
  if(code=="__..")
    return "Z";
  return "";
}
void setup(){
  Serial.begin(1200); 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
}
String display="", currentLetter="";
int shortSignal = 6000; 
bool run = true;
int bound = 500;
int one = 0,zero = 0;  //counters
int lightB = 0,light = 0; 
void loop() {
  if(!run){
    light = analogRead(A5);
    if(light>=bound)
      one+=1;
    else{
      shortSignal=one;
      one=0;
    }
    if(shortSignal>0 && one==0){
      run=true;
      Serial.println(shortSignal);
    }
  }
  else{
    lightB=light;
    light = analogRead(A5);
    if(lightB>=bound){
      one+=1;
      zero=0;
    }
    else{
      one=0;
      zero+=1;
    }
    if(lightB>=bound && light<bound){ //LIGHT CHANGE
      if(one){
        if(one<3*shortSignal && one>2500)
          currentLetter+=".";
        else if(one>2500)
          currentLetter+="_";
        Serial.println(currentLetter);
      }
    }
    if(zero>3*shortSignal){
      display+=morseCode(currentLetter);
      //lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(display);
      Serial.println(display);
      currentLetter="";
      zero=0;
    }
  }
}