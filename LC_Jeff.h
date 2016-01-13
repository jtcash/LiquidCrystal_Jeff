#ifndef LC_Jeff_h
#define LC_Jeff_h

#include "LiquidCrystal.h"
#include "Arduino.h"
#include "Print.h"
#include "Printable.h"

extern void printMem();
// Made for a 16x2 LCD Display
/* TODO:
 *  - Figure out how to optimally make each line array hold 80 characters
 *  in order to accomodate the ram of the display and match the default implementation  
 *  - Find a way to implement rightToLeft and leftToRight
 *  
 *  - Make scroll left and right change a member variable which holds the current scroll
 *  amount of the screen.
 *  - Implement autoscroll for when scroll is enabled. Should be as simple as calling
 *  scrollDisplayLeft when the cursor is at the right of the screen, but it wont be
 *  that simple in practice.
 *  - Figure out how to optimize for when every digit on the display changes. 
 */

//#include <string.h>

// Simplify the container to just be a container class, and add the LCD member functions one by one while also adding
// optimized versions.


//figure out what to do with virtual write

/* //////////////   NOTES: ///////////////////////
 *  The LiquidCrystal::write(uint8_t) function always returns 0, assuming success. 
 * because of this, I can skip some "+="s and just do "++"s
 */

//#define LCD_WIDTH 16 // No longer needed, now is dynamic
//#define LCD_HEIGHT 2

#define LCD_DDRAM_BYTES 80 // The amount of bytes of ram the LCD Driver has for
// storing characters on the screen. 80 is the default for HD44780, KS0066 and
// compatible LCD drivers

#define pl() Serial.print(F("Line : "));Serial.println(__LINE__); delay(10);


#define INITIALIZE_LIST initialized(false), cols(0), rows(0), cursorX(0), cursorY(0),\
showCursor(false), showBlink(false), notDisplayed(false), scrollTrick(0)
#define WARN_UNIMPLEMENTED() \
Serial.print(F("WARNING: called unimplemented function:\n\t"));\
Serial.println(__PRETTY_FUNCTION__)

class LC_Jeff : Print {
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

#define echo(x) Serial.print('\t'); Serial.print(F(#x)); Serial.print(F(" = "));\
Serial.println(x) 
#define printArray(x, n) Serial.print(F(#x)); Serial.print(F(":\t"));\
for(int i=0; i<n-1; ++i) { Serial.print((byte)x[i]); Serial.print(','); }\
Serial.println((byte)x[n-1]);
  
  void begin(uint8_t col, uint8_t row, uint8_t charsize = LCD_5x8DOTS){
    lcd.begin(col, row, charsize);
    beginSetup(col, row);
  }
#define SLINES_NEW_CHAR '\0'

  // Create the line arrays for a number of cols and rows
  void beginSetup(uint8_t col, uint8_t row){
    if(initialized){
      // If it's already intitialized, delete the line arrays
      //if the cols or rows differ, and then reallocate them
      if(cols != col || rows != row){
        for(int i=0; i<rows; ++i){
          delete[] lines[i];
          delete[] slines[i];
        }
        delete[] lines;
        delete[] slines;
      }
    } else {
      initialized = true;
    }
    lines = new char*[row];
    slines = new char*[row];
    for(int i=0; i<row; ++i){
      lines[i] = new char[col];
      slines[i] = new char[col];
    }
    // Clear the line arrays
    for(int i=0; i<row; ++i){
      memset(lines[i], ' ', col);
      memset(slines[i], SLINES_NEW_CHAR, col);
    }

    cursorX = 0;
    cursorY = 0;
    cols = col;
    rows = row;
  }



  // SHOULD I HAVE IT AUTOUPDATE FOR COMMANDS LIKE CLEAR?
  // Probably not because the whole point is that this container is manual
  //update.
  inline void clear(){ 
    // Clear line arrays and set cursor to home // TODO: Blinking and underline stop too?
    if(scrollTrick)
      lcd.home();
    clearLines();
    setCursor(0,0);
  }

  inline void home(){ 
    // Set cursor to home.
    if(scrollTrick)
      lcd.home();
    setCursor(0,0);
//    WARN_UNIMPLEMENTED(); lcd.home(); 
  }

  
  inline void noDisplay(){ 
    if(!notDisplayed){
      lcd.noDisplay();
      notDisplayed = true;
    }
//    WARN_UNIMPLEMENTED(); lcd.noDisplay(); 
  }

  inline void display(){ 
    if(notDisplayed){
      lcd.display();
      notDisplayed = false;
//      WARN_UNIMPLEMENTED(); lcd.display(); }
    }
  }

  inline void noBlink(){ 
    if(showBlink){
      lcd.noBlink();
      showBlink = false;
//     WARN_UNIMPLEMENTED(); lcd.noBlink(); }
    }
  }

  inline void blink(){ 
    if(!showBlink){
      lcd.blink();
      showBlink = true;
//      WARN_UNIMPLEMENTED(); lcd.blink(); }
    }
  }

  inline void noCursor(){ 
    if(showCursor){
      lcd.noCursor();
      showCursor = false;
//      WARN_UNIMPLEMENTED(); lcd.noCursor(); }
    }
  }

  inline void cursor(){ 
    if(!showCursor){
      lcd.cursor();
      showCursor = true;
//      WARN_UNIMPLEMENTED(); lcd.cursor(); }
    }
  }

  // Scrolling left and right is currently unsupported, but is under development. I just
  // need to find a decent way to deal with the fact that I'd need a total of 160bytes of
  // ram just for the line arrays to implement this. Maybe could make a function to setup
  // for scrolling, which would do something similar to this->begin(40,2), without actually
  // calling the lcd's begin(). That could trick my implementation into supporting scrolling.
  // There should be better ways of doing this, however.
  //
  // Now works after enableScroll has been called, but when scroll is enabled, performance
  // is greatly reduced
  inline void scrollDisplayLeft(){ 
    if(scrollTrick)
      lcd.scrollDisplayLeft();
    else {
      WARN_UNIMPLEMENTED(); /*lcd.scrollDisplayLeft();*/ 
      Serial.println("Scroll is currently disabled");
    }
  }
  inline void scrollDisplayRight(){ 
    
    if(scrollTrick)
      lcd.scrollDisplayRight();
    else {
      WARN_UNIMPLEMENTED(); /*lcd.scrollDisplayRight();*/ 
      Serial.println("Scroll is currently disabled");
    }
  }

  inline void enableScroll(){
    Serial.println(__PRETTY_FUNCTION__);
    uint8_t numCols = LCD_DDRAM_BYTES/rows;
    scrollTrick = numCols - cols;
    beginSetup(numCols, rows);
  }
  inline void disableScroll(){
    if(scrollTrick){
      beginSetup(cols - scrollTrick, rows);
      scrollTrick = 0;
    }
  }

  
  // Changing text direction is currently unsupported, partly because of the strange way
  // the default library handles it. For some reason, if you write chars rightToLeft
  // from (0,0), it will only write over characters [0,7] and never touch [8,39]
  inline void leftToRight(){ WARN_UNIMPLEMENTED(); lcd.leftToRight(); }
  inline void rightToLeft(){ WARN_UNIMPLEMENTED(); lcd.rightToLeft(); }

  inline void autoscroll(){ WARN_UNIMPLEMENTED(); lcd.autoscroll(); }

  inline void noAutoscroll(){ WARN_UNIMPLEMENTED(); lcd.noAutoscroll(); }


  inline void setRowOffsets(int row1, int row2, int row3, int row4){ WARN_UNIMPLEMENTED(); lcd.setRowOffsets(row1, row2, row3, row4); }

  inline void createChar(uint8_t location, uint8_t charmap[]){ WARN_UNIMPLEMENTED(); lcd.createChar(location, charmap); }

  inline void setCursor(uint8_t col, uint8_t row){
    if(col != cursorX || row != cursorY)
      if(col < cols && row < rows){ // See if I can optimize these conditionals
        lcd.setCursor(col, row);
        cursorX = col;
        cursorY = row;
      }
  }

  inline void command(uint8_t value){ WARN_UNIMPLEMENTED(); lcd.command(value); }


/////////////////////////////////////////////////////////////////////////
//////////////////    PRINT    //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

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
    // For now, just be lazy and use the print(const char*)
    // Later, maybe use the size() function to optimize
    return print(s.c_str());
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
    if(base < 2) return 0; // Shitty fallback behavior, different than
    //the default implementation's fallbacks
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
    if(base < 2) return 0; // Shitty fallback behavior, different than
    //the default implementation's fallbacks
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
  
  inline size_t print(long n){
    if(n<0) return print('-') + print((unsigned long)-n);
    return print((unsigned long)n);
  }
  inline size_t print(unsigned long n){
    char buf[8*sizeof(unsigned long) + 1];
    char *str = buf + sizeof(buf) - 1;
    *str = '\0';
    do {
      unsigned long m = n;
      n /= 10;
      char c = m - 10*n; // get the last digit of n;
      *--str = c < 10 ? c + '0' : c + 'A' - 10; // write it into str
    } while(n);
    return print(str);
  }
  inline size_t print(unsigned long n, int base){
    if(base < 2) return 0; // Shitty fallback behavior, different than
    //the default implementation's fallbacks
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
  inline size_t print(long n, int base) {
    if(n<0)
      return print('-') + print( (unsigned long)n*-1, base );
    return print( (unsigned long)n, base );
  }


#define TEST_PRINT_SPLIT true // Use either the ul or uc print() based on digit number
  inline size_t print(double x, int digits = 2){
    size_t n;
    
    if (isnan(x)) return print(F("nan")); // Check float options
    if (isinf(x)) return print(F("inf"));
    if (x > 4294967040.0 || x <-4294967040.0) return print (F("ovf"));  
    // constant determined empirically
    if(x < 0.0){
      print('-'); /// n += print('-');
      n = 1;
      x = -x;
    } else {
      n = 0;
    }
    unsigned long wholes = (unsigned long)x;
    n += print(wholes);
    if(digits < 1) return n; // If no precision, just return here
    
    n += print('.');
    x -= wholes; // Subtract wholes from, so x is just the decimal part;
    unsigned long multiplier = 10; // How much to multiply x to get digits to whole numbers
    for(uint8_t i=1; i<digits; ++i)
      multiplier *= 10; // Crude way to do this, but should be fine for default digits=2
    // Use print(unsigned long) to print the decimal part.
    
#if TEST_PRINT_SPLIT
    // If digits<3, use the unsigned char printer to print the digits
    return n + (digits<3) ? print((unsigned char)(x*multiplier+0.5)) : \
      print((unsigned long)(x*multiplier+0.5));
#else
    return n + print((unsigned long)(x*multiplier+0.5)); 
    // Add 0.5 to round to the nearest digit
#endif
  }

  
  inline size_t print(const Printable& x){
    return x.printTo(*this);
  }

  
  /* Just in case I forgot to implement some of the print functions, here
   * are some template functions to catch any leftovers */
  template<typename T>
  inline size_t print(T data){
    WARN_UNIMPLEMENTED();
    return lcd.print(data);
  }
  template<typename T, typename D>
  inline size_t print(T data, D data2){
    WARN_UNIMPLEMENTED();
    return lcd.print(data, data2);
  }



/////////////////////////////////////////////////////////////////////////
///////////////////   PRINTLN   /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

  /* I will add some new functionality with the println function.
   * using println() will make the corresponding call to print(), but then
   * clear the remaining cells in the line.
   */
  inline size_t println(){
    if(cursorX < cols)
      memset(lines[cursorY]+cursorX, ' ', cols - cursorX);
    return 1;
  }
  template<typename T>
  inline size_t println(T data){
    //WARN_UNIMPLEMENTED();
    size_t n = print(data);
//    char *line = lines[cursorY];
//    for(uint8_t i=cursorX; i<cols; ++i)
//      line[i] = ' ';
    if(cursorX < cols)
      memset(lines[cursorY]+cursorX, ' ', cols - cursorX);
    return n;
  }
  template<typename T, typename D>
  inline size_t println(T data, D data2){
    //WARN_UNIMPLEMENTED();
    size_t n = print(data, data2);
    if(cursorX < cols)
      memset(lines[cursorY]+cursorX, ' ', cols - cursorX);
//    char *line = lines[cursorY];
//    for(uint8_t i=cursorX; i<cols; ++i)
//      line[i] = ' ';
    return n;
  }







/////////////////////////////////////////////////////////////////////////
///////////////////   WRITE   ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
  
  // For now, only use the write functions for direct writing, 
  //do not change cursorX unless it seems necessary
  inline size_t write(char c){
    return lcd.write(c);
  }
  inline size_t write(uint8_t value){
    return lcd.write(value);
  }
  
  inline size_t write(const char *str) {
    if(str == NULL) return 0;
    const char *strOrig = str;
    while(*str)
      write(*str++);
    return (size_t)(str - strOrig);
  }

  inline size_t write(const uint8_t *buffer, size_t size){
    size_t n = 0;
    while(size--) // Code taken from "Print.h"
      n += write(*buffer++);
    return n;
  }
  
  /* Just in case I forgot to implement some of the write functions, here
   * are some template functions to catch any leftovers */
  template<typename T>
  inline size_t write(T data){
    WARN_UNIMPLEMENTED();
    return lcd.write(data);
  }
  template<typename T, typename D>
  inline size_t write(T data, D data2){
    WARN_UNIMPLEMENTED();
    return lcd.write(data, data2);
  }
 

///////////////////////////////////////////////////////////////////////

  

  // updates the LCD to show changes since last update. Needs to be called manually
  inline void update(){ // TODO: Optimize this
    // Iterate through the rows, grabbing the line and sline arrays

    uint8_t oldCursorX = cursorX;
    uint8_t oldCursorY = cursorY;
    for(uint8_t y=0; y<rows; ++y){
      char *line = lines[y];
      char *sline = slines[y];
      // Iterate across the line/sline arrays, updating the screen from lin if there
      // is a difference between the two
      for(uint8_t x=0; x<cols; ++x){
        if(sline[x] != line[x]){
          setCursor(x,y);
          write(line[x]);
          sline[x] = line[x];
        }
      }
    }
    setCursor(oldCursorX, oldCursorY);
  }


  
  inline void clearLines(){
    for(uint8_t y=0; y<rows; ++y)
      memset(lines[y], ' ', cols);
  }
  inline void clearSLines(){
    for(uint8_t y=0; y<rows; ++y)
      memset(slines[y], ' ', cols);
  }
//  inline void clearLineArrays(){
//    for(uint8_t y=0; y<rows; ++y){
//      memset(slines[y], ' ', cols);
//      memset(lines[y], ' ', cols);
//    }
//  }

  
  
  void testPrint(const char *str){
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
  void serialDebug(){
    Serial.print(cursorX); Serial.print(','); Serial.println(cursorY);
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

private:
  LiquidCrystal lcd;
  char **lines;
  char **slines; // The lines currently on the screen
  bool initialized;// True if the container has been initialized
  
  uint8_t cols;    // Number of cols in LCD
  uint8_t rows;    // Number of rows in LCD
  uint8_t cursorX; // Current x location of LCD cursor
  uint8_t cursorY; // Current y location of LCD cursor
  uint8_t maxX;    // The max x position, LCD_DDRAM_BYTES/rows


  bool showCursor;
  bool showBlink;
  bool notDisplayed;

  uint8_t scrollTrick;

  // Now implement all of the LiquidCrystal methods as inlines and then start optimizing
    
};

#endif
