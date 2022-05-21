#include <virtuabotixRTC.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>


virtuabotixRTC RTCSaat(6, 7, 8);

LiquidCrystal_I2C lcd(0x27,16,2); 

const int trigPin = 9;
const int echoPin = 2;


int sensor = 4;
int buzzer1 = 5;
int ledpin = 3;
int hours,minute,second,day,month,year;
long mesafe;
int mesafeCm;
File dataFile;

void setup() {


lcd.init();
lcd.backlight();

//RTCSaat.setDS1302Time(0,44,13,4,11,05,2022);
analogReference(INTERNAL);
pinMode (buzzer1, OUTPUT);
pinMode (ledpin, OUTPUT);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(sensor, INPUT);
Serial.begin(9600);  
while (!Serial)
    ; // wait for serial port to connect. Needed for native USB port only
  Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  delay(2000); 

}
uint16_t line = 1;

void loop() {
delay(1000);
RTCSaat.updateTime(); //Bu kod ile RTC deki anlık zamanı alıyoruz.
hours=RTCSaat.hours;
minute=RTCSaat.minutes;
second=RTCSaat.seconds;
day=RTCSaat.dayofmonth;
month=RTCSaat.month;
year=RTCSaat.year;
 
char date[10]; 
sprintf (date,"%02d/%02d/%02d",day,month,year);
char zaman[9]; 
sprintf (zaman,"%02d:%02d:%02d",hours,minute,second);
 
// Tarih ve Saat bilgisini LCD ekrana yazdırıyoruz.
lcd.setCursor(0,0);
lcd.print("Tarih:");
lcd.setCursor(6,0);
lcd.print(date);
lcd.setCursor(0,1);
lcd.print("Saat:");
lcd.setCursor(5,1);
lcd.print(zaman);

 
delay( 2000);// 1 saniye bekletiyoruz.
lcd.clear();
 
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
mesafe = pulseIn(echoPin, HIGH);
mesafeCm = (mesafe / 2) / 29.1;
Serial.print("Mesafe = ");
Serial.print(mesafeCm);
Serial.println("cm");
lcd.setCursor(0,0);

if (mesafeCm < 5) {
  lcd.print("Carpma Tehlikesi!");
  tone(buzzer1, 200);
  digitalWrite(ledpin, HIGH);
  delay(1000);
}
else if (mesafeCm < 10) {
  lcd.print("Mesafe: ");
  lcd.print(mesafeCm);
  lcd.print("cm");
  tone(buzzer1, 400);
  digitalWrite(ledpin, HIGH);
  delay(1000);
  }
else if (mesafeCm < 15) {
   lcd.print("Mesafe: ");
  lcd.print(mesafeCm);
  lcd.print("cm");
  tone(buzzer1, 600);
  digitalWrite(ledpin, HIGH);
  delay(200);
  noTone(buzzer1);
digitalWrite(ledpin,LOW);
delay(1000);
}
else if (mesafeCm <= 20) {
  lcd.print("Mesafe: ");
  lcd.print(mesafeCm);
  lcd.print("cm");
  tone(buzzer1, 400);
  digitalWrite(ledpin, HIGH);
  delay(250);
  digitalWrite(ledpin,LOW);
  delay(1000);
   }


else if (mesafeCm > 20) {
  noTone(buzzer1);
  digitalWrite(ledpin, LOW);
  lcd.println("Engel Yok");
  delay(1000);
   }
lcd.clear();

int Yagmur = digitalRead(sensor); 
  
  if(Yagmur == HIGH){
    lcd.clear(); 
    lcd.print("Not: "); 
    lcd.setCursor(0, 1); 
    lcd.print("Raining"); 
    Serial.println("It is not raining"); 
  }
  
  if(Yagmur == LOW){
    lcd.clear(); 
    lcd.print("It: "); 
    lcd.setCursor(0, 1); 
    lcd.print("rains"); 
    Serial.println("It is raining"); //Seri Ekranımıza "Yağmur Yağıyor" yazısını yazdırıyoruz
  }
   
   delay(1000); 
   dataFile = SD.open("pandr.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (dataFile) {
    
    Serial.print(line);
    Serial.print(":    Mesafe = ");
    Serial.print(mesafeCm);
    Serial.print(",    Yagmur = ");
    Serial.print(Yagmur);
    Serial.print(",    Gun = ");
    Serial.print(day);
    Serial.print(",    Ay = ");
    Serial.print(month);
    Serial.print(",    Yil = ");
    Serial.print(year);
    Serial.print(",    Saat = ");
    Serial.print(hours);
    Serial.print(":,    Dakika = ");
    Serial.print(minute);
   
    // Write data to SD card file (DHT11Log.txt)
    dataFile.print(line++);
    dataFile.print(":    Mesafe = ");
    dataFile.print(mesafeCm);
    dataFile.print(",    Yagmur = ");
    dataFile.print(Yagmur);
    dataFile.print(",    Gun = ");
    dataFile.print(day);
    dataFile.print(",    Ay = ");
    dataFile.print(month);
    dataFile.print(",    Yil = ");
    dataFile.print(year);
    dataFile.print(",    Saat = ");
    dataFile.print(hours);
    dataFile.print(",    Dakika = ");
    dataFile.print(minute);
   
    dataFile.close();
  }
  // if the file didn't open, print an error:
  else
    Serial.println("error opening DHT11Log.txt");
}
