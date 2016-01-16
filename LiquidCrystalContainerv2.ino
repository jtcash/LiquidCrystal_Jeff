
#include <LiquidCrystal.h>
//#include "LiquidCrystal_O.h"
#include "TestSpeed.h"
#include "LC_Jeff.h"
#include "LC_Opt.h"
#define LCD_PINS 12, 11, 5, 4, 3, 2


#define SERIAL_BAUD 230400
//#define LCD_PINS 22, 23, 24, 25, 26, 27
//////////////////////////////////////////////
//MAKE A CLASS THAT IS JUST3 AN EXTENTION OF THE LIQUIDCRYSTAL CLASS, AND OVERRIDE/HIDES SOME OF THE FUNCTIONS 
///////////////////////////////////////////////
LiquidCrystal lcd_old(LCD_PINS);
//LiquidCrystal_O lcd(LCD_PINS);
//LC_Jeff lcd(LCD_PINS);
LC_Opt lcd(LCD_PINS);


TestSpeed tester;
//LCDisplay display(LCD_PINS);

void testOpt(){
//  LC_Opt lcd(LCD_PINS);

  lcd.printMem();
  
  lcd.write('l');
  lcd.write('o');
  lcd.write('z');
  lcd.printMem();
  lcd.update();
  delay(1000);
  lcd.printMem();
  lcd.setCursor(0,1);
  lcd.write('j');
  lcd.update();
  delay(1000);
  lcd.print(F("TEST"));
  lcd.print("0123");
  lcd.update();
  delay(500);
  
  lcd.print("WTF");
  lcd.printMem();
  lcd.update();
  lcd.printMem();
  
  delay(1000);
  lcd.clear();
  lcd.update();

  

//  lcd.printMem();
}

#define TEST_TIME(f) Serial.flush(); t0=micros(); f; t1=micros(); Serial.print(F(#f));\
Serial.print(F(":\t")); Serial.print(t1-t0); Serial.println(F(" us"))
void testUpdate(){
  
  unsigned long t0,t1;
  TEST_TIME(lcd.update());
  TEST_TIME(lcd.print(F("0123456789012345")));
  lcd.setCursor(0,1);
  lcd.print(F("0123456789012345"));
  TEST_TIME(lcd.update());
  TEST_TIME(lcd.update());
  lcd.printMem();
  lcd.update();

  
  
}
void testClear(){
  lcd.printMem();
  lcd.print(F("FUCKFUCKFUCK"));
  lcd.setCursor(0,1);
  lcd.print(F("01234566"));
  lcd.printMem();
  lcd.update();
  lcd.printMem();
  delay(1000);
  lcd.clear();
  lcd.update();
  lcd.printMem();
  
  
}
void timeUpdate(){
  unsigned long t0,t1;
  uint8_t reps = 255;
  t0=micros();
  do{
    lcd.print(F("0123456789012345"));
    lcd.setCursor(0,1);
    lcd.print(F("0123456789012345"));
    lcd.update();
    lcd.print(F("1234567890123456"));
    lcd.setCursor(0,1);
    lcd.print(F("1234567890123456"));
    lcd.update();
  }while(--reps);
  t1=micros();

  Serial.print(t1-t0); Serial.print(F(",\t"));
  Serial.flush();
  lcd.print(F("0123456789012345"));
  t0=micros();
  do{
    lcd.print(F("0123456789012345"));
    lcd.setCursor(0,1);
    lcd.print(F("0123456789012345"));
    lcd.update();
    lcd.print(F("0123456789012345"));
    lcd.setCursor(0,1);
    lcd.print(F("0123456789012345"));
    lcd.update();
  }while(--reps);
  t1=micros();

  Serial.println(t1-t0);

  
}
void setup() {
  Serial.begin(SERIAL_BAUD);

//  testClear();
//  testUpdate();
//  testOpt();
//  lcd.printMem();
//  tester.fuck(lcd);
    timeUpdate();
//  tester.runTest(lcd_old, lcd);
//  lcd.clear();
//  lcd.update();
  
  //#define TESTY loltest
  // put your setup code here, to run once
  //lcd.begin(16,2);
  //lcd.print(F("TEST"));
  //lcd.write("test");  
  //testPrintNumbers();
  //basicTest();
  
 /* lcd.begin(16, 2);
  lcd.clear();
  lcd.print("HI");
  delay(1000);
  lcd_old.begin(16, 2);
  lcd_old.clear();
  lcd_old.print("HI2");*/
  
}
#define pl() Serial.print(F("Line : "));Serial.println(__LINE__); delay(10);
void testPrintNumbers() {
  printMem();
  LC_Jeff lcd(LCD_PINS);
  lcd.begin(16,2);
  lcd.print('A');
  lcd.print(' ');
  lcd.serialDebug();
  lcd.print('A', 10);
  lcd.serialDebug();
  lcd.print(' ');
  lcd.print((unsigned char)'A', 10);
  lcd.print(' '); 
  lcd.print(-1234, 16);

  lcd.setCursor(0,1);
  lcd.print(-0.5678);
  lcd.print(' ');
  lcd.print(12.2345,0);

  //lcd.write(" test");
  
  
  pl();
  lcd.update();
  delay(1000);
  lcd.setCursor(4,0);
  lcd.println(F("JEFF"));
  lcd.update();
  lcd.clear();
//  pl();
//  printMem();
//  pl();
  
  lcd.enableScroll();
//  pl();
//  delay(100);
//  printMem();
//  delay(100);
  lcd.setCursor(14,0);
  lcd.print(F("0123456"));
  lcd.update();
  lcd.scrollDisplayLeft();
  delay(500);
  lcd.scrollDisplayLeft();
  delay(500);
  lcd.scrollDisplayLeft();
  delay(500);
  lcd.scrollDisplayLeft();
  delay(500);
  lcd.scrollDisplayLeft();
  delay(500);
  lcd.scrollDisplayLeft();
  delay(500);
  lcd.home();
}
void basicTest(){
  
  LC_Jeff lcd(LCD_PINS);
  
  lcd.begin(16,2);
  //lcd.testWriteSpeed();
  lcd.print(F("TEST45678901234567"));
  lcd.setCursor(0,1);
  lcd.print("LOL");
  lcd.print(F("_tt"));
  lcd.print('_');
  Serial.println(lcd.write(1234567));
  lcd.print('_');
  lcd.print((unsigned char)'A', 10);
  lcd.print(12345u, 10);
  //lcd.print("lol2");
  //lcd.print(F("_two"));
  delay(1000);
  //lcd.testPrint("LOLOL");
  lcd.update();
  //lcd.print("POOP");
}
void printMem(){
  int avail = availableMemory();
  Serial.print(F("Available Memory: "));
  Serial.print(avail);
  Serial.println(F("bytes"));
  delay(10);
}
int availableMemory() {
  int size = 2048; // Use 2048 with ATmega328
  byte *buf;
  while ((buf = (byte *) malloc(--size)) == NULL);
  free(buf);
  return size;
}
void loop() {
  // put your main code here, to run repeatedly:

}
