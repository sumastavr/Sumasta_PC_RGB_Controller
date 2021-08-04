#include <Arduino.h>

// SW Version
const String SWVersion = "V1.31 (28-07-21) P.Sumasta";

// Function prototype
void FillLEDsFromPaletteColors(uint8_t colorIndex);
void ChangePalettePeriodically(boolean muted);
void SetupBlackAndWhiteStripedPalette();
void SetupPurpleAndGreenPalette();
void SetupTotallyRandomPalette();