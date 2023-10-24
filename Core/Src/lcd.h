#ifndef LCD_H_
#define LCD_H_

#include "stdint.h"
#include "stm32f7xx.h"

#define SCREEN_HEIGHT ILI9341_SCREEN_HEIGHT
#define SCREEN_WIDTH ILI9341_SCREEN_WIDTH

extern const uint8_t SCREEN_ROTATION;

typedef struct _Point
{
	uint16_t x;
	uint16_t y;
} Point;

typedef struct _Rectangle
{
	uint16_t x0;
	uint16_t y0;
	uint16_t x1;
	uint16_t y1;
} Rectangle;

typedef struct _Circle
{
	uint16_t x;
	uint16_t y;
	uint16_t radius;
} Circle;

typedef struct _Image
{
	const char *imageArray;
	Point drawPoint;
	uint16_t width;
	uint16_t height;
} Image;

void fillScreenColor(uint16_t color);
void clearScreenArea(Rectangle area, uint16_t color);

void setRotation(uint8_t rotation);

void drawText(const char *text, uint8_t x, uint8_t y, uint16_t size);
void drawTextAtPoint(const char *text, Point position, uint16_t size);
void drawTextWithColor(const char *text, uint8_t x, uint8_t y, uint16_t color, uint16_t size, uint16_t backgroundColor);
void drawTextWithColorAtPoint(const char *text, Point position, uint16_t color, uint16_t size, uint16_t backgroundColor);

void drawPixel(uint16_t x, uint16_t y, uint16_t color);
void drawPixelAtPoint(Point point, uint16_t color);

void drawHollowRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void drawHollowRectangleAtCoord(Rectangle rectangle, uint16_t color);
void drawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void drawFilledRectangleAtCoord(Rectangle rectangle, uint16_t colore);

void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color);
void drawHorizontalLineAtPoint(Point point, uint16_t width, uint16_t color);
void drawVerticallLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color);
void drawVerticalLineAtPoint(Point point, uint16_t width, uint16_t color);

void drawHollowCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color);
void drawHollowCircleAtCoord(Circle circle, uint16_t color);
void drawFilledCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color);
void drawFilledCircleAtCoord(Circle circle, uint16_t color);

void drawImage(const char *imageArray, uint8_t orientation);
void drawImageAtPoint(Image image, uint8_t rotation);
Rectangle getImageArea(Image image);

uint16_t getCircleEdgeX(Circle circle);
uint16_t getCircleEdgeY(Circle circle);
uint16_t getCircleEdgeXNegative(Circle circle);
uint16_t getCircleEdgeYNegative(Circle circle);

#endif /* MY_LCD_H_ */
