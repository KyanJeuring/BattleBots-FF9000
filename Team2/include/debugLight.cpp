void resetLight()
{
    NeoPixel.clear();
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
}

void setDriveForwardColor()
{
    NeoPixel.clear();
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 255, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 255, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
}

void setDriveBackwardColor()
{
    NeoPixel.clear();
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 0, 0);
    NeoPixel.show();
}

void setTurnRightColor()
{
    NeoPixel.clear();
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 100, 0);
    NeoPixel.show();
}

void setTurnLeftColor()
{
    NeoPixel.clear();
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
}

void setTurnAroundColor()
{
    NeoPixel.clear();
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 100, 0);
    NeoPixel.show();
}

void setDriveStopColor()
{
    NeoPixel.clear();
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 0, 0);
    NeoPixel.show();
}

void setStandByColor()
{
    NeoPixel.clear();
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 85, 90);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 85, 90);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 85, 90);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 85, 90);
    NeoPixel.show();
}