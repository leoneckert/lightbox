
// This is a demonstration on how to use an input device to trigger changes on your neo pixels.
// You should wire a momentary push button to connect from ground to a digital IO pin.  When you
// press the button it will change to a new pixel animation.  Note that you need to press the
// button once to start the first animation!

//#include <Adafruit_NeoPixel.h>
#include "FastLED.h"


bool state_changed = 0;

#define BUTTON1_PIN   4    // Digital IO pin connected to the button.  This will be
bool button1_state = 0;
bool button1_state_old = 0;
bool button1_changed = 0;

#define BUTTON2_PIN   3    // Digital IO pin connected to the button.  This will be
bool button2_state = 0;
bool button2_state_old = 0;
bool button2_changed = 0;

#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.

#define ROTARY_PIN    A0
int rotary_state = 0;
int old_rotary_state = 0;
int rotary_smooth_threshold = 200; // the higher the smoother.
int rotary_smoother = rotary_smooth_threshold;
bool rotary_changed = false;

#define POT_PIN    A1
int min_a_read = 665;
int max_a_read = 995;
float pot_state = 0.0;
float old_pot_state = 0.0;
int pot_smooth_threshold = 50; // the higher the smoother.
int pot_smoother = pot_smooth_threshold;
bool pot_changed = false;





#define PIXEL_COUNT 30

CRGB leds[PIXEL_COUNT];



//MODE variables:

// m0

int m0hsvselector = 0;
int m0hsvvalues[] = {255, 255, 255};
int m0softanimationtoggle = -1;

int m0animationstage = 0;


long int m0animationtime = 0;
long int m0animationspeed = 50;
long int m0animationdelays[] = {10, 50, 500, 1000, 5000, 7000, 10000, 15000, 20000, 40000, 60000, 120000, 400000};

long int m0animationdelay = 0;
int m0groups[10][3] = {
  {0,  19,  20 },
  {1,  18,  21  },
  {2,  17,  22  },
  {3,  16,  23  },
  {4,  15,  24  },
  {5,  14,  25  },
  {6,  13,  26  },
  {7,  12,  27  },
  {8,  11,  28  },
  {9,  10,  29  }
};
int m0animtationdirection = 1;
bool m0animationchangemade = false;




void setup() {
  Serial.begin(9600);
  pinMode(BUTTON1_PIN, INPUT);
  button1_state = button1_state_old = digitalRead(BUTTON1_PIN);

  pinMode(BUTTON2_PIN, INPUT);
  button2_state = button2_state_old = digitalRead(BUTTON2_PIN);

  pinMode(POT_PIN, INPUT);
  pinMode(ROTARY_PIN, INPUT);

  delay(3000);
  //
  //  strip.begin();
  //  strip.show(); // Initialize all pixels to 'off'
  FastLED.addLeds<NEOPIXEL, 6>(leds, PIXEL_COUNT);



  //MODE setups:
  // m0

}

void loop() {

  // BUTTON1 STATE:
  button1_state = digitalRead(BUTTON1_PIN);
  if ( button1_state != button1_state_old) {
    delay(20);
    button1_state = digitalRead(BUTTON1_PIN);
    if ( button1_state != button1_state_old) {
      Serial.print("BTN1    ");
      Serial.print(button1_state_old);
      Serial.print("    -->    ");
      Serial.println(button1_state);
      button1_state_old = button1_state;
      button1_changed = true;
      state_changed = true;
    }
  }

  // BUTTON2 STATE:
  button2_state = digitalRead(BUTTON2_PIN);
  if ( button2_state != button2_state_old) {
    delay(20);
    button2_state = digitalRead(BUTTON2_PIN);
    if ( button2_state != button2_state_old) {
      Serial.print("BTN2    ");
      Serial.print(button2_state_old);
      Serial.print("    -->    ");
      Serial.println(button2_state);
      button2_state_old = button2_state;
      button2_changed = true;
      state_changed = true;
    }
  }



  //POT STATE:
  int pot_val = analogRead(POT_PIN);  // min value: 575 / max value: 870
  int pot_range = map(pot_val, min_a_read, max_a_read, 0, 109);
  int new_pot_range = 0;
  for (int i = 0; i <= 20; i++) {
    int v = i * 5;
    if (pot_range > v) {
      new_pot_range = v;
    }
  }
  float pot_state = float(new_pot_range) / 100;
  if (pot_state != old_pot_state) {
    pot_smoother -= 1;
    if (pot_smoother <= 0) {
      Serial.print("POT   ");
      Serial.print(old_pot_state);
      Serial.print("   -->   ");
      Serial.println(pot_state);
      old_pot_state = pot_state;
      pot_changed = true;
      state_changed = true;
    }
  } else {
    pot_smoother = pot_smooth_threshold;
  }

  //  ROTARY STATE:
  int rotary_val = analogRead(ROTARY_PIN);

  //    Serial.println(rotary_val);
  //  Serial.print(" ROTARY: ");
  if (rotary_val < 370) {
    rotary_state = 0;
  } else if (rotary_val < 430) {
    rotary_state = 1;
  } else if (rotary_val < 510) {
    rotary_state = 2;
  } else if (rotary_val < 615) {
    rotary_state = 3;
  } else if (rotary_val < 750) {
    rotary_state = 4;
  } else {
    rotary_state = 5;
  }


  if (rotary_state != old_rotary_state) {
    rotary_smoother -= 1;
    if (rotary_smoother <= 0) {

      Serial.print("ROTARY   ");
      Serial.print(old_rotary_state);
      Serial.print("   -->   ");
      Serial.println(rotary_state);
      old_rotary_state = rotary_state;
      rotary_changed = true;
      state_changed = true;
    }
  } else {
    rotary_smoother = rotary_smooth_threshold;
  }




  ////////////////////////////BELOW HERE HAPPENS THE ACTUAL MAGIC:





  if (state_changed == true) {

    if (old_rotary_state == 0) {

      //      MODE ZERO

      //      LET SELECT ONE COLOR AND ADJUST BRIGHTNESS OF THAT COLOR
      //      ONE BUTTON SWTICHES HSV IDX,
      //      POT CHANGES THE VALUES
      //      THE OTHER BUTTON ADDS SOFT SAT and brightness noise
      Serial.println("MODE: 0");
      if (button1_changed == true) {
        m0hsvselector += 1;
        if (m0hsvselector > 2) {
          m0hsvselector = 0;
        }
      }
      if (button2_changed == true) {
        m0softanimationtoggle *= -1;
      }
      if (pot_changed) {
        m0hsvvalues[m0hsvselector] = old_pot_state * 255;

      }

      if (m0softanimationtoggle == 1) {
        for (uint16_t i = 0; i < PIXEL_COUNT; i++) {
          leds[i] = CHSV( m0hsvvalues[0], m0hsvvalues[1], m0hsvvalues[2]);
        }
        FastLED.show();
        delay(30);
      }else{
        m0animationchangemade = true;
      }





      //

    } else if (old_rotary_state == 1) {

      //      MODE ONE
      Serial.println("MODE: 1");



      //

    } else if (old_rotary_state == 2) {

      //      MODE TWO
      Serial.println("MODE: 2");



      //

    } else if (old_rotary_state == 3) {

      //      MODE THREE
      Serial.println("MODE: 3");



      //

    } else if (old_rotary_state == 4) {

      //      MODE FOUR
      Serial.println("MODE: 4");



      //

    } else if (old_rotary_state == 5) {

      //      MODE FIVE
      Serial.println("MODE: 5");



      //

    }


    pot_changed = false;
    button1_changed = false;
    button2_changed = false;
    state_changed = false;

  }

  // Here comes stuff that happens every loop:

  if (old_rotary_state == 0) {
    // MODE 0, everyloop
    if (m0animationchangemade == true || (m0softanimationtoggle == -1 && millis() - m0animationtime > m0animationspeed && millis() - m0animationtime > m0animationdelay )) {
      for (uint16_t i = 0; i < PIXEL_COUNT; i++) {
        leds[i] = CHSV( 0, 0, 0);
      }
      for (int i = 0; i < 3; i++) {
        int prepreprepre = m0animationstage - 4;
        int preprepre = m0animationstage - 3;
        int prepre = m0animationstage - 2;
        int pre = m0animationstage - 1;
        int post = m0animationstage + 1;
        int postpost = m0animationstage + 2;
        int postpostpost = m0animationstage + 3;
        int postpostpostpost = m0animationstage + 4;
        leds[ m0groups[m0animationstage][i] ] = CHSV( m0hsvvalues[0], m0hsvvalues[1], m0hsvvalues[2]);

        if (prepreprepre >= 0) {
          leds[ m0groups[prepreprepre][i] ] = CHSV( m0hsvvalues[0], m0hsvvalues[1], m0hsvvalues[2]);
          leds[ m0groups[prepreprepre][i] ].fadeLightBy( 250 );
        }
        if (preprepre >= 0) {
          leds[ m0groups[preprepre][i] ] = CHSV( m0hsvvalues[0], m0hsvvalues[1], m0hsvvalues[2]);
          leds[ m0groups[preprepre][i] ].fadeLightBy( 240 );
        }
        if (prepre >= 0) {
          leds[ m0groups[prepre][i] ] = CHSV( m0hsvvalues[0], m0hsvvalues[1], m0hsvvalues[2]);
          leds[ m0groups[prepre][i] ].fadeLightBy( 230 );
        }
        if (pre >= 0) {
          leds[ m0groups[pre][i] ] = CHSV( m0hsvvalues[0], m0hsvvalues[1], m0hsvvalues[2]);
          leds[ m0groups[pre][i] ].fadeLightBy( 200 );
        }
        if (post <= 9) {
          leds[ m0groups[post][i] ] = CHSV( m0hsvvalues[0], m0hsvvalues[1], m0hsvvalues[2]);
          leds[ m0groups[post][i] ].fadeLightBy( 200 );
        }
        if (postpost <= 9) {
          leds[ m0groups[postpost][i] ] = CHSV( m0hsvvalues[0], m0hsvvalues[1], m0hsvvalues[2]);
          leds[ m0groups[postpost][i] ].fadeLightBy( 230 );
        }
        if (postpostpost <= 9) {
          leds[ m0groups[postpostpost][i] ] = CHSV( m0hsvvalues[0], m0hsvvalues[1], m0hsvvalues[2]);
          leds[ m0groups[postpostpost][i] ].fadeLightBy( 240 );
        }
        if (postpostpostpost <= 9) {
          leds[ m0groups[postpostpostpost][i] ] = CHSV( m0hsvvalues[0], m0hsvvalues[1], m0hsvvalues[2]);
          leds[ m0groups[postpostpostpost][i] ].fadeLightBy( 250 );
        }

      }

      FastLED.show();
      delay(30);
      m0animationtime = millis();
      m0animationdelay = 0;
      m0animationchangemade = false;
      
      m0animationstage += m0animtationdirection;
      
      if (m0animationstage > 9 || m0animationstage < 0) {
        m0animationstage -= m0animtationdirection;
        m0animtationdirection *= -1;
        int numDelays = sizeof(m0animationdelays) / sizeof(long int) ;
        m0animationdelay = m0animationdelays[ int(random(0,numDelays)) ];
      }

      

      
    }
  }


}
