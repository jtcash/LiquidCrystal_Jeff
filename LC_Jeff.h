#ifndef LC_Jeff_h
#define LC_Jeff_h

#include "LiquidCrystal.h"
#include "Arduino.h"
#include "Printable.h"

// Made for a 16x2 LCD Display


//#include <string.h>

// Simplify the container to just be a container class, and add the LCD member functions one by one while also adding
// optimized versions.


//figure out what to do with virtual write

#define LCD_WIDTH 16
#define LCD_HEIGHT 2

#define INITIALIZE_LIST initialized(false), cols(0), rows(0), cursorX(0), cursorY(0)
#define WARN_UNIMPLEMENTED() \
Serial.print(F("WARNING: called unimplemented function:\n\t"));\
Serial.println(__PRETTY_FUNCTION__)

class LC_Jeff {
public:

  ~LC_Jeff(){
    for(int i=0; i<rows; ++i){
      delete[] lines[i];
      delete[] slines[i];
    }
    delete[] lines;
    delete[] slines;
  }
  
  LC_Jeff(uint8_t rs, uint8_t enable,
    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) :
      lcd(rs, enable, d0, d1, d2, d3, d4, d5, d6, d7), INITIALIZE_LIST {
    setup();
    initialize();
  }
    
  LC_Jeff(uint8_t rs, uint8_t rw, uint8_t enable,
    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) :
      lcd(rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7), INITIALIZE_LIST {
    initialize();
  }
    
  LC_Jeff(uint8_t rs, uint8_t rw, uint8_t enable,
    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) :
      lcd(rs, rw, enable, d0, d1, d2, d3), INITIALIZE_LIST {
    initialize();
  }
  LC_Jeff() : lcd(12, 11, 5, 4, 3, 2), INITIALIZE_LIST {
    initialize();
  }
  LC_Jeff(uint8_t rs, uint8_t enable,
    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) :
      lcd(rs, enable, d0, d1, d2, d3), INITIALIZE_LIST {
    initialize();
  }

//////////////////////////////////////////////////////////////////////////////////
  void initialize(){
    // Will later set the timer to update the screen n times/second
  }


#define printArray(x, n) Serial.print(F(#x)); Serial.print(F(":\t"));\
for(int i=0; i<n-1; ++i) { Serial.print((byte)x[i]); Serial.print(','); }\
Serial.println((byte)x[n-1]);
  
  void begin(uint8_t col, uint8_t row, uint8_t charsize = LCD_5x8DOTS){
    lcd.begin(col, row, charsize);
    if(initialized){
      // If it's already intitialized, delete the line arrays
      //if the cols or rows differ, and then reallocate them
      if(cols != col || rows != row){
        for(int i=0; i<LCD_HEIGHT; ++i){
          delete[] lines[i];
          delete[] slines[i];
        }
        delete[] lines;
        delete[] slines;
        
      }
    } else {
      lines = new char*[row];
      slines = new char*[row];
      for(int i=0; i<row; ++i){
        lines[i] = new char[col];
        slines[i] = new char[col];
      }
      initialized = true;
    }

    // Clear the line arrays
    //printArray(lines[0], width); printArray(lines[1], width);
    for(int i=0; i<row; ++i){
      memset(lines[i], ' ', col);
      memset(slines[i], ' ', col);
    }
      
    //printArray(lines[0], col); printArray(lines[1], col);
    //printArray(slines[0], col); printArray(slines[1], col);
    cursorX = 0;
    cursorY = 0;
    cols = col;
    rows = row;
  }




  inline void clear(){ WARN_UNIMPLEMENTED(); lcd.clear(); }

  inline void home(){ WARN_UNIMPLEMENTED(); lcd.home(); }

  
  inline void noDisplay(){ WARN_UNIMPLEMENTED(); lcd.noDisplay(); }

  inline void display(){ WARN_UNIMPLEMENTED(); lcd.display(); }

  inline void noBlink(){ WARN_UNIMPLEMENTED(); lcd.noBlink(); }

  inline void blink(){ WARN_UNIMPLEMENTED(); lcd.blink(); }

  inline void noCursor(){ WARN_UNIMPLEMENTED(); lcd.noCursor(); }

  inline void cursor(){ WARN_UNIMPLEMENTED(); lcd.cursor(); }

  inline void scrollDisplayLeft(){ WARN_UNIMPLEMENTED(); lcd.scrollDisplayLeft(); }

  inline void scrollDisplayRight(){ WARN_UNIMPLEMENTED(); lcd.scrollDisplayRight(); }

  inline void leftToRight(){ WARN_UNIMPLEMENTED(); lcd.leftToRight(); }

  inline void rightToLeft(){ WARN_UNIMPLEMENTED(); lcd.rightToLeft(); }

  inline void autoscroll(){ WARN_UNIMPLEMENTED(); lcd.autoscroll(); }

  inline void noAutoscroll(){ WARN_UNIMPLEMENTED(); lcd.noAutoscroll(); }


  inline void setRowOffsets(int row1, int row2, int row3, int row4){ WARN_UNIMPLEMENTED(); lcd.setRowOffsets(row1, row2, row3, row4); }

  inline void createChar(uint8_t location, uint8_t charmap[]){ WARN_UNIMPLEMENTED(); lcd.createChar(location, charmap); }

  inline void setCursor(uint8_t col, uint8_t row){ 
    if(col != cursorX || row != cursorY){
      lcd.setCursor(col, row);
      cursorX = col;
      cursorY = row;
    }
  }

  inline void command(uint8_t value){ WARN_UNIMPLEMENTED(); lcd.command(value); }



////////// Modify all prints to use this same formula. // Optimize this formula as well
  size_t print(const __FlashStringHelper *ifsh){
    PGM_P p = reinterpret_cast<PGM_P>(ifsh); // PGM_P is a macro for const char *
    //PGM_P pOrig = p;
    //size_t n = 0;
    char *line = lines[cursorY] + cursorX;
    uint8_t remain = cols - cursorX;
    uint8_t remainOld = remain;
    while (remain) {
      unsigned char c = pgm_read_byte(p);
      if (c == 0) break;
      *line++ = c;
      ++p;
      --remain;
    }
    size_t n = remainOld - remain;
    cursorX += remainOld - remain;
    return n;    
  }
  
  inline size_t print(const char str[]){ // same as const char str*
    char *line = lines[cursorY] + cursorX;
    uint8_t remain = cols - cursorX;
    uint8_t remainOld = remain;
    while(remain){
      if(*str == 0) break;
      *line++ = *str++;
      --remain;
    }
    size_t n = remainOld - remain;
    cursorX += remainOld - remain;
    return n;
  }
  inline size_t print(char *str){
    return print((const char*) str);
  }
  inline size_t print(const String &s){
    //return write(s.c_str(), s.length());
    const char *cstr = s.c_str();
    uint8_t n = 0; // TODO: Check if I even need to count the number of bytes written
    // And if i can assume that write will always return 1;
    while(*cstr) // TODO: See if this is an optimal way to iterate.
      n += write(*cstr++); //the compiler should be able to fully optimize this
    //WARN_UNIMPLEMENTED();
    cursorX += n;
    return n;
  }
  
  inline size_t print(char c){
    if(cursorX >= cols) return 0;
    lines[cursorY][cursorX++] = c;
    return 1;
//  if(cursorX < cols){ lines[cursorY][cursorX++] = c; return 1; } return 0;
  }
  inline size_t print(char c, int base){
    if(c<0)
      return print('-') + print((unsigned char)c*-1, base);
    return print((unsigned char)c,base);
  }

  
  inline size_t print(unsigned char n, int base = DEC){
    if(base < 2) return write(n);// Used to be ==0, but changed it to <2 for simplicity
    char buf[8*sizeof(unsigned char) + 1];
    char *str = buf + sizeof(buf) - 1;
    *str = '\0';
    // Write the digits of n in base base to buf using str
    do {
      unsigned char m = n;
      n /= base;
      char c = m - base*n;
      *--str = (c<10) ? c+'0' : c+'A'-10;
    } while(n);
    
    return print(str);
  }
  
  inline size_t print(unsigned int n, int base = DEC){
    if(base < 2) return (uint8_t)n; // Shitty fallback behavior, slightly
    // different than the default implementation's fallbacks
    char buf[8*sizeof(unsigned int) + 1];
    char *str = buf + sizeof(buf) - 1;
    *str = '\0';

    do {
      unsigned int m = n;
      n /= base;
      char c = m - base*n; // get the last digit of n;
      *--str = c < 10 ? c + '0' : c + 'A' - 10; // write it into str
    } while(n);

    return print(str);
  }
  inline size_t print(int n, int base = DEC) {
    if(n<0)
      return print('-') + print( (unsigned int)n*-1, base );
    return print( (unsigned int)n, base );
  }

  inline size_t print(unsigned long n, int base = DEC){
    if(base < 2) return (uint8_t)n; // Shitty fallback behavior, slightly
    // different than the default implementation's fallbacks
    char buf[8*sizeof(unsigned long) + 1];
    char *str = buf + sizeof(buf) - 1;
    *str = '\0';

    do {
      unsigned long m = n;
      n /= base;
      char c = m - base*n; // get the last digit of n;
      *--str = c < 10 ? c + '0' : c + 'A' - 10; // write it into str
    } while(n);

    return print(str);
  }
  inline size_t print(long n, int base = DEC) {
    if(n<0)
      return print('-') + print( (unsigned int)n*-1, base );
    return print( (unsigned int)n, base );
  }
  /*
  
  
  
  size_t print(long, int = DEC);
  size_t print(unsigned long, int = DEC);
  size_t print(double, int = 2);
  size_t print(const Printable_O&);
  */




  template<typename T>
  inline void print(T data){
    WARN_UNIMPLEMENTED();
    lcd.print(data);
  }

  template<typename T, typename D>
  inline void print(T data, D data2){
    WARN_UNIMPLEMENTED();
    lcd.print(data, data2);
  }



/// So calling write on individual characters is faster.
// TODO: Implement unimplemented print functions and optimize the write const char * function
  void test0(){
    lcd.clear();
    const char* testStr = "0123456789012345";
    lcd.write(testStr);
  }
  void test1(){
    lcd.clear();
    const char *testStr = "0123456789012345";
    const char *tstr = testStr;
    while(*tstr)
      lcd.write(*tstr++);
  }
  void testWriteSpeed(){
    int i;
    #define TEST_REPS 100
    unsigned long t0 = micros();
    for(i=0;i<TEST_REPS;++i)
      test0();

    unsigned long t1 = micros();
    for(i=0;i<TEST_REPS;++i)
      test1();

    unsigned long t2 = micros();
    Serial.print(F("testWriteSpeed\t"));
    Serial.print(t1 - t0);
    Serial.print('\t');
    Serial.print(t2 - t1);
  }


  
  // For now, only use the write functions for direct writing, 
  //do not change cursorX unless it seems necessary
  inline size_t write(char c){
    // Figure out where lcd's write function is defined
    return lcd.write(c);
  }
  inline size_t write(uint8_t c){
    // Figure out where lcd's write function is defined
    return lcd.write(c);
  }
  
  inline size_t write(const char *str) {
    if(str == NULL) return 0;
    uint8_t n = 0; // It seems unnecessary to use a size_t here, 
    //unless you plan on writing >256 chars
    // TODO: See if it's necessary to guard this so it won't write after
    //char c;

    while(*str)
      n += write(*str++);
      //if (str == NULL) return 0;
      //return write((const uint8_t *)str, strlen(str));
    //cursorX += n;
    return n;
  }
  
  
  template<typename T>
  inline size_t write(T data){
    WARN_UNIMPLEMENTED();
    //uint8_t n = lcd.write(data);
    //cursorX += n;
    //return n;
    return lcd.write(data);
  }
  template<typename T, typename D>
  inline size_t write(T data, D data2){
    WARN_UNIMPLEMENTED();
//    size_t n = lcd.write(data, data2);
//    cursorX += n;
//    return n;
    return lcd.write(data, data2);
  }
  /*
  inline size_t write(uint8_t data){
    return lcd.write(data);
  }*/
  /*
  
  
  
  size_t print(unsigned char, int = DEC);
  size_t print(int, int = DEC);
  size_t print(unsigned int, int = DEC);
  size_t print(long, int = DEC);
  size_t print(unsigned long, int = DEC);
  size_t print(double, int = 2);
  size_t print(const Printable_O&);

  size_t println(const __FlashStringHelper *);
  size_t println(const String &s);
  size_t println(const char[]);
  size_t println(char);
  size_t println(unsigned char, int = DEC);
  size_t println(int, int = DEC);
  size_t println(unsigned int, int = DEC);
  size_t println(long, int = DEC);
  size_t println(unsigned long, int = DEC);
  size_t println(double, int = 2);
  size_t println(const Printable_O&);
  size_t println(void);
*/


  
  inline void testPrint(const char *str){
    char *line = lines[cursorY];
    uint8_t i = cursorX;
    while(i<cols){
      if(*str == 0) break;
      line[i++] = *str++;
    }
    cursorX = i;
    printArray(lines[cursorY], cols);
    printArray(slines[cursorY], cols);
  }
  // updates the LCD to show changes since last update. Needs to be called manually
  inline void update(){
    for(uint8_t y=0; y<rows; ++y){
      char *line = lines[y];
      char *sline = slines[y];
      for(uint8_t x=0; x<cols; ++x){
        if(sline[x] != line[x]){
          setCursor(x,y);
          write(line[x]);
          sline[x] = line[x];
        }
      }
    }
  }

  inline void clearLineArrays(){
    for(uint8_t y=0; y<rows; ++y){
      memset(slines[y], ' ', cols);
      memset(lines[y], ' ', cols);
    }
  }

  
  LiquidCrystal *getLcdPtr(){
    return &lcd;
  }

  void serialDebug(){
    Serial.print(cursorX); Serial.print(','); Serial.println(cursorY);
  }
private:
  LiquidCrystal lcd;
  char **lines;
  char **slines; // The lines currently on the screen
  bool initialized;// True if the container has been initialized
  uint8_t cols;    // Number of cols in LCD
  uint8_t rows;    // Number of rows in LCD
  uint8_t cursorX; // Current x location of LCD cursor
  uint8_t cursorY; // Current y location of LCD cursor

  // Now implement all of the LiquidCrystal methods as inlines and then start optimizing
    
};

#endif
