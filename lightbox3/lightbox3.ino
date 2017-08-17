
// This is a demonstration on how to use an input device to trigger changes on your neo pixels.
// You should wire a momentary push button to connect from ground to a digital IO pin.  When you
// press the button it will change to a new pixel animation.  Note that you need to press the
// button once to start the first animation!

#include <Adafruit_NeoPixel.h>

bool state_changed = 0;

#define BUTTON1_PIN   3    // Digital IO pin connected to the button.  This will be
bool button1_state = 0;
bool button1_state_old = 0;
bool button1_changed = 0;

#define BUTTON2_PIN   4    // Digital IO pin connected to the button.  This will be
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

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//NEO_RGB     Pixels are wired for RGB bitstream
//NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick

//
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
//
//
//
//float old_pot_percentage = -0.0;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON1_PIN, INPUT);
  button1_state = button1_state_old = digitalRead(BUTTON1_PIN);

  pinMode(BUTTON2_PIN, INPUT);
  button2_state = button2_state_old = digitalRead(BUTTON2_PIN);

  pinMode(POT_PIN, INPUT);
  pinMode(ROTARY_PIN, INPUT);


  //
  strip.begin();
  for (uint16_t i = 0; i < strip.numPixels(); i++) {

    float shade = 30;
    //        float shade = 120;
    //        Serial.print("shade: ");\
    //      Serial.println();
    //        Serial.println(shade);
    strip.setPixelColor(i, strip.Color(shade, shade, shade));

  }
  strip.show(); // Initialize all pixels to 'off'
}

int leoncolorindex = 0;
uint32_t leoncolor[] = { strip.Color(250, 0, 0), strip.Color(0, 255, 0) };


//bool state_changed = false;
//bool pot_changed = false;

//long int t = 0;
//bool rainbow = false;

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
  for (int i = 0; i <= 10; i++) {
    int v = i * 10;
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


  //  if(pot_changed == true){
  //    for(uint16_t i=0; i<15; i++) {
  //      float shade = 250 * old_pot_state;
  ////        float shade = 250;
  //      Serial.print("shade: ");\
  //      Serial.println();
  //      Serial.println(shade);
  //      strip.setPixelColor(i, strip.Color(250, shade, shade));
  //    }
  //    rotary_changed = false;
  //    pot_changed = false;
  //    strip.show();
  //    delay(50);
  //  }

  if(state_changed == true){
    if(old_rotary_state == 0){
      if(button1_changed == true){

        leoncolorindex = leoncolorindex + 1;
//        Serial.println(sizeof(leoncolor) / sizeof(uint32_t) );
        if(leoncolorindex > 1){
          leoncolorindex = 0;
        }

        for(uint16_t i=0; i<15; i++) {
//          float shade = 250 * old_pot_state;
    //        float shade = 250;
//          Serial.print("shade: ");\
//          Serial.println();

          
          strip.setPixelColor(i, leoncolor[leoncolorindex]);
        }
        rotary_changed = false;
        pot_changed = false;
        strip.show();
        delay(50);
       
      }


      state_changed = false;
      button1_changed = false;
      
    }
    



    
  }
  
//
//  if (rotary_changed == true || just_started == true) {
//    just_started = false;
//    for (uint16_t i = 0; i < 15; i++) {
//      if (old_rotary_state == 0) { 
//          strip.setPixelColor(i, strip.Color(250, 0, 0));
//      } else if (old_rotary_state == 1) {
//          strip.setPixelColor(i, strip.Color(0, 250, 0));
//      } else if (old_rotary_state == 2) {
//          strip.setPixelColor(i, strip.Color(0, 0, 250));
//      } else if (old_rotary_state == 3) {
//          strip.setPixelColor(i, strip.Color(250, 250, 0));
//      } else if (old_rotary_state == 4) {
//          strip.setPixelColor(i, strip.Color(250, 0, 250));
//      } else if (old_rotary_state == 5) {
//          strip.setPixelColor(i, strip.Color(0, 250, 250));
//      }
//    }
//    rotary_changed = false;
//    pot_changed = false;
//    strip.show();
//    delay(50);
//  }



  //
  //  if(state_changed == true){
  //    Serial.println("state_changed");
  //    if(rotary_state == 0 && ( rotary_changed == true || pot_changed == true ) ){
  //      Serial.println("state is 0!");
  //      for(uint16_t i=0; i<strip.numPixels(); i++) {
  //        float shade = 250 * old_pot_state;
  ////        float shade = 120;
  //        Serial.print("shade: ");\
  //        Serial.println();
  //        Serial.println(shade);
  //        strip.setPixelColor(i, strip.Color(250, shade, shade));
  //      }
  //
  //      rotary_changed = false;
  //      pot_changed = false;
  //      strip.show();
  //      delay(50);
  //    }
  //
  //    state_changed = false;
  //
  //
  //  }
  //
  ////  if(rotary_changed == true && rotary_state == 0){
  ////        for(uint16_t i=0; i<strip.numPixels(); i++) {
  ////        float shade = 250 * old_pot_state;
  //////        float shade = 120;
  //////        Serial.print("shade: ");\
  ////  //      Serial.println();
  //////        Serial.println(shade);
  ////        strip.setPixelColor(i, strip.Color(250, shade, shade));
  ////      }
  ////
  ////  }else if(rotary_changed == true){
  ////
  ////
  ////  }
  //
  ////
  ////
  ////  // MODES
  ////
  ////    if(button1_state == 1 && (button1_changed || pot_changed)){
  ////      for(uint16_t i=0; i<strip.numPixels(); i++) {
  ////        float shade = 250 * old_pot_state;
  //////        float shade = 120;
  //////        Serial.print("shade: ");\
  ////  //      Serial.println();
  //////        Serial.println(shade);
  ////        strip.setPixelColor(i, strip.Color(250, shade, shade));
  ////      }
  ////      strip.show();
  ////      button1_changed = false;
  ////      pot_changed = false;
  //////      rainbow = false;
  ////      delay(50);
  ////    }
  ////  }else if(state == 0 && (state_changed || pot_changed)){
  ////
  ////    if(old_pot_percentage == 0.00){
  ////      for(uint16_t i=0; i<strip.numPixels(); i++) {
  ////        strip.setPixelColor(i, strip.Color(0, 0, 0));
  ////      }
  ////      strip.show();
  ////      state_changed = false;
  ////      pot_changed = false;
  ////      rainbow = false;
  ////      Serial.println("OFF");
  ////      delay(50);
  ////    }else{
  ////      rainbow = true;
  ////      state_changed = false;
  ////      pot_changed = false;
  ////    }
  ////
  ////  }
  ////
  ////
  ////  if(rainbow){
  ////    if (millis() - t > 5000) {
  ////      for (int i = 0; i <= strip.numPixels(); i++) {
  ////        int r = random(0, 100);
  ////        float rr = r * 0.01;
  ////
  ////        if (rr < 0.1) {
  ////          strip.setPixelColor(i, old_pot_percentage * 255.0, old_pot_percentage * float(random(0, 20)), old_pot_percentage * float(random(0, 20)));
  ////        } else if (rr < 0.2) {
  ////          strip.setPixelColor(i, old_pot_percentage * float(random(0, 20)), old_pot_percentage * random(0, 20), old_pot_percentage * 255.0);
  ////        } else if (rr < 0.3) {
  ////          strip.setPixelColor(i, old_pot_percentage * float(random(0, 20)), old_pot_percentage * 255.0, old_pot_percentage * float(random(0, 20)));
  ////        } else if (rr < 0.5) {
  ////          strip.setPixelColor(i, old_pot_percentage * 70.0, old_pot_percentage * 70.0, old_pot_percentage * 70.0);
  ////        }
  ////      }
  ////      t = millis();
  ////      strip.show();
  ////      delay(50);
  ////    }
  ////  }
  //
  //

}
