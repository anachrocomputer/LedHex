/* LedHexTest --- test the TI TLC5916 with a few LEDs    2014-08-29 */

#define LE_PIN 10
#define SDA_PIN 11
#define SCK_PIN 13
/* OE pin grounded */

#define NDIGITS  (4)

const int F = (1 << 14);

// Usual 7-segment names for SAA1064
#define A  (1 << 8)
#define B  (1 << 0)
#define C  (1 << 2)
#define D  (1 << 4)
#define E  (1 << 11)
//#define F  (1 << 14)
#define G  (1 << 10)
#define DP (1 << 12)

// Additional dot names for HDSP0761
#define H (1 << 1)
#define I (1 << 3)
#define J (1 << 9)
#define K (1 << 6)
#define L (1 << 15)
#define M (1 << 13)
#define COLON (1 << 5)

//     H C C I
//     B     D
//     B     D
//     J G G K
//     A     E
//     A     E
//     L F F M

const unsigned int TestSeq[] = {
  B, H, C, I, D, K, E, M, F, L, A, J, G, COLON, DP
};

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
  pinMode (LE_PIN, OUTPUT);
  pinMode (SDA_PIN, OUTPUT);
  pinMode (SCK_PIN, OUTPUT);
  
  digitalWrite (LE_PIN, LOW);
  digitalWrite (SCK_PIN, LOW);
}

void loop (void)
{
  const int NTESTS = sizeof (TestSeq) / sizeof (TestSeq[0]);
  int i, j;
  int d;
  unsigned int digit[NDIGITS];
  unsigned int segs;
 
  for (i = 0; i < NTESTS; i++) { 
    segs = TestSeq[i];
    
    for (d = 0; d < NDIGITS; d++)
      digit[d] = segs;
      
    LedHex_send (digit, NDIGITS);
    
    delay (1000);
  }
}

void LedHex_send (unsigned int leds[], int ndig)
{
  int i;
  int d;
  
  digitalWrite (LE_PIN, LOW);
  digitalWrite (SCK_PIN, LOW);
  
  for (d = 0; d < ndig; d++) {
    for (i = 0; i < 16; i++) {
      if (leds[d] & (1u << i))
        digitalWrite (SDA_PIN, HIGH);
      else
        digitalWrite (SDA_PIN, LOW);
      
      delayMicroseconds (1);
      
      digitalWrite (SCK_PIN, HIGH);
      
      delayMicroseconds (1);
      
      digitalWrite (SCK_PIN, LOW);  
    }
  }
  
  digitalWrite (LE_PIN, HIGH);
  
  delayMicroseconds (1);
    
  digitalWrite (LE_PIN, LOW);
}

