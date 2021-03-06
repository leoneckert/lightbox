
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

// color

int colorselectstage = 0;
int currentColor = 0;
int colorselectview = 0;

bool colorselectgroups[][30] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1,  1, 1, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 1, 1,},

  {0, 0, 0, 0, 1, 1, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 1, 1, 0, 0, 0, 0,  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,},

  {1, 1, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {1, 1, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 1, 1,  1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},

};
long int colortimer = millis();
int randomcolorinterval = 400;
bool colorselectchange = false;

int colors[5][4] = {
  {255, 0, 255, 0},
  {255, 255, 255, 0},
  {160, 255, 255, 0}
};



// m0

//int m0hsvselector = 0;
//int m0hsvvalues[] = {255, 255, 255};
int m0formationstage = 0;
bool m0staticformations[][30] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},

  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 1, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 1, 1, 0, 0, 0, 0,  0, 0, 0, 1, 1, 1, 1, 0, 0, 0,  0, 0, 0, 0, 1, 1, 0, 0, 0, 0,},

  {1, 1, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 1, 1,  1, 1, 0, 0, 0, 0, 0, 0, 0, 0,},
  {1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 0, 0, 0, 0, 0,},

  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1,  1, 1, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 1, 1,},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 1, 1, 1, 1, 1,}

};
bool m1extrastage = false;
long int m1cubicwavetime = 0;
long int m1cubicwaveindex = 0;

long int m1cubicwavecurrentdelay = 100;




// m1

int m1hsvselector = 0;
int m1hsvvalues[] = {255, 255, 255};
int m1softanimationtoggle = -1;
int m1animationstage = 0;
long int m1animationtime = 0;
long int m1animationspeed = 50;
long int m1animationdelays[] = {10, 50, 500, 1000, 5000, 7000, 10000, 15000, 20000, 40000, 60000, 120000, 400000};
long int m1animationdelay = 0;
int m1groups[10][3] = {
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
int m1animtationdirection = 1;
bool m1animationchangemade = false;




void setup() {
  Serial.begin(9600);
  pinMode(BUTTON1_PIN, INPUT);
  button1_state = button1_state_old = digitalRead(BUTTON1_PIN);

  pinMode(BUTTON2_PIN, INPUT);
  button2_state = button2_state_old = digitalRead(BUTTON2_PIN);

  pinMode(POT_PIN, INPUT);
  pinMode(ROTARY_PIN, INPUT);

  delay(5000);
  //
  //  strip.begin();
  //  strip.show(); // Initialize all pixels to 'off'
  FastLED.addLeds<NEOPIXEL, 6>(leds, PIXEL_COUNT);

  delay(5000);



  //MODE setups:
  // m1

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


      // COLOR:


      Serial.println("MODE: COLOR");
      if (button1_changed == true) {
        colorselectstage += 1;
        if (colorselectstage > 3) {
          colorselectstage = 0;
          currentColor += 1;
          if (currentColor > 2) {
            currentColor = 0;
          }
        }
        colorselectchange = true;
      }
      if (button2_changed == true) {
        colorselectview += 1;
        if (colorselectview > 2) {
          colorselectview = 0;
        }

        colorselectchange = true;
      }
      if (pot_changed) {
        //        m0hsvvalues[m0hsvselector] = old_pot_state * 255;
        colors[currentColor][colorselectstage] = old_pot_state * 255;
        if (colorselectstage == 3) {
          if (old_pot_state < 0.5) {
            colors[currentColor][colorselectstage] = 0;
          } else {
            colors[currentColor][colorselectstage] = 1;
          }
        }
        colorselectchange = true;

      }








      //

    } else if (old_rotary_state == 1) {

      int numFormations = sizeof(m0staticformations) / sizeof(bool[30]);

      //      MODE TWO
      Serial.println("MODE: 1");

      if (button1_changed == true) {
        m0formationstage += 1;
        if (m0formationstage > numFormations + 3 - 1) {
          m0formationstage = 0;
        }
      }

      if (button2_changed == true) {
        currentColor += 1;
        if (currentColor > 2) {
          currentColor = 0;
        }
      }

      if (pot_changed) {
        colors[currentColor][2] = old_pot_state * 255;
        m1cubicwavecurrentdelay = map(old_pot_state, 0, 1, 0, 400);
      }

      if ( m0formationstage < numFormations) {
        m1extrastage = false;
        for (uint16_t i = 0; i < PIXEL_COUNT; i++) {
          if (m0staticformations[m0formationstage][i] == 0) {
            leds[i] = CHSV(0, 0, 0);
          } else {
            leds[i] = CHSV( colors[currentColor][0], colors[currentColor][1], colors[currentColor][2]);

          }

        }
        FastLED.show();
        delay(30);
      } else {
        m1extrastage = true;
        Serial.print("extra stage ");
        Serial.println(abs(numFormations - m0formationstage));

      }


    } else if (old_rotary_state == 2) {


      //      MODE ONE
      Serial.println("MODE: 1");

      if (button1_changed == true) {
        m1hsvselector += 1;
        if (m1hsvselector > 2) {
          m1hsvselector = 0;
        }
      }
      if (button2_changed == true) {
        m1softanimationtoggle *= -1;
      }
      if (pot_changed) {
        m1hsvvalues[m1hsvselector] = old_pot_state * 255;
      }

      //      if (m1softanimationtoggle == 1) {
      //        for (uint16_t i = 0; i < PIXEL_COUNT; i++) {
      //          leds[i] = CHSV(0, 0, 0);
      //          //          leds[i] = CHSV( m1hsvvalues[0], m1hsvvalues[1], m1hsvvalues[2]);
      //        }
      //        leds[14] = CHSV( m1hsvvalues[0], m1hsvvalues[1], m1hsvvalues[2]);
      //        leds[15] = CHSV( m1hsvvalues[0], m1hsvvalues[1], m1hsvvalues[2]);
      //      } else {
      m1animationchangemade = true;
      //      }



      //


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
  if (old_rotary_state == 0 ) {
    if (millis() - colortimer > randomcolorinterval || colorselectchange == true) {
      colorselectchange = false;


      for (uint16_t j = 0; j < PIXEL_COUNT; j++) {
        leds[j] = CHSV(0, 0, 0);

      }

      //      if (colorselectfullscreen == false) {
      if (colorselectview == 0) {
        for (uint16_t i = 0; i < 6; i += 2) {
          int randomcolor = int(random(0, 256));
          for (uint16_t j = 0; j < PIXEL_COUNT; j++) {
            //            if ( (i / 2 != currentColor && colorselectgroups[i][j] == 0) || ( i / 2 == currentColor && colorselectgroups[i + 1][j] == 0 )) {
            //            } else {

            if ( (i / 2 != currentColor && colorselectgroups[i][j] == 1) || ( i / 2 == currentColor && colorselectgroups[i + 1][j] == 1 )) {

              if ( (colors[i / 2][3] == 1 && i / 2 != currentColor) || (i / 2 == currentColor && colorselectstage > 2 &&  colors[i / 2][3] == 1 ) ) {

                leds[j] = CHSV( randomcolor, randomcolor, colors[i / 2][2]);
              } else {
                leds[j] = CHSV( colors[i / 2][0], colors[i / 2][1], colors[i / 2][2]);

              }

            }
          }
        }
      } else if (colorselectview == 2) {
        for (uint16_t i = 0; i < 6; i += 2) {
          int randomcolor = int(random(0, 256));
          for (uint16_t j = 0; j < PIXEL_COUNT; j++) {
            if ( colorselectgroups[i + 1][j] == 1 ) {

              if ( (colors[i / 2 ][3] == 1 ) ) {

                leds[j] = CHSV( randomcolor, randomcolor, colors[i / 2][2]);
              } else {
                leds[j] = CHSV( colors[i / 2][0], colors[i / 2][1], colors[i / 2][2]);

              }

            }
          }
        }

      } else if (colorselectview == 1) {
        int randomcolor = int(random(0, 256));
        for (uint16_t j = 0; j < PIXEL_COUNT; j++) {
          //          if (colors[currentColor][3] == 0) {
          if ( colors[currentColor][3] == 1 && colorselectstage > 2 ) {
            leds[j] = CHSV( randomcolor, randomcolor, colors[currentColor][2]);

          } else {
            leds[j] = CHSV( colors[currentColor][0], colors[currentColor][1], colors[currentColor][2]);

          }

        }
      }

      FastLED.show();
      delay(30);
      colortimer = millis();

    }

  } else if (old_rotary_state == 1) {

    if (m1extrastage == true) {
      int numFormations = sizeof(m0staticformations) / sizeof(bool[30]);
      int stage = abs(numFormations - m0formationstage);


      if (stage == 0) {
        if (millis() - m1cubicwavetime > m1cubicwavecurrentdelay) {
          m1cubicwaveindex += 1;
          colors[currentColor][2]  = map(quadwave8(m1cubicwaveindex), 0, 255, 50, 255);
          Serial.println(colors[currentColor][2]);
          for (uint16_t j = 0; j < PIXEL_COUNT; j++) {
            leds[j] = CHSV( colors[currentColor][0], colors[currentColor][1], colors[currentColor][2]);
          }

          FastLED.show();
          delay(30);
          m1cubicwavetime = millis();
        }

      }




    }



    // MODE 1, everyloop
    //    if (m1animationchangemade == true || (millis() - m1animationtime > m1animationspeed && millis() - m1animationtime > m1animationdelay )) {
    //      for (uint16_t i = 0; i < PIXEL_COUNT; i++) {
    //        leds[i] = CHSV( 0, 0, 0);
    //      }
    //      for (int i = 0; i < 3; i++) {
    //        int prepreprepre = m1animationstage - 4;
    //        int preprepre = m1animationstage - 3;
    //        int prepre = m1animationstage - 2;
    //        int pre = m1animationstage - 1;
    //        int post = m1animationstage + 1;
    //        int postpost = m1animationstage + 2;
    //        int postpostpost = m1animationstage + 3;
    //        int postpostpostpost = m1animationstage + 4;
    //        leds[ m1groups[m1animationstage][i] ] = CHSV( m1hsvvalues[0], m1hsvvalues[1], m1hsvvalues[2]);
    //
    //        if (prepreprepre >= 0) {
    //          leds[ m1groups[prepreprepre][i] ] = CHSV( m1hsvvalues[0], m1hsvvalues[1], m1hsvvalues[2]);
    //          leds[ m1groups[prepreprepre][i] ].fadeLightBy( 250 );
    //        }
    //        if (preprepre >= 0) {
    //          leds[ m1groups[preprepre][i] ] = CHSV( m1hsvvalues[0], m1hsvvalues[1], m1hsvvalues[2]);
    //          leds[ m1groups[preprepre][i] ].fadeLightBy( 240 );
    //        }
    //        if (prepre >= 0) {
    //          leds[ m1groups[prepre][i] ] = CHSV( m1hsvvalues[0], m1hsvvalues[1], m1hsvvalues[2]);
    //          leds[ m1groups[prepre][i] ].fadeLightBy( 230 );
    //        }
    //        if (pre >= 0) {
    //          leds[ m1groups[pre][i] ] = CHSV( m1hsvvalues[0], m1hsvvalues[1], m1hsvvalues[2]);
    //          leds[ m1groups[pre][i] ].fadeLightBy( 200 );
    //        }
    //        if (post <= 9) {
    //          leds[ m1groups[post][i] ] = CHSV( m1hsvvalues[0], m1hsvvalues[1], m1hsvvalues[2]);
    //          leds[ m1groups[post][i] ].fadeLightBy( 200 );
    //        }
    //        if (postpost <= 9) {
    //          leds[ m1groups[postpost][i] ] = CHSV( m1hsvvalues[0], m1hsvvalues[1], m1hsvvalues[2]);
    //          leds[ m1groups[postpost][i] ].fadeLightBy( 230 );
    //        }
    //        if (postpostpost <= 9) {
    //          leds[ m1groups[postpostpost][i] ] = CHSV( m1hsvvalues[0], m1hsvvalues[1], m1hsvvalues[2]);
    //          leds[ m1groups[postpostpost][i] ].fadeLightBy( 240 );
    //        }
    //        if (postpostpostpost <= 9) {
    //          leds[ m1groups[postpostpostpost][i] ] = CHSV( m1hsvvalues[0], m1hsvvalues[1], m1hsvvalues[2]);
    //          leds[ m1groups[postpostpostpost][i] ].fadeLightBy( 250 );
    //        }
    //
    //      }
    //
    //      FastLED.show();
    //      delay(30);
    //      m1animationtime = millis();
    //      m1animationdelay = 0;
    //      m1animationchangemade = false;
    //
    //      m1animationstage += m1animtationdirection;
    //
    //      if (m1animationstage > 9 || m1animationstage < 0) {
    //        m1animationstage -= m1animtationdirection;
    //        m1animtationdirection *= -1;
    //        int numDelays = sizeof(m1animationdelays) / sizeof(long int) ;
    //        m1animationdelay = m1animationdelays[ int(random(0, numDelays)) ];
    //      }
    //    }
  }


}
