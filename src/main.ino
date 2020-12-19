#include <Adafruit_NeoPixel.h>
#include <GyverButton.h>
#define TOP_PIN 7
#define SIDES_PIN 9
#define BTN 6
#define TOP_PIXEL_COUNT 16  // Popular NeoPixel ring size
#define SIDES_PIXEL_COUNT 3 // Popular NeoPixel ring size
#define BRGTH_MIN 15
#define BRGTH_DEFAULT 120
#define BRGTH_MAX 250

GButton butt1(BTN, HIGH_PULL);
Adafruit_NeoPixel pixelsTop(TOP_PIXEL_COUNT, TOP_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsSides(SIDES_PIXEL_COUNT, SIDES_PIN, NEO_GRB + NEO_KHZ800);
uint32_t colorArray[3] = {pixelsTop.Color(0, 255, 0), pixelsTop.Color(255, 0, 0), pixelsTop.Color(0, 0, 255)};
uint32_t currentColor = 0;
int brgtArray[3] = {BRGTH_MIN, BRGTH_DEFAULT, BRGTH_MAX};

int value = 0;
void setup()
{
  Serial.begin(9600);
  butt1.setDebounce(50);
  butt1.setTimeout(300);
  pinMode(BTN, INPUT_PULLUP);
  pixelsTop.begin();
  pixelsSides.begin();
  currentColor = colorArray[0];
  highlight();
}

void loop()
{
  static unsigned long lastClick;
  static int colorIndx = 0;
  static bool choosed = true;
  butt1.tick();

  if (butt1.isClick())
  {
    choosed = false;
    currentColor = pixelsTop.Color(random(0, 255), random(0, 255), random(0, 255));
    chooseColor();
    lastClick = millis();
  }
  else if (butt1.isDouble())
  {
    colorIndx++;
    if (colorIndx > 2)
    {
      colorIndx = 0;
    }
    choosed = false;
    currentColor = colorArray[colorIndx];
    chooseColor();
    lastClick = millis();
  }
  else if (butt1.isHolded())
  {
    decay();
  }
  if (!choosed && millis() - lastClick > 1000)
  {
    highlight();
    choosed = true;
  }
}

void chooseColor()
{
  pixelsTop.clear();
  pixelsSides.clear();
  pixelsTop.setBrightness(BRGTH_DEFAULT);
  pixelsTop.setBrightness(BRGTH_DEFAULT);
  pixelsTop.setPixelColor(0, currentColor);
  pixelsTop.show();
  pixelsSides.show();
}

// Зажигает линию
void drawLines()
{
  pixelsTop.setBrightness(BRGTH_DEFAULT);
  pixelsSides.setBrightness(BRGTH_DEFAULT);
  int sidesI = 0;
  for (int i = 0; i < TOP_PIXEL_COUNT; i++)
  {
    pixelsTop.setPixelColor(i, currentColor);
    // if (i % 5 == 0)
    // {
    // pixelsSides.setPixelColor(sidesI, currentColor);
    // pixelsSides.show();
    // sidesI++;
    // }
    pixelsTop.show();
    delay(50);
  }
  delay(500);
  for (int i = 0; i < SIDES_PIN; i++)
  {
    pixelsSides.setPixelColor(i, currentColor);
    pixelsSides.show();
    delay(50);
  }
}

// Запуск свечения
void highlight()
{
  drawLines();
  for (int i = 0; i < 15; i++)
  {
    int bright = brgtArray[random(0, 2)];
    pixelsTop.setBrightness(bright);
    pixelsSides.setBrightness(bright);
    pixelsTop.show();
    pixelsSides.show();
    delay(random(50, 150));
  }
  pixelsTop.setBrightness(BRGTH_DEFAULT);
  pixelsSides.setBrightness(BRGTH_DEFAULT);
  pixelsTop.show();
  pixelsSides.show();
}

// Запуск угасания
void decay()
{
  int sidesI = 2;
  for (int i = TOP_PIXEL_COUNT; i >= 0; i--)
  {
    pixelsTop.fill(0, i, 1);
    pixelsTop.show();
    if (i % 5 == 0)
    {
      pixelsSides.fill(0, sidesI, 1);
      pixelsSides.show();
      sidesI--;
    }
    delay(50);
  }
}