#include <Servo.h>
#include <LiquidCrystal_I2C.h>
Servo mot;
Servo mott;
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define echo 3
#define trig 2
#define echoo 8
#define trigg 7
int angle = 0;
int angle_2 = 0;
int  mesafe, zaman, veri, ran, mesafe_2, zaman_2;
String data;
const unsigned long period = 100;
const unsigned long period_2 = 500;
unsigned long currentTime;
unsigned long previousTime = 0;
unsigned long currentTime_2;
unsigned long previousTime_2 = 0;
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  lcd.backlight();
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echoo, INPUT);
  pinMode(trigg, OUTPUT);
  mot.attach(6);
  mott.attach(5);
  mot.write(0);
  mott.write(0);

}

void loop() {
  if (Serial.available() > 1) {
    data = Serial.readString();                 //Get data from serial to decide whether it's automatic or manuel control
  }
  veri = data.toInt();
  
  if (veri == 5000) {                         //Manuel control loop
    while (true) {
      currentTime = millis();
      if (currentTime - previousTime >= period) {     //Distance measurement with millis for delaying
        distance();
        previousTime = currentTime;
      }
      currentTime_2 = millis();
      if (currentTime_2 - previousTime_2 >= period_2) {   //Printing measured distance to serial monitor
        lcdPrint();
        previousTime_2 = currentTime_2;
      }


      if (Serial.available() > 1) {                 //Receiving data from serial to control servo motor
        data = Serial.readString();
      }
      veri = data.toInt();
      if (veri > 10 && veri < 190) {               //Control of servo 1
        mot.write(veri - 10);
      }
      else if (veri > 190 && veri <= 370) {     //Control of servo 2
        mott.write(veri - 190);
      }
      else if (veri == 6000) {                 //Breaking out from loop if changed to automatic mode from 
        break;                                  //python gui and switch to automatic mode
      }

    }

  }
  else if (veri == 6000) {                  //Automatic Control loop
    while (true) {
      currentTime = millis();
      if (currentTime - previousTime >= period) {
        distance();                               //Distance measurement and printing results to serial
        Serial.print(mesafe);
        Serial.print(" ");
        Serial.print(mesafe_2);
        Serial.print(" ");
        if (angle >= 0 && angle < 180) {        
          Serial.println(angle);
        }
        if (angle >= 180 && angle < 360) {
          Serial.println(angle_2);
        }
        if (angle >= 0 && angle < 180) {        //Control automatically servo
          mot.write(angle);
          mott.write(angle);
          angle += 1;
        }
        if (angle >= 180 && angle < 360) {
          angle_2 = 360 - angle;
          mot.write(angle_2);
          mott.write(angle_2);
          angle += 1;
        }
        if (angle == 360) {
          angle = 0;
        }
        previousTime = currentTime;
      }
      
      currentTime_2 = millis();

      if (currentTime_2 - previousTime_2 >= period_2) {    //Printing to lcd according to data gotten python gui
        if (veri == 1000) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Mesafe1: ");
          lcd.print(mesafe);
          lcd.setCursor(0, 1);
          lcd.print("Mesafe2: ");
          lcd.print(mesafe_2);
        }
        if (veri == 2000) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Servo: ");
          if (angle >= 0 && angle < 180) {
            lcd.print(angle);
          }
          if (angle >= 180 && angle < 360) {
            lcd.print(angle_2);
          }
        }
        if (veri == 5000) {                   //Breaking out from the loop if manuel control is clicked
          break;
        }
        previousTime_2 = currentTime_2;
      }
      if (Serial.available() > 1) {             //Receiving data from python gui for lcd print
        data = Serial.readString();
      }
      veri = data.toInt();

    }
  }

}
void distance() {                   //distance measurement function
  digitalWrite(trig, HIGH);
  delayMicroseconds(100);
  digitalWrite(trig, LOW);
  zaman = pulseIn(echo, HIGH);
  mesafe = (zaman / 2) / 29.1;
  digitalWrite(trigg, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigg, LOW);
  zaman_2 = pulseIn(echoo, HIGH);
  mesafe_2 = (zaman_2 / 2) / 29.1;
}
void lcdPrint() {                 //serial and lcd print function
  Serial.print(mesafe);
  Serial.print(" ");
  Serial.println(mesafe_2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mesafe 1,2  Servo");
  lcd.setCursor(0, 1);
  lcd.print(mesafe);
  lcd.setCursor(6, 1);
  lcd.print(mesafe_2);
  lcd.setCursor(12, 1);
  lcd.print(veri - 10);
}
