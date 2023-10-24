#include "lcd.h"
#include "stdio.h"

#include "ILI9341_Touchscreen.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "spi.h"

const uint8_t SCREEN_ROTATION = SCREEN_HORIZONTAL_1;

void fillScreenColor(uint16_t color)
{
	ILI9341_Fill_Screen(color);
}

void clearScreenArea(Rectangle area, uint16_t color)
{
	ILI9341_Set_Address(area.x0, area.y0, area.x1, area.y1);
	ILI9341_Draw_Colour_Burst(color, (area.x1 - area.x0) * (area.y1 - area.y0));
}

void setRotation(uint8_t rotation)
{
	ILI9341_Set_Rotation(rotation);
}

void drawText(const char *text, uint8_t x, uint8_t y, uint16_t size)
{
	ILI9341_Draw_Text(text, x, y, BLACK, size, WHITE);
}

void drawTextAtPoint(const char *text, Point position, uint16_t size)
{
	drawText(text, position.x, position.y, size);
}

void drawTextWithColor(const char *text, uint8_t x, uint8_t y, uint16_t color,
		uint16_t size, uint16_t backgroundColor)
{
	ILI9341_Draw_Text(text, x, y, color, size, backgroundColor);
}

void drawTextWithColorAtPoint(const char *text, Point position, uint16_t color,
		uint16_t size, uint16_t backgroundColor)
{
	drawTextWithColor(text, position.x, position.y, color, size, backgroundColor);
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	ILI9341_Draw_Pixel(x, y, color);
}

void drawPixelByPoint(Point point, uint16_t color)
{
	ILI9341_Draw_Pixel(point.x, point.y, color);
}

// Based on ILI9341_Draw_Rectangle(x, y, width, height, color);
void drawHollowRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	ILI9341_Draw_Hollow_Rectangle_Coord(x0, y0, x1, y1, color);
}

void drawHollowRectangleAtCoord(Rectangle rectangle, uint16_t color)
{
	ILI9341_Draw_Hollow_Rectangle_Coord(rectangle.x0, rectangle.y0, rectangle.x1, rectangle.y1, color);
}

void drawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	ILI9341_Draw_Filled_Rectangle_Coord(x0, y0, x1, y1, color);
}

void drawFilledRectangleAtCoord(Rectangle rectangle, uint16_t color)
{
	ILI9341_Draw_Filled_Rectangle_Coord(rectangle.x0, rectangle.y0, rectangle.x1, rectangle.y1, color);
}

void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color)
{
	ILI9341_Draw_Horizontal_Line(x, y, width, color);
}

void drawHorizontalLineAtPoint(Point point, uint16_t width, uint16_t color)
{
	ILI9341_Draw_Horizontal_Line(point.x, point.y, width, color);
}

void drawVerticallLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color)
{
	ILI9341_Draw_Vertical_Line(x, y, width, color);
}

void drawVerticalLineAtPoint(Point point, uint16_t width, uint16_t color)
{
	ILI9341_Draw_Horizontal_Line(point.x, point.y, width, color);
}

void drawHollowCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color)
{
	ILI9341_Draw_Hollow_Circle(x, y, radius, color);
}

void drawHollowCircleAtCoord(Circle circle, uint16_t color)
{
	ILI9341_Draw_Hollow_Circle(circle.x, circle.y, circle.radius, color);
}

void drawFilledCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color)
{
	ILI9341_Draw_Filled_Circle(x, y, radius, color);
}

void drawFilledCircleAtCoord(Circle circle, uint16_t color)
{
	ILI9341_Draw_Filled_Circle(circle.x, circle.y, circle.radius, color);
}

void drawImage(const char *imageArray, uint8_t orientation)
{
	ILI9341_Draw_Image(imageArray, orientation);
}

void drawImageAtPoint(Image image, uint8_t rotation)
{
	setRotation(rotation);

	Point startPoint = image.drawPoint;
	Point endPoint = { 0, 0};

	switch (rotation)
	{
	case SCREEN_HORIZONTAL_1:
	case SCREEN_HORIZONTAL_2:
		endPoint.x = startPoint.x + image.width - 1;
		endPoint.y = startPoint.y + image.height;
		break;
	case SCREEN_VERTICAL_1:
	case SCREEN_VERTICAL_2:
		endPoint.x = startPoint.y + image.height - 1;
		endPoint.y = startPoint.x + image.width;
		break;
	}

	ILI9341_Set_Address(startPoint.x, startPoint.y, endPoint.x, endPoint.y);

	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);

	unsigned char Temp_small_buffer[BURST_MAX_SIZE];
	uint32_t counter = 0;
	for (uint32_t i = 0; i < image.width * image.height * 2 / BURST_MAX_SIZE; i++)
	{
		for (uint32_t k = 0; k < BURST_MAX_SIZE; k++)
		{
			Temp_small_buffer[k] = image.imageArray[counter + k];
		}
		HAL_SPI_Transmit(HSPI_INSTANCE, (unsigned char*) Temp_small_buffer, BURST_MAX_SIZE, 10);
		counter += BURST_MAX_SIZE;
	}
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
}

Rectangle getImageArea(Image image)
{
	Rectangle imageArea =
	{
			image.drawPoint.x,
			image.drawPoint.y,
			image.drawPoint.x + image.width,
			image.drawPoint.y + image.height
	};
	return imageArea;
}

uint16_t getCircleEdgeX(Circle circle)
{
	return circle.x + circle.radius;
}

uint16_t getCircleEdgeY(Circle circle)
{
	return circle.y + circle.radius;
}

uint16_t getCircleEdgeXNegative(Circle circle)
{
	return circle.x - circle.radius;
}

uint16_t getCircleEdgeYNegative(Circle circle)
{
	return circle.y - circle.radius;
}
