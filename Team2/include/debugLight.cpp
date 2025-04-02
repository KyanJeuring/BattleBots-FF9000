// Copyright © Kyan Jeuring & Miriam Cerulíková 2025

void resetLights()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
}

void blinkLEDs(int frequency, int r, int g, int b)
{
    static unsigned long previousMillis = 0;
    static bool isOn = false;
    const unsigned long interval = frequency; // Interval in milliseconds

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        isOn = !isOn;

        if (isOn)
        {
            NeoPixel.setPixelColor(PIXEL_TOP_LEFT, r, g, b);
            NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, r, g, g);
            NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, r, g, g);
            NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, r, g, g);
            NeoPixel.show();
        }
        else
        {
            resetLights();
        }
    }
}

void setDriveForwardColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 255, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 255, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
}

void setDriveBackwardColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 0, 0);
    NeoPixel.show();
}

void setTurnRightColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 144, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 144, 0);
    NeoPixel.show();
}

void setTurnLeftColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 144, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 144, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
}

void setTurnAroundColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 144, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 144, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 144, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 144, 0);
    NeoPixel.show();
}

void setDriveStopColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 0, 0);
    NeoPixel.show();
}

void setStandByColor()
{
    blinkLEDs(350, 255, 0, 0);
}

void setCalibrateColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 60, 255);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 60, 255);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 60, 255);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 60, 255);
    NeoPixel.show();
}