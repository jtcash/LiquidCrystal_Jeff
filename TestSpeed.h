#include "Arduino.h"
#include "LC_Jeff.h"
#include "LC_Opt.h"

#define REPS 50
#define TEST_MAX_RANDOM 10000  // The max random value

#define CUST_LCD_CLASS LC_Opt

//"testSimplePrint", "testSimplePrint", "testLongPrint", "testLongPrint", "testCompound", "testCompound", "testRandom", "testRandom", "testFilledRand", "testFilledRandKeep", "testDummyRandom"
/*
testSimplePrint
testLongPrint
testCompound
testRandom
testRandom
testFilledRand
testFilledRandKeep
testDummyRandom

*/

//template<typename Def, typename Opt>
class TestSpeed {
public:
//  TestSpeed(Def def, Opt opt) : def(def), opt(opt) { };
  TestSpeed() { }
#define NUM_TESTS 11
//#define DO_TEST_DEF(t, l) testNames[defi]=#t; defResults[defi++]=t(l)
//#define DO_TEST_OPT(t, l) optResults[opti++]=t(l); 


//Make it show the speedups stats, then start optimizing the actual files.

#define PRINT_SPEEDUP(d,o) Serial.print('\t'); Serial.print(d); Serial.print('\t'); Serial.print(o), \
Serial.print('\t'); Serial.print((double)d/o, 6); Serial.println();

#define DO_TEST(t) Serial.flush(); testNames[testi]=#t; defResults[testi]=t(def); optResults[testi]=t(opt);  \
Serial.print(#t); Serial.print(F("    "));; Serial.print('\t'); \
PRINT_SPEEDUP(defResults[testi], optResults[testi]);   ++testi;

  template<typename Opt>
  void fuck(Opt &opt){
    opt.printMem();
  }
  template<typename Def, typename Opt>
  void runTest(Def &def, Opt &opt){
    //unsigned long {testSimplePrint, testLongPrint, testCompound, testRandom, testRandom, testFilledRand, testFilledRandKeep, testDummyRandom};

    unsigned long testStart = micros();
//    opt.printMem();
    def.begin(16,2);
    opt.begin(16,2);
    byte testi = 0;
    unsigned long defResults[NUM_TESTS] = {};
    unsigned long optResults[NUM_TESTS] = {};
    const char *testNames[NUM_TESTS] = {};
    

    
    //Serial.println(defResults[0]);
    
    //DO_TEST_DEF(testSimplePrint, def);
    // Run all of the print tests for both lcd libraries
//    opt.printMem();
    DO_TEST(testSimplePrint);
//    opt.printMem();
    DO_TEST(testSimplePrint_P);
//    opt.printMem();
    DO_TEST(testLongPrint);
//    opt.printMem();
    DO_TEST(testLongPrint_P);
//    opt.printMem();
    DO_TEST(testCompound);
//    opt.printMem();
    DO_TEST(testCompound_P);
//    opt.printMem();
    DO_TEST(testRandomNum);
//    opt.printMem();
    DO_TEST(testRandomNum_P);
//    opt.printMem();
    DO_TEST(testFilledRand);
//    opt.printMem();
    DO_TEST(testFilledRandKeep);
//     const char **t = testNames;

   /* 
    while(*t)
      Serial.print(*t++);*/
   // Hide compiler warning
   const char* c = *testNames;
   *testNames = *(testNames+1);
   *testNames = c;

   opt.clear();
   opt.print(F("Finished testing"));
   opt.update();
   Serial.flush();
   
#define BASE_SPEED 8378088ul
   unsigned long testTime = micros() - testStart;
   long testDiff = BASE_SPEED - testTime;
   double change = ((double)testDiff)/(double)BASE_SPEED*100.0;
   double rate = (double)testTime/(double)BASE_SPEED;
   
   Serial.println();
   Serial.print(F("Test took "));
   Serial.print(testTime);
   Serial.print(F("us; "));
   Serial.print(change,4);
   Serial.print(F("% speedup ("));
   Serial.print(testDiff);
   Serial.print(F("us faster, "));
   Serial.print(rate,4);
   Serial.print(F("x the speed of base)"));
   Serial.println();


//   opt.clear();
//   opt.update();
//   opt.printMem();
  }
  
  //Def def;
  //Opt opt;
  //Def lcd;

  template<typename Lcd>
  unsigned long testSimplePrint(Lcd &lcd){
    lcd.clear();
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      lcd.clear();
      lcd.print("TEST");
      lcd.setCursor(0,1);
      lcd.print("TSETEST");
      
    }
    return micros() - t0;
    
  }
  unsigned long testSimplePrint(CUST_LCD_CLASS &lcd){
    lcd.clear();
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      lcd.clear();
      lcd.print("TEST");
      lcd.setCursor(0,1);
      lcd.print("TSETEST");
      lcd.update();
    }
    return micros() - t0;
    
  }
  template<typename Lcd>
  unsigned long testSimplePrint_P(Lcd &lcd){
    lcd.clear();
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      lcd.print(F("TEST"));
      lcd.setCursor(0,1);
      lcd.print(F("TSETEST"));
      
    }
    return micros() - t0;
  }
  unsigned long testSimplePrint_P(CUST_LCD_CLASS &lcd){
    lcd.clear();
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      lcd.print(F("TEST"));
      lcd.setCursor(0,1);
      lcd.print(F("TSETEST"));
      lcd.update();
    }
    return micros() - t0;
  }
  template<typename Lcd>
  unsigned long testLongPrint(Lcd &lcd){
    lcd.clear();
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      //lcd.setCursor(0,0);
      lcd.print("0123456789012345");
      lcd.setCursor(0,1);
      lcd.print("5432109876543210");
  
      
    }
    return micros() - t0;
  }
  unsigned long testLongPrint(CUST_LCD_CLASS &lcd){
    lcd.clear();
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      //lcd.setCursor(0,0);
      lcd.print("0123456789012345");
      lcd.setCursor(0,1);
      lcd.print("5432109876543210");
      lcd.update();
      
    }
    return micros() - t0;
  }
    
  template<typename Lcd>
  unsigned long testLongPrint_P(Lcd &lcd){
    lcd.clear();
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      //lcd.setCursor(0,0);
      lcd.print(F("0123456789012345"));
      lcd.setCursor(0,1);
      lcd.print(F("5432109876543210"));
  
      
    }
    return micros() - t0;
  }
   unsigned long testLongPrint_P(CUST_LCD_CLASS &lcd){
    lcd.clear();
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      //lcd.setCursor(0,0);
      lcd.print(F("0123456789012345"));
      lcd.setCursor(0,1);
      lcd.print(F("5432109876543210"));
      lcd.update();
      
    }
    return micros() - t0;
  }

  template<typename Lcd>
  unsigned long testCompound(Lcd &lcd){
    lcd.clear();
    unsigned long t0 = micros();
    unsigned int i;
    int int0 = 10;
    int int1 = 9876;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      lcd.print("integer0=");
      lcd.print(int0);
      lcd.setCursor(0,1);
      lcd.print("integer1=");
      lcd.print(int1);
      
    }
    return micros() - t0;
  }
  unsigned long testCompound(CUST_LCD_CLASS &lcd){
    lcd.clear();
    unsigned long t0 = micros();
    unsigned int i;
    int int0 = 10;
    int int1 = 9876;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      lcd.print("integer0=");
      lcd.print(int0);
      lcd.setCursor(0,1);
      lcd.print("integer1=");
      lcd.print(int1);
      lcd.update();
    }
    return micros() - t0;
  }
  template<typename Lcd>
  unsigned long testCompound_P(Lcd &lcd){
    lcd.clear();
    unsigned long t0 = micros();
    unsigned int i;
    int int0 = 10;
    int int1 = 9876;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      lcd.print(F("integer0="));
      lcd.print(int0);
      lcd.setCursor(0,1);
      lcd.print(F("integer1="));
      lcd.print(int1);
      
    }
    return micros() - t0;
  }
  unsigned long testCompound_P(CUST_LCD_CLASS &lcd){
    lcd.clear();
    unsigned long t0 = micros();
    unsigned int i;
    int int0 = 10;
    int int1 = 9876;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      lcd.print(F("integer0="));
      lcd.print(int0);
      lcd.setCursor(0,1);
      lcd.print(F("integer1="));
      lcd.print(int1);
      lcd.update();
    }
    return micros() - t0;
  }

  template<typename Lcd>
  unsigned long testRandomNum(Lcd &lcd){
    lcd.clear();
    randomSeed(0);
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      lcd.print("Rand0=");
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.setCursor(0,1);
      lcd.print("Rand1=");
      lcd.print(random(TEST_MAX_RANDOM));
      
    }
    return micros() - t0;
  }

  unsigned long testRandomNum(CUST_LCD_CLASS &lcd){
    lcd.clear();
    randomSeed(0);
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      lcd.print("Rand0=");
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.setCursor(0,1);
      lcd.print("Rand1=");
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.update();
    }
    return micros() - t0;
  }
  template<typename Lcd>
  unsigned long testRandomNum_P(Lcd &lcd){
    lcd.clear();
    randomSeed(0);
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      lcd.print(F("Rand0="));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.setCursor(0,1);
      lcd.print(F("Rand1="));
      lcd.print(random(TEST_MAX_RANDOM));
      
    }
    return micros() - t0;
  }
  unsigned long testRandomNum_P(CUST_LCD_CLASS &lcd){
    lcd.clear();
    randomSeed(0);
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
      lcd.print(F("Rand0="));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.setCursor(0,1);
      lcd.print(F("Rand1="));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.update();
    }
    return micros() - t0;
  }

  template<typename Lcd>
  unsigned long testFilledRand(Lcd &lcd){
    lcd.clear();
    randomSeed(0);
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
     
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      
      lcd.setCursor(0,1);
  
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      
    }
    return micros() - t0;
  }
  unsigned long testFilledRand(CUST_LCD_CLASS &lcd){
    lcd.clear();
    randomSeed(0);
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.clear();
     
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      
      lcd.setCursor(0,1);
  
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.update();
    }
    return micros() - t0;
  }

  template<typename Lcd>
  unsigned long testFilledRandKeep(Lcd &lcd){
    lcd.clear();
    randomSeed(0);
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.setCursor(0,0);
     
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      
      lcd.setCursor(0,1);
  
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      
    }
    return micros() - t0;
  }
   unsigned long testFilledRandKeep(CUST_LCD_CLASS &lcd){
    lcd.clear();
    randomSeed(0);
    unsigned long t0 = micros();
    unsigned int i;
    for(i=0; i<REPS; ++i){
      
      lcd.setCursor(0,0);
     
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      
      lcd.setCursor(0,1);
  
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.print(random(TEST_MAX_RANDOM));
      lcd.update();
    }
    return micros() - t0;
  }

  unsigned long testDummyRandom(){
    randomSeed(0);
    unsigned long t0 = micros();
    unsigned int i;
    unsigned long temp;
    for(i=0; i<REPS; ++i){
      temp += random(TEST_MAX_RANDOM);
      temp += random(TEST_MAX_RANDOM);
    }
    return micros() - t0;
  }
};
