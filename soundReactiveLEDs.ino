#include <FastLED.h>

#define MIC_PIN 15
#define LED_PIN 13
#define NUM_LEDS 42
#define NUM_LEDS_1 14 // Side of the triangle
#define NUM_LEDS_2 14 // Side of the triangle
#define NUM_LEDS_3 14 // Bottom of the triangle

#define BRIGHTNESS 80
#define GATE 1000

// Setup an array with the three sides
CRGBArray<NUM_LEDS> leds;
CRGBSet leds_1(leds, 0, NUM_LEDS_1);
CRGBSet leds_2(leds, NUM_LEDS_1, NUM_LEDS_2);
CRGBSet leds_3(leds, NUM_LEDS_1+NUM_LEDS_2, NUM_LEDS_3+NUM_LEDS_1+NUM_LEDS_2);


void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811,LED_PIN,GRB>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.println(leds_1.size());

}

void loop() {
  // put your main code here, to run repeatedly:
  int micValue, height;
  // Read the microphone value and try to remove the noise
  micValue = analogRead(MIC_PIN);
  micValue = abs(1023 - micValue);
  micValue = (micValue <= GATE) ? 0 : abs(micValue - GATE);

  // Calculate height i.e how many LEDs will light up
  height = (NUM_LEDS_1+2) * (micValue) / (long)(2000);
  // If height is negative set it above the top LED
  if (height < 0L) {
    height = NUM_LEDS_1+2;
  }

  // Set the LED color according to the LED position
  for (uint8_t i = 0; i < NUM_LEDS_1; i++) {
    if (i >= height) {
      leds_1[i] = CRGB(0,0,0);
    }
    else leds_1[i] = Wheel( map( i, 0, NUM_LEDS_1-1, 30, 150 ) );
  }

  for (uint8_t i = 0; i < NUM_LEDS_1; i++) {
    // Copy the color values to the other side strip in reverse order
    leds_2[NUM_LEDS_1 - i] = leds_1[i];
    // Set the bottom strip to the same color as the first LED on the side strips
    leds_3[i] = leds_1[0];
  }
  FastLED.show();
}

CRGB Wheel(byte wheelPos) {
  if (wheelPos < 85) {
    return CRGB(wheelPos * 3, 255 - wheelPos * 3, 0);
  } 
  else if (wheelPos < 170) {
    wheelPos -= 85;
    return CRGB(255 - wheelPos * 3, 0, wheelPos * 3);
  } 
  else {
    wheelPos -= 170;
    return CRGB(0, wheelPos * 3, 255 - wheelPos * 3);
  }
}
