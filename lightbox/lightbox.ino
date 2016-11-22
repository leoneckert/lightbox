#include <Adafruit_NeoPixel.h>

#define PIN 6

const int pot_pin = A0;
int pot_value = 0;

const int button_one = 12;
int buttonOneState = 0;
//const int button_two = 7;
//int buttonTwoState = 0;

double t = 0;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(9600);

  pinMode(button_one, INPUT);
  pinMode(pot_pin, INPUT);

//  strip.setBrightness(10);
  strip.begin();
  strip.show();
}

int previousPercentages[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float percentage = 0.0;

void loop() {

  // put your main code here, to run repeatedly:
  int
  pot_value = analogRead(pot_pin);
  int potential_percentage = int(map(pot_value, 680, 950, 1, 100));
  if (potential_percentage < 10) {
    potential_percentage = 10;
  }else if (potential_percentage > 100) {
    potential_percentage = 100;
  }
  int remainder = potential_percentage%20;
  potential_percentage = potential_percentage - remainder;

  bool p_changed = true;

  for(int i = 0; i <= (sizeof(previousPercentages)/sizeof(int)); i++){
    if(previousPercentages[i] != potential_percentage){
      p_changed = false;
    }
  }
  if(p_changed){
    percentage = float(potential_percentage) * 0.01;
  }
  
  for(int i = 0; i <= (sizeof(previousPercentages)/sizeof(int)); i++){
    if(i == (sizeof(previousPercentages)/sizeof(int))){
       previousPercentages[i] = potential_percentage;
    }else{
       previousPercentages[i] = previousPercentages[i+1];
    }
  }
//  
  
  buttonOneState = digitalRead(button_one);

  if (buttonOneState == 0) {
    for (int i = 0; i <= strip.numPixels(); i++) {
      strip.setPixelColor(i, 255, percentage * 255, percentage * 255);
    }
    strip.show();
  } else if (buttonOneState == 1) {
    if (millis() - t > 5000) {
      for (int i = 0; i <= strip.numPixels(); i++) {
        int r = random(0, 100);
        //      Serial.println(r);
        float rr = r * 0.01;
        if (rr < 0.1) {
          strip.setPixelColor(i, percentage * 255.0, percentage * float(random(0, 20)), percentage * float(random(0, 20)));
        } else if (rr < 0.2) {
          strip.setPixelColor(i, percentage * float(random(0, 20)), percentage * random(0, 20), percentage * 255.0);
        } else if (rr < 0.3) {
          strip.setPixelColor(i, percentage * float(random(0, 20)), percentage * 255.0, percentage * float(random(0, 20)));
        } else if (rr < 0.5) {
          strip.setPixelColor(i, percentage * 70.0, percentage * 70.0, percentage * 70.0);
        } 
      }
      t = millis();
      strip.show();
    }
  }
  




}
