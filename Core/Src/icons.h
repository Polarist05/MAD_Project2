//#ifndef ICONS_H_
//#define ICONS_H_
//
//#include "lcd.h"
//
//#define ICON_WIDTH  ((uint8_t) 16)
//#define ICON_HEIGHT ((uint8_t) 16)
//#define BURST_AREA_ICON 0xF800
//
//extern Image playerIcon_1;
//extern Image playerIcon_2;
//extern Image playerIcon_3;
//extern Image bombIcon;
//extern Image wallIcon_1;
//extern Image wallIcon_2;
//extern Image heartIcon;
//extern Image shoeIcon;
//extern Image potionIcon;
//extern Image burstIcon;
//
//
//#endif /* ICONS_H_ */

#ifndef ICONS_H_
#define ICONS_H_

#include "lcd.h"

#define ICON_WIDTH ((uint8_t)16)
#define ICON_HEIGHT ((uint8_t)16)
#define BURST_AREA_ICON 0xF800
//
//typedef struct _Point
//{
//	uint8_t x;
//	uint8_t y;
//} Point;
//
//typedef struct _Image
//{
//	const char *imageArray;
//	Point drawPoint;
//	uint16_t width;
//	uint16_t height;
//} Image;

extern Image playerIcon1;
extern Image playerIcon2;
extern Image playerIcon3;
extern Image playerIcon4;
extern Image playerIcon5;

extern Image playerIcon1Yellow_32;
extern Image playerIcon2Yellow_32;
extern Image playerIcon3Yellow_32;
extern Image playerIcon4Yellow_32;
extern Image playerIcon5Yellow_32;
extern Image playerIcon1Orange_32;
extern Image playerIcon2Orange_32;
extern Image playerIcon3Orange_32;
extern Image playerIcon4Orange_32;
extern Image playerIcon5Orange_32;

extern Image playerIcon1_Bomb;
extern Image playerIcon2_Bomb;
extern Image playerIcon3_Bomb;
extern Image playerIcon4_Bomb;
extern Image playerIcon5_Bomb;

//Icon with out background
extern Image bombIcon_16;

//Icon on grass
extern Image wallIcon1;
extern Image wallIcon2;
extern Image heartIcon;
extern Image bombIcon;
extern Image shoeIcon;
extern Image potionIcon;
extern Image grassIcon;
extern Image addBombIcon;

extern Image playerIcon1_56;
extern Image playerIcon2_56;
extern Image playerIcon3_56;
extern Image playerIcon4_56;
extern Image playerIcon5_56;

extern Image playerIcon1_112;
extern Image playerIcon2_112;
extern Image playerIcon3_112;
extern Image playerIcon4_112;
extern Image playerIcon5_112;

extern Image settingPage_1;
extern Image settingPage_2;
extern Image winnerPage;

extern const uint8_t playerImage1[512];
extern const uint8_t playerImage2[512];
extern const uint8_t playerImage3[512];
extern const uint8_t playerImage4[512];
extern const uint8_t playerImage5[512];
extern const uint8_t playerImage1Bomb[512];
extern const uint8_t playerImage2Bomb[512];
extern const uint8_t playerImage3Bomb[512];
extern const uint8_t playerImage4Bomb[512];
extern const uint8_t playerImage5Bomb[512];

extern const uint8_t playerImage1Yellow_32[2048];
extern const uint8_t playerImage2Yellow_32[2048];
extern const uint8_t playerImage3Yellow_32[2048];
extern const uint8_t playerImage4Yellow_32[2048];
extern const uint8_t playerImage5Yellow_32[2048];

extern const uint8_t playerImage1Orange_32[2048];
extern const uint8_t playerImage2Orange_32[2048];
extern const uint8_t playerImage3Orange_32[2048];
extern const uint8_t playerImage4Orange_32[2048];
extern const uint8_t playerImage5Orange_32[2048];

extern const uint8_t playerImage1White_56[6272];
extern const uint8_t playerImage2White_56[6272];
extern const uint8_t playerImage3White_56[6272];
extern const uint8_t playerImage4White_56[6272];
extern const uint8_t playerImage5White_56[6272];

extern const uint8_t playerImage1_112[25088];
extern const uint8_t playerImage2_112[25088];
extern const uint8_t playerImage3_112[25088];
extern const uint8_t playerImage4_112[25088];
extern const uint8_t playerImage5_112[25088];

extern const uint8_t bombImage_16[512];
extern const uint8_t bombImage[512];
extern const uint8_t shoeImage[512];
extern const uint8_t heartImage[512];
extern const uint8_t potionImage[512];
extern const uint8_t wallImage1[512];
extern const uint8_t wallImage2[512];
extern const uint8_t grassImage[512];
extern const uint8_t addBombImage[512];

extern const uint8_t winPage[153600];
extern const uint8_t setting_1[153600];
extern const uint8_t setting_2[33600];

#endif /* ICONS_H_ */

