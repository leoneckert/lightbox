// This is a demonstration on how to use an input device to trigger changes on your neo pixels.
// You should wire a momentary push button to connect from ground to a digital IO pin.  When you
// press the button it will change to a new pixel animation.  Note that you need to press the
// button once to start the first animation!

//#include <Adafruit_NeoPixel.h>

//#define BUTTON_PIN   12    // Digital IO pin connected to the button.  This will be
//                          // driven with a pull-up resistor so the switch should
//                          // pull the pin to ground momentarily.  On a high -> low
//                          // transition the button press logic will execute.
//
//#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.

#define ROTARY_PIN    A0
int rotary_state = 0;
int old_rotary_state = 0;
int rotary_smooth_threshold = 100; // the higher the smoother.
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



//#define PIXEL_COUNT 30

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick

//
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
//
//
//
//bool old_state = 0;
//float old_pot_percentage = -0.0;

void setup() {
  Serial.begin(9600);
//  pinMode(BUTTON_PIN, INPUT);
//  old_state = digitalRead(BUTTON_PIN);

  pinMode(POT_PIN, INPUT);
  
  
  pinMode(ROTARY_PIN, INPUT);
  

//  
//  strip.begin();
//  strip.show(); // Initialize all pixels to 'off'
}



//bool state_changed = false;
//bool pot_changed = false;

//long int t = 0;
//bool rainbow = false;

void loop() {
//  
//  // BUTTON STATE:
//  bool state = digitalRead(BUTTON_PIN);
//  // Check if state changed from high to low (button press).
//  if (state == 0 && old_state != 0) {
//    delay(20);
//    // Check if button is still low after debounce.
//    state = digitalRead(BUTTON_PIN);
//    if (state == 0) {
//       Serial.println("changed mode: 0");
//       old_state = 0;
//       state_changed = true;
//    }
//  }else if (state == 1 && old_state != 1) {
//    delay(20);
//    state = digitalRead(BUTTON_PIN);
//    if (state == 1) {
//      old_state = 1;
//      Serial.println("changed mode: 1");
//      state_changed = true;
//    }
//  }


  //POT STATE:
  int pot_val = analogRead(POT_PIN);  // min value: 575 / max value: 870
//  Serial.println(pot_state);

  int pot_range = map(pot_val, min_a_read, max_a_read, 0, 109);

  int new_pot_range = 0;
//  for(int i = 0; i <= 10; i++){
//    int v = i*10;
//    if(pot_range > v){
//      new_pot_range = v;
//    }
//  }
  for(int i = 0; i <= 20; i++){
      int v = i*5;
      if(pot_range > v){
        new_pot_range = v;
      }
  }
  float pot_state = float(new_pot_range) / 100;  

  if (pot_state != old_pot_state){
    pot_smoother -= 1;
    if(pot_smoother <= 0){

      Serial.print("POT   ");
      Serial.print(old_pot_state);
      Serial.print("   -->   ");
      Serial.println(pot_state);
      old_pot_state = pot_state;       
      pot_changed = true;
    }
  }else{
    pot_smoother = pot_smooth_threshold;
  }
  

//  Serial.print(pot_state);
//  Serial.print("POT: ");
//  Serial.print(pot_percentage);


//  ROTARY STATE:
  int rotary_val = analogRead(ROTARY_PIN);
  
//  Serial.print(" ROTARY: ");
  if(rotary_val < 400){
    rotary_state = 0;
  }else if(rotary_val < 450){
    rotary_state = 1;  
  }else if(rotary_val < 530){
    rotary_state = 2;
  }else if(rotary_val < 620){
    rotary_state = 3;   
  }else if(rotary_val < 750){
    rotary_state = 4;
  }else{
    rotary_state = 5;
  }
//  if(rotary_state != old_rotary_state){
//    Serial.print("ROTARY   ");
//    Serial.print(old_rotary_state);
//    Serial.print("   -->   ");
//    Serial.println(rotary_state);
//    old_rotary_state = rotary_state;
//  }

  if (rotary_state != old_rotary_state){
    rotary_smoother -= 1;
    if(rotary_smoother <= 0){

      Serial.print("ROTARY   ");
      Serial.print(old_rotary_state);
      Serial.print("   -->   ");
      Serial.println(rotary_state);
      old_rotary_state = rotary_state;       
      rotary_changed = true;
    }
  }else{
    rotary_smoother = rotary_smooth_threshold;
  }
  
  
//  Serial.println("");
//  Serial.print("rotary_state");
//  Serial.println(rotary_state);
  
//  if (pot_percentage != old_pot_percentage){
//    pot_smoother -= 1;
//    if(pot_smoother <= 0){
//      old_pot_percentage = pot_percentage;
//      Serial.print("changed pot percentage:");
//      Serial.println(old_pot_percentage); 
//      Serial.print("analogRead(POT_PIN):");
//      Serial.println(analogRead(POT_PIN)); 
//       
//      pot_changed = true;
//    }
//  }else{
//    pot_smoother = pot_smooth_threshold;
//  }
//  
//  
//  // MODES
//
//  if(state == 1 && (state_changed || pot_changed)){
//    for(uint16_t i=0; i<strip.numPixels(); i++) {
//      float shade = 250 * old_pot_percentage;
//      Serial.print("shade: ");\
////      Serial.println();
//      Serial.println(shade);
//      strip.setPixelColor(i, strip.Color(250, shade, shade)); 
//    }  
//    strip.show();
//    state_changed = false;
//    pot_changed = false;
//    rainbow = false;
//    delay(50);
//  }else if(state == 0 && (state_changed || pot_changed)){
//
//    if(old_pot_percentage == 0.00){
//      for(uint16_t i=0; i<strip.numPixels(); i++) {
//        strip.setPixelColor(i, strip.Color(0, 0, 0));
//      }
//      strip.show();
//      state_changed = false;
//      pot_changed = false;
//      rainbow = false;
//      Serial.println("OFF");
//      delay(50);
//    }else{
//      rainbow = true;
//      state_changed = false;
//      pot_changed = false;
//    }
//    
//  }
//
//
//  if(rainbow){
//    if (millis() - t > 5000) {
//      for (int i = 0; i <= strip.numPixels(); i++) {
//        int r = random(0, 100);
//        float rr = r * 0.01;
//        
//        if (rr < 0.1) {
//          strip.setPixelColor(i, old_pot_percentage * 255.0, old_pot_percentage * float(random(0, 20)), old_pot_percentage * float(random(0, 20)));
//        } else if (rr < 0.2) {
//          strip.setPixelColor(i, old_pot_percentage * float(random(0, 20)), old_pot_percentage * random(0, 20), old_pot_percentage * 255.0);
//        } else if (rr < 0.3) {
//          strip.setPixelColor(i, old_pot_percentage * float(random(0, 20)), old_pot_percentage * 255.0, old_pot_percentage * float(random(0, 20)));
//        } else if (rr < 0.5) {
//          strip.setPixelColor(i, old_pot_percentage * 70.0, old_pot_percentage * 70.0, old_pot_percentage * 70.0);
//        } 
//      }
//      t = millis();
//      strip.show();
//      delay(50);
//    }
//  }

  

}
