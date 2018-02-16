/* LedHexTest --- test the LED hex display board          2014-08-29 */
/* Copyright (c) 2014 John Honniball                                 */

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


void setup (void)
{
  // Set Arduino's pins to outputs
  pinMode (LE_PIN, OUTPUT);
  pinMode (SDA_PIN, OUTPUT);
  pinMode (SCK_PIN, OUTPUT);
    
  digitalWrite (LE_PIN, LOW);
  digitalWrite (SCK_PIN, LOW);
}


void loop (void)
{
  int i, j;
  unsigned int segs;       // 16-bit unsigned binary
 
  for (i = 0; i < 16; i++) { 
    segs = HDSPsegtab[i];  // Pick up segment pattern from table
    LedHex_send (segs);    // Send to display
    delay (1000);          // Wait one second
  }
}


/* LedHex_send --- send a 16-bit pattern to the LED driver chip */

void LedHex_send (const unsigned int leds)
{
  // Straightforward implementation using digitalWrite(). Could be made
  // faster by using direct port I/O or SPI hardware.
  int i;
  
  digitalWrite (LE_PIN, LOW);
  digitalWrite (SCK_PIN, LOW);
  
  // Send 16 bits, enough for both TLC5916 chips
  for (i = 0; i < 16; i++) {
    // Send one bit on SDA pin
    if (leds & (1u << i))
      digitalWrite (SDA_PIN, HIGH);
    else
      digitalWrite (SDA_PIN, LOW);
    
    delayMicroseconds (1);
    
    // One microsecond pulse on SCK to clock the bit along
    digitalWrite (SCK_PIN, HIGH);
    
    delayMicroseconds (1);
    
    digitalWrite (SCK_PIN, LOW);  
  }
  
  // One microsecond pulse on LE to latch all 16 bits
  digitalWrite (LE_PIN, HIGH);
  
  delayMicroseconds (1);
    
  digitalWrite (LE_PIN, LOW);
}

