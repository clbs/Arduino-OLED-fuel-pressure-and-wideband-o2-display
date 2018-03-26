#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4

Adafruit_SSD1306 display1(OLED_RESET);
Adafruit_SSD1306 display2(OLED_RESET);

const int fpVolts = A0;
int fpSignal = 0;
int fpOutput = 0;
int fpFinal = 0;
const int wbVolts = A1;
int wbSignal = 0;
int wbOutput = 0;
int wbFinal = 0;
int counter1;
const int sampleRate = 0; //in milliseconds


void setup() {
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);

}



void display1Draw(){
  display1.clearDisplay();
  display1.setTextSize(1);
  display1.setTextColor(WHITE);
  display1.setCursor(0,0);
  display1.println("Fuel Pressure:");
  display1.setTextSize(1);
  display1.setCursor(45,24);
  display1.println("PSI");
  
}

void display2Draw(){

  display2.clearDisplay();
  display2.setTextSize(1);
  display2.setTextColor(WHITE);
  display2.setCursor(0,0);
  display2.println("Wideband 02:");
  display2.setTextSize(1);
  display2.setCursor(95,24);
  display2.println("AFR");
}


void fuelPress() {
  
  //fpSignal = analogRead(fpVolts); //Uncomment this to get out of the test version
   //test counter
  fpSignal = counter1;
  
  fpOutput = map(fpSignal, 0, 1023, 0, 5000);
  fpFinal = ((fpOutput - 500)/40);
  String Output;
  Output = fpFinal;
  
  if (fpFinal > 99) {
    display1.setTextSize(3);
    display1.setCursor(0,10);
    display1.println("OL");   
  }

  else if (fpFinal < 0){
    display1.setTextSize(3);
    display1.setCursor(0,10);
    display1.println("UL");   
  }
  
  else {
    display1.setTextSize(3);
    display1.setCursor(0,10);
    display1.println(Output);
  }
 }




void wbAFR() {
  
  //wbSignal = analogRead(wbVolts);  //Uncomment this to get out of the test version
   //test counter
  wbSignal = counter1;
  
  wbOutput = map(wbSignal, 0, 1023, 0, 5000); 
  
  String wbFinalStr;
  String wholenumber = ("0"); // String for whole number output of our sensor value
  int wbcalc = (wbOutput/50*15+735); // Calculation to convert a 0-5V input to the sensor output and stay within a 16 bit integer
  String deci = ("0"); // String for decimal output
  String wbCalcStr = String(wbcalc); // Converts calculation to string to parse
   // Pulls the last 2 characters of the string for our decimal place data

  // checks the calculated sensor value so we can decide how far from the left we need to parse the string
  // could have also checked the length of the string for validation
  
  if (wbcalc > 999){
      wholenumber = wbCalcStr.substring(0,2);
      deci = wbCalcStr.substring(2,4);
  }
  else if (wbcalc < 999 || wbcalc > 99){
      wholenumber = wbCalcStr.substring(0,1);
      deci = wbCalcStr.substring(1,3);
  }
  else {
      wholenumber = ("X");
      deci = ("X");
  }

  wbFinalStr = (wholenumber + "." + deci);
  display2.setTextSize(3);
  display2.setCursor(0,10);
  display2.println(wbFinalStr);
  
}

void loop() {
  for (counter1 = 0; counter1 < 1023; counter1++) {
  // Fuel pressure gauge display code
  display1Draw();
  fuelPress();
  display1.display();
  // Wideband gauge display code
  display2Draw();
  wbAFR();
  display2.display();
  delay(sampleRate);
  
  }
}
