/* LedHexClock --- display a four-digit clock             2018-02-18 */
/* Copyright (c) 2018 John Honniball                                 */

#define NDIGITS    (4)   // This program uses a four-digit display

// Pin connections from LED board to Arduino board
// 1 VLED +5V (LED power, OK to use Arduino +5V for a single digit)
// 2 VCC  +5V (TLC5916 chip power)
// 3 SDI  D11 (Serial Data In on TLC5916)
// 4 SDO  no connection (Serial Data out on TLC5916, unused)
// 5 LE   D10 (Latch Enable on TLC5916)
// 6 OE   GND (Output Enable, unused in this test, GND to enable)
// 7 SCK  D13 (Serial Clock to TLC5916)
// 8 GND  GND (Ground to both LEDs and chip)
//
#define LE_PIN 10   // Arduino digital pin 10
#define SDA_PIN 11  // Arduino digital pin 11
#define SCK_PIN 13  // Arduino digital pin 13
/* OE pin grounded */

// 'F' is #defined as a macro by Arduino framework. Use an int instead
const int F = (1 << 14);

// Usual 7-segment names
#define A  (1 << 8)
#define B  (1 << 0)
#define C  (1 << 2)
#define D  (1 << 4)
#define E  (1 << 11)
//#define F  (1 << 14)
#define G  (1 << 10)
#define DP (1 << 12)

// Additional dot names
#define H (1 << 1)
#define I (1 << 3)
#define J (1 << 9)
#define K (1 << 6)
#define L (1 << 15)
#define M (1 << 13)
#define COLON (1 << 5)

// LED layout on PCB:
//
//     H C C I
//     B     D
//     B     D
//     J G G K
//     A     E
//     A     E
//     L F F M DP

// Table of 14-segment digits 0-9 and A-F
const unsigned int HDSPsegtab[16] = {
  A | B | C | D | E | F | J | K,         // 0
  D | E | I | K | M,                     // 1
  A | C | D | F | G | H | L | M,         // 2
  C | D | E | F | G | H | L,             // 3
  B | D | E | G | H | J | K | M,         // 4
  B | C | E | F | G | H | I | J | L,     // 5
  A | B | C | E | F | G | J,             // 6
  C | D | E | H | I | K | M,             // 7
  A | B | C | D | E | F | G,             // 8
  B | C | D | E | F | G | K,             // 9
  A | B | C | D | E | G | J | K | L | M, // A
  A | B | C | D | E | F | G | H | J | L, // B
  A | B | C | F | I | J | M,             // C
  A | B | C | D | E | F | H | J | K | L, // D
  A | B | C | F | G | H | I | J | L | M, // E
  A | B | C | G | H | I | J | L          // F
};


void setup(void)
{
  pinMode(LE_PIN, OUTPUT);
  pinMode(SDA_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  
  digitalWrite(LE_PIN, LOW);
  digitalWrite(SCK_PIN, LOW);
}


void loop(void)
{
  int d;
  unsigned int hours = 12, minutes = 0;
  unsigned int frames = 0;
  char timstr[8];
  unsigned int digit;
  unsigned int segs[NDIGITS];           // 16-bit unsigned binary

  for (;;) {
    snprintf(timstr, sizeof (timstr), "%02d%02d", hours, minutes);
    
    for (d = 0; d < NDIGITS; d++) {
      digit = timstr[d] - '0';
        
      segs[d] = HDSPsegtab[digit];   // Pick up segment pattern from table
    }

    if (frames > 25)
      segs[1] |= COLON;

    LedHex_send(segs, NDIGITS);     // Send to displays
    
    minutes++;

    if (minutes >= 60) {
      minutes = 0;
      hours++;

      if (hours >= 24)
        hours = 0;
    }

    frames++;
    if (frames >= 50)
      frames = 0;

    delay(20);                      // Update at 50Hz
  }
}


/* LedHex_send --- send 'ndig' 16-bit patterns to the LED driver chip */

void LedHex_send(const unsigned int leds[], const int ndig)
{
  // Straightforward implementation using digitalWrite(). Could be made
  // faster by using direct port I/O or SPI hardware.
  int i;
  int d;
  
  digitalWrite(LE_PIN, LOW);
  digitalWrite(SCK_PIN, LOW);

  // Send 16 bits to each pair of TLC5916 chips per digit
  for (d = 0; d < ndig; d++) {
    for (i = 0; i < 16; i++) {
      // Send one bit on SDA pin
      if (leds[d] & (1u << i))
        digitalWrite(SDA_PIN, HIGH);
      else
        digitalWrite(SDA_PIN, LOW);
      
      delayMicroseconds(1);

      // One microsecond HIGH pulse on SCK to clock the bit along
      digitalWrite(SCK_PIN, HIGH);
      
      delayMicroseconds(1);
      
      digitalWrite(SCK_PIN, LOW);  
    }
  }

  // One microsecond HIGH pulse on LE to latch all bits into all the digits
  digitalWrite(LE_PIN, HIGH);
  
  delayMicroseconds(1);
    
  digitalWrite(LE_PIN, LOW);
}

