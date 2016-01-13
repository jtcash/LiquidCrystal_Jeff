#include "Arduino.h"
#include "LC_Jeff.h"

#define REPS 10
#define TEST_MAX_RANDOM 10000  // The max random value

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

#define DO_TEST(t) testNames[testi]=#t; defResults[testi]=t(def); optResults[testi]=t(opt);  \
Serial.print(#t); Serial.print(F("    "));; Serial.print('\t'); \
PRINT_SPEEDUP(defResults[testi], optResults[testi]);   ++testi;

  template<typename Def, typename Opt>
  void runTest(Def def, Opt opt){
    //unsigned long {testSimplePrint, testLongPrint, testCompound, testRandom, testRandom, testFilledRand, testFilledRandKeep, testDummyRandom};
    
    def.begin(16,2);
    opt.begin(16,2);
    byte testi = 0;
    unsigned long defResults[NUM_TESTS] = {};
    unsigned long optResults[NUM_TESTS] = {};
    const char *testNames[NUM_TESTS] = {};
    

    
    //Serial.println(defResults[0]);
    
    //DO_TEST_DEF(testSimplePrint, def);
    // Run all of the print tests for both lcd libraries
    DO_TEST(testSimplePrint);
    DO_TEST(testSimplePrint_P);
    DO_TEST(testLongPrint);
    DO_TEST(testLongPrint_P);
    DO_TEST(testCompound);
    DO_TEST(testCompound_P);
    DO_TEST(testRandomNum);
    DO_TEST(testRandomNum_P);
    DO_TEST(testFilledRand);
    DO_TEST(testFilledRandKeep);
     const char **t = testNames;

    
    while(*t)
      Serial.println(*t++);

   opt.clear();
   opt.print(F("Finished testing"));
      
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
  unsigned long testSimplePrint(LC_Jeff &lcd){
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
  unsigned long testSimplePrint_P(LC_Jeff &lcd){
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
  unsigned long testLongPrint(LC_Jeff &lcd){
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
   unsigned long testLongPrint_P(LC_Jeff &lcd){
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
  unsigned long testCompound(LC_Jeff &lcd){
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
  unsigned long testCompound_P(LC_Jeff &lcd){
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

  unsigned long testRandomNum(LC_Jeff &lcd){
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
  unsigned long testRandomNum_P(LC_Jeff &lcd){
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
  unsigned long testFilledRand(LC_Jeff &lcd){
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
   unsigned long testFilledRandKeep(LC_Jeff &lcd){
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
