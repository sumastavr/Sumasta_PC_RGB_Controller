#include <sumastaConfig.h>

#include "FastLED.h"

#define LED_PIN     5
#define NUM_LEDS    40
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.



CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

#include "Keyboard.h"

const int buttonPin = A2;          // input pin for pushbutton
const int pcCasePin = 2;

int previousButtonState = HIGH;   // for checking the state of a pushButton
int previousButtonState2=HIGH;
int counter = 0;                  // button push counter

byte brightness[]={0,50,100,255};
const byte maxBrightness=3;
byte brightnessPointer=2;

void setup() {
    delay(3000); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    // make the pushButton pin an input:
    pinMode(buttonPin, INPUT);
    pinMode(pcCasePin, INPUT);

    digitalWrite(pcCasePin,HIGH);

    // initialize control over the keyboard:
    Keyboard.begin();
}

void loop()
{
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */

    if(brightnessPointer!=0){
        FillLEDsFromPaletteColors(startIndex);
        FastLED.show();
    }

    FastLED.delay(1000 / UPDATES_PER_SECOND);

    int buttonState = digitalRead(buttonPin);
    int buttonState2=digitalRead(pcCasePin);

    // if the button state has changed, send a toggle CTRL+SHIFT+M for mute/unmute microsoft teams
    if (buttonState != previousButtonState) {
        Keyboard.press(KEY_LEFT_CTRL);
        delay(200);
        Keyboard.press(KEY_LEFT_SHIFT);
        delay(200);
        Keyboard.press('m');
        delay(100);
        Keyboard.releaseAll();
    }

    if (buttonState2 == LOW) {
        
        Keyboard.press(KEY_DOWN_ARROW);
        brightnessPointer++;
        if (brightnessPointer>maxBrightness){
            brightnessPointer=0;
        }
        FastLED.setBrightness(brightness[brightnessPointer]);
        delay(100);
        Keyboard.releaseAll();
        delay(200);
    
    }

    if (buttonState == HIGH) {
        ChangePalettePeriodically(false);
    } else {
        ChangePalettePeriodically(true);
    }

    previousButtonState = buttonState;
    previousButtonState2 = buttonState2;

}

void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    uint8_t brightness = 255;

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically(boolean muted)
{   
    if (muted) {
        uint8_t secondHand = (millis() / 1000) % 90;
        static uint8_t lastSecond = 99;

        if (lastSecond != secondHand) {
            lastSecond = secondHand;
            if (secondHand >= 0 && secondHand < 10) { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
            if (secondHand >= 10 && secondHand < 20) { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND; }
            if (secondHand >= 20 && secondHand < 30) { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
            if (secondHand >= 30 && secondHand < 40) { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
            if (secondHand >= 40 && secondHand < 50) { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
            //if (secondHand == 30) { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
            //if (secondHand == 35) { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
            if (secondHand >= 50 && secondHand < 60) { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
            if (secondHand >= 60 && secondHand < 70) { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
            if (secondHand >= 70 && secondHand < 80) { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND; }
            if (secondHand >= 80 && secondHand < 90) { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
        }
    }
    else {
        SetupBlackAndWhiteStripedPalette();  // black and white here for the PC means RED and black,      
        currentBlending = LINEARBLEND;
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for (int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV(random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid(currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::Red;
    currentPalette[4] = CRGB::Red;
    currentPalette[8] = CRGB::Red;
    currentPalette[12] = CRGB::Red;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV(HUE_PURPLE, 255, 255);
    CRGB green = CHSV(HUE_GREEN, 255, 255);
    CRGB black = CRGB::Black;

    currentPalette = CRGBPalette16(
        green, green, black, black,
        purple, purple, black, black,
        green, green, black, black,
        purple, purple, black, black);
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,

    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,

    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};
