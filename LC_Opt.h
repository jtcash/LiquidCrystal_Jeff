#ifndef LC_Opt_h
#define LC_Opt_h

/* A liquid crystal display driving class optimized for HD44780, KS0066
 *  compatible 16x2 LCDs
 * 
 * 
 */

#include "Arduino.h"
#include "LiquidCrystal.h"
#include "Print.h"
#include "Printable.h"

#define LC_USE_ATTRIBUTES false



#define LC_COLS 16
#define LC_ROWS 2
#define LC_RAM_BYTES 80
#define LC_ROW_BYTES (LC_RAM_BYTES/LC_ROWS)
#define LC_ARR_BYTES (LC_RAM_BYTES*2)
#define LC_FRAME_GAP (LC_ROW_BYTES-LC_COLS) // Distance in between row frames

#define LC_BLANK_CHAR ' '

#define pvar(x) Serial.print(F(#x)); Serial.print(F(" = ")); Serial.println(x)

#define LC_INITIALIZE_LIST sarr(arr+LC_RAM_BYTES), \
cur(arr), scur(sarr), \
cursorX(0), \
fr0(arr), fr1(arr+LC_ROW_BYTES)
//fr0(0), fr1(LC_ROW_BYTES)

class LC_Opt : Print {
public:
  LC_Opt(uint8_t rs, uint8_t enable,
    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) :
      lcd(rs, enable, d0, d1, d2, d3), LC_INITIALIZE_LIST {

    init();       
  }
  inline void init(){
    lcd.begin(LC_COLS, LC_ROWS);
    beginSetup();
    testVals();
    
  }
  
  inline void beginSetup(){
    // Wipe the array
    memset(arr, LC_BLANK_CHAR, LC_ARR_BYTES);
    
  }
  inline void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS){
    if(cols != LC_COLS || rows != LC_ROWS){
      Serial.println(F("Can only initialize displays with 16x2"));
      return;
    }
    lcd.begin(cols, rows, charsize);
    beginSetup();
  }

  inline void clear(){ 
    memset(arr, LC_BLANK_CHAR, LC_RAM_BYTES);
    setCursor(0);
    
  }
  inline void home(){ 
    setCursor(0);
  }








  inline size_t write(uint8_t n){
    return (size_t)write((char)n);
  }
#if LC_USE_ATTRIBUTES
  __attribute__((always_inline, hot))
#endif
  inline uint8_t write(char c){
    *cur++ = c;
    if(cur == sarr) // Loop back to start if end is reached
      cur = arr;
    return 1; // Of course it will succeed
  }


  inline uint8_t write(const char *str) {
    if(str == NULL) return 0;
    uint8_t n = 0;
    char c;
    while( (c=*str++) ){
      n += write(c);
    }
    return n;
  }

  inline size_t write(const uint8_t *buffer, size_t size){
    uint8_t len = (uint8_t)size;
    uint8_t n = 0;
    do{
      n += write(*buffer++);
    }while(--len);
    return n;
  }

//  inline char comp(char c, char *sptr){
//    if(c == *sptr){
//      return 0;
//    }
//    *sptr = c;
//    return c;
//  }
  // Update the LCD
#if LC_USE_ATTRIBUTES
  __attribute((hot))
#endif
  inline void updateFrame(char *fr){
    char *ptr = fr;
    uint8_t len = LC_COLS;
    bool lastWritten = false;
    do{
      //char c = comp(*ptr, ptr+LC_RAM_BYTES);
      char c = *ptr;
      char *sptr = ptr+LC_RAM_BYTES;
      if( c != *sptr ){
        // Need to write to the display
        // Check if cursorX is up to date
        *sptr = c;
        if(!lastWritten){
          // cursorX is not up to date, so update it to line up
          setCursor(ptr-arr);
          lastWritten = true;
        }
        // Now write the char to the display
        
        lcd.write(c);
        
      } else {
        lastWritten = false;
      }
      ++ptr;
    }while(--len);
  }
#if LC_USE_ATTRIBUTES
  __attribute__((flatten, hot))
#endif
  inline void update() {
//    char *saveCur = cur;
    uint8_t saveCursorX = cursorX;
    
    updateFrame(fr0);
    updateFrame(fr1);
    
//    cur = saveCur;
    cursorX = saveCursorX;
    cur = arr+cursorX;
    
  }

  
  // Compare arr to sarr, write to display only if 
//  uint8_t compWrite(char *&ptr, char*&sptr){
//    if(++cursorX > LC_RAM_BYTES) // Move forward/loop back cursorX
//      cursorX = 0;
//      
//    if(*ptr == *sptr)
//      return 0;
//      
//    *sptr = *ptr;
//    lcd.write(*ptr);
//    
//    return 1;
//  }


/////////////////////


  inline void setCursor(uint8_t x, uint8_t y){
    setCursor(x+y*LC_ROW_BYTES);
  }
#if LC_USE_ATTRIBUTES
  __attribute__((hot))
#endif
  inline void setCursor(uint8_t x) {
    if(x != cursorX){
      if(x > LC_RAM_BYTES)
        x = x%LC_RAM_BYTES; // Maybe can optimize this to just subtract LC_RAM_BYTES and trust input
      cursorX = x;
      cur = arr+x;
      lcd.setCursor(x,0);
    }
  }




//// COPIED PRINT FUNCTIONS
  inline uint8_t print(const __FlashStringHelper *ifsh){
    PGM_P p = reinterpret_cast<PGM_P>(ifsh); // PGM_P is a macro for const char *
    unsigned char c;// = pgm_read_byte(p);
    PGM_P p0 = p;
//    uint8_t n = 0;
    while( (c = pgm_read_byte(p++)) ){
//      n += write(c);
      write(c);
    }
//    return n;
    return (uint8_t)(p-p0-1);
  }
  inline uint8_t print(const char str[]){// same as const char str*
    unsigned char c;// = pgm_read_byte(p);
    uint8_t n = 0;
    while( (c = *str++) ){
      n += write(c);
    }
    return n;
  }
  /*
  inline uint8_t print(const char str[]){// same as const char str*
    unsigned char c;// = pgm_read_byte(p);
    uint8_t n = 0;
    while( (c = *str++) ){
      n += write(c);
    }
    return n;
  }*/
  inline uint8_t print(char *str){
    return print((const char*) str);
  }
  inline uint8_t print(const String &s){
    // For now, just be lazy and use the print(const char*)
    // Later, maybe use the size() function to optimize
    return print(s.c_str());
  }
  
  inline uint8_t print(char c){
    return write(c);
  }
  
  inline uint8_t print(char c, int base){
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


  // NO printlns here





//////////////////////////////////////////////////



  ///

  void testVals(){
    pvar(LC_COLS);
    pvar(LC_ROWS);
    pvar(LC_RAM_BYTES);
    pvar(LC_ROW_BYTES);
    pvar(LC_ARR_BYTES);
    pvar(LC_FRAME_GAP);
    pvar(LC_BLANK_CHAR);
  }

#define spl(x) Serial.println(x)
#define sp(x) Serial.print(x)
#define splf(x) Serial.println(F(x));
#define spf(x) Serial.print(F(x));
#define spp(x) printPtr(#x, x);
  void printMem(){

    splf("\nLC_Opt's memory state:");
    spf("idx:\t");
    for(uint8_t i=0; i<LC_ARR_BYTES; ++i){
      sp('|');
      spPad(i,2);
    }
    splf("|\n");

    spf("arr:\t");
    for(uint8_t i=0; i<LC_ARR_BYTES; ++i){
      spf("|");
      spChar(arr[i]);
    }
    spl('|');
    
    spp(sarr);
    spp(cur);
    spp(scur);
    spp(fr0);
    spp(fr1);
    spf("cursorX = ");
    spl(cursorX);
  }
  void spPad(uint8_t n, uint8_t len, char padding = '0'){
    char ca[len+1];
    ca[len] = 0;
    char *back = ca+len-1;
    while(back >= ca){
      if(n!=0){
        *back-- = '0'+n%10;
        n/=10;
      } else {
        *back-- = padding;
      }
    }
    Serial.print(ca);
    
  }
  void spChar(char c){
    uint8_t n = (uint8_t)c;
    if(n<0x10)
      Serial.print(' ');
    Serial.print(n,16);
  }


  void printPtr(const char *str, char *ptr){
    Serial.print('\t');
    uint8_t len = ptr - arr+1;
    while(--len){
      Serial.print(F("   "));
    }
    Serial.print('^');
    Serial.println(str);
  }
private:
  LiquidCrystal lcd;
  char arr[LC_ARR_BYTES];
  char * const sarr; // pointer to the live part

  char *cur; // The current place being written to in the arr
  char *scur; // The current place being written to in sarr

  uint8_t cursorX;
  //uint8_t x; // The cursor location, only using x, not y. Use the wraparound to your advantage
  // Offset of the current frames for lcd lines 0 and 1
  char *fr0;
  char *fr1;

  


  
};





#endif
