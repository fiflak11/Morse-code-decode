#include <LiquidCrystal.h>
String morseCode(String code){ //transform dots and dashes to letters
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

String display="", currentLetter=""; //dispaly is letters that we show on LCE, currentLetter stores dots and dashes
int shortSignal = 0; //how long "short" signal(.) should be (lower than 3*shortSignal is interpreta as short signal)
bool run = false; //it is for check if we start to send light signal
int line = 0; //it is boundries where we interprete signal as high/low
int one = 0,zero = 0;  //counters (how long signal is high/low)
int lightB = 0,light = 0; //light - current light intensity, lightB stores the previous value from light (to compare is light intesity getting bigger/lower)

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //16x2 display
void setup(){
  Serial.begin(1200); 
  pinMode(8, OUTPUT); //"run" diode HIGH IF RUN IS TRUE
  pinMode(12, OUTPUT); //"shortSignal" diode HIGH AS LONG AS LENGTH OF LIGHT SINGAL IS INTERPRETE AS SHORT
  pinMode(13, OUTPUT); //"longSingal" diode HIGH AS LONG AS LENGTH OF LIGHT SINGAL IS INTERPRETE AS LONG
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  line = analogRead(A5); // A5 get information about light intensity (0-1000), line shouldn't be close to boundaries so i limited it(300-700)
  if(line>700)
    line=700;
  else if(line<300)
    line=300;
}

void loop() {
  if(!run){ //waiting here to first light signal
    digitalWrite(8,LOW);
    light = analogRead(A5);
    if(light>=line)
      one+=1;
    else{
      shortSignal=one;
      one=0;
    }
    if(shortSignal>0 && one>0){ //if light signal has ended we set short signal (4000-6000) and start gathering information
      run=true;
      if(shortSignal>6000)
        shortSignal=4000;
      else if(shortSignal<4000)
        shortSignal=4000;
      Serial.println(shortSignal);
    } 
  }
  else{
    digitalWrite(8,HIGH);
    lightB=light;
    light = analogRead(A5);
    if(lightB>=line){
      one+=1;
      zero=0;
      if(one<3*shortSignal && one>shortSignal)
        digitalWrite(12,HIGH);
      else if(one>3*shortSignal){
        digitalWrite(13,HIGH);
        digitalWrite(12,LOW);
      }
    }
    else{
      one=0;
      zero+=1;
      digitalWrite(12,LOW);
      digitalWrite(13,LOW);
    }
    if(lightB>=line && light<line){ //LIGHT INSTENSITY CHANGEhg
        if(one<3*shortSignal && one>shortSignal) //short signal(.), one>shortSignal/2 this condition is for not to interpret very short light signal as dot
          currentLetter+=".";
        else if(one>shortSignal) //long signal(_)
          currentLetter+="_";
    }
    if(zero>3*shortSignal){ //if we get long low signal we tranform "currentLetter" into real letter and add it to dispaly and start to "write" new letter
      display+=morseCode(currentLetter);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(display);
      currentLetter="";
      zero=0;
    }
  }
}