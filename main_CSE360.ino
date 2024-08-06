#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#include <Wire.h>

 
#define SS_PIN 10
#define RST_PIN 9
//#define LED_G 4 //define green LED pin
#define LED_R 5 //define red LED
#define BUZZER 8 //buzzer pin
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myServo; //define servo name

int IR1 = 4;
int IR2 = 2;

int Slot = 4;           //Enter Total number of parking Slots

int flag1 = 0;
int flag2 = 0;

const int trigPin = 6;
const int echoPin = 7;
const int ledPin = 5;
long duration;
int distance;
int safetyDistance;
void setup()
{
  //lcd setup
//  lcd.init();
//  lcd.backlight();
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
   
  myServo.attach(3);
  myServo.write(100);
 
//  lcd.setCursor (0,0);
//  lcd.print("ARDUINO");
//  lcd.setCursor (0,1);
//  lcd.print("PARKING SYSTEM");
//  delay (2000);
//  lcd.clear();  
 
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myServo.attach(3); //servo pin
  myServo.write(0); //servo start position
  //pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  Serial.println("Put your card to the reader...");
  Serial.println();

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ledPin, OUTPUT);

}
void loop()
{
  //Serial.println("Hello");
  // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;

safetyDistance = distance;
if (safetyDistance <= 20){ //Enter the Distance
  digitalWrite(ledPin, HIGH);
  digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(1000);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
}
else{
  digitalWrite(ledPin, LOW);
}
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
 
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "F3 C6 7F 16") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    //digitalWrite(LED_G, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    //myServo.write(180);
    delay(5000);
    myServo.write(0);
    //digitalWrite(LED_G, LOW);
  }

  else   {
    Serial.println(" Access denied");
    //digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(1000);
    //digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }

    if(digitalRead (IR1) == LOW && flag1==0){
if(Slot>0){flag1=1;
if(flag2==0){myServo.write(0); Slot = Slot-1;}
}
//else{
//lcd.setCursor (0,0);
//lcd.print("  SORRY 🙁  ");
//lcd.setCursor (0,1);
//lcd.print(" Parking Full ");
//delay (3000);
//lcd.clear();
//}
}
if(digitalRead (IR2) == LOW && flag2==0){flag2=1;
if(flag1==0){myServo.write(0); Slot = Slot+1;}
}
if(flag1==1 && flag2==1){
delay (1000);
myServo.write(100);
flag1=0, flag2=0;
}
//lcd.setCursor (0,0);
//lcd.print("  WELCOME!  ");
//lcd.setCursor (0,1);
//lcd.print("Slot Left: ");
//lcd.print(Slot);
}

 
