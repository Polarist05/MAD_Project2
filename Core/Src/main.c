/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "rng.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gameUtility.h"
#include "utility.h"
#include "stdio.h"
#include "ILI9341_Touchscreen.h"

#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

#include "snow_tiger.h"
#include "icons.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define GAME_TIME 300
bool endGameFlag=true;
char str[90];
Player *playerWinner[4];
int playerWinnerCount=0;
int clock =0;
uint8_t isTouchWithinRectangle(Rectangle rectangle, Point point) {
	return (point.x >= rectangle.x0 && point.x <= rectangle.x1
			&& point.y <= rectangle.y1 && point.y >= rectangle.y0);
}
Image character112List[5];
Image character56List[5];
void startGame();
void checkMap(Point cursor, int mapState[], Rectangle mapRectangle[]) {
	for (int i = 0; i < 3; i++) {
		if (isTouchWithinRectangle(mapRectangle[i], cursor)) {
			mapState[i] = 1;
			for (int j = 0; j < 3; j++) {
				if (j == i)
					continue;
				mapState[j] = 0;
			}
		}
		if (mapState[i]) {
			ILI9341_Draw_Filled_Rectangle_Coord((mapRectangle[i]).x0,
					(mapRectangle[i]).y0, (mapRectangle[i]).x1,
					(mapRectangle[i]).y1, 0x02e0);
			switch (i) {
			case 0:
				ILI9341_Draw_Text("Map 1", 48, 180, WHITE, 1, 0x02e0);
				break;
			case 1:
				ILI9341_Draw_Text("Map 2", 118, 180, WHITE, 1, 0x02e0);
				break;
			case 2:
				ILI9341_Draw_Text("Map 3", 188, 180, WHITE, 1, 0x02e0);
				break;
			}
		} else {
			ILI9341_Draw_Filled_Rectangle_Coord((mapRectangle[i]).x0,
					(mapRectangle[i]).y0, (mapRectangle[i]).x1,
					(mapRectangle[i]).y1,
					GREEN);
			ILI9341_Draw_Hollow_Rectangle_Coord((mapRectangle[i]).x0,
					(mapRectangle[i]).y0, (mapRectangle[i]).x1,
					(mapRectangle[i]).y1, 0x02e0);
			switch (i) {
			case 0:
				ILI9341_Draw_Text("Map 1", 48, 180, 0x02e0, 1,
				GREEN);
				break;
			case 1:
				ILI9341_Draw_Text("Map 2", 118, 180, 0x02e0, 1,
				GREEN);
				break;
			case 2:
				ILI9341_Draw_Text("Map 3", 188, 180, 0x02e0, 1,
				GREEN);
				break;
			}
		}
	}
}

int checkStartGame(int numPlayerState[], int mapState[], int characterState[],
		int characterState_2[], int* displayScreen) {
	int checkPlay = 0;
	for (int i = 0; i < 2; i++) {
		if (numPlayerState[i] == 1) {
			checkPlay++;
			break;
		}
	}
	for (int i = 0; i < 3; i++) {
		if (mapState[i] == 1) {
			checkPlay++;
			break;
		}
	}

	for (int i = 0; i < 5; i++) {
		if (characterState[i] == 1) {
			checkPlay++;
			break;
		}
	}
	if (numPlayerState[1]) {
		for (int i = 0; i < 5; i++) {
			if (characterState_2[i] == 1) {
				checkPlay++;
				break;
			}
		}
	}

	if (numPlayerState[0] && checkPlay == 3) {
		return 1;
	} else if (numPlayerState[1] && checkPlay == 4) {
		return 1;
	} else {
		checkPlay = 0;
		return 0;
	}
}
int stateWin = 0;
int displayScreen = 0;
	int numPlayerState[2] = { 0, 0 };
	int characterState[5] = { 0, 0, 0, 0, 0 };
	int characterState_2[5] = { 0, 0, 0, 0, 0 };
	int mapState[3] = { 0, 0, 0 };
	int mapID = 0;

	Rectangle numPlayerRectangle[2] = { };
	Rectangle charRectangle[5] = { };
	Rectangle charHighlightRectangle[5] = { };
	Rectangle mapRectangle[3] = { };

	uint16_t xPos = 0;
	uint16_t yPos = 0;
//setting container
	Rectangle c1 = { 10, 50, 250, 90 }; //size: 40x240
	Rectangle c2 = { 10, 100, 250, 155 }; //size: 55x240
	Rectangle c3 = { 10, 160, 250, 205 }; //size: 40x240
//setting Button
	Rectangle bPlayer_1 = { 45, 65, 125, 85 }; //size: 80x20
	Rectangle bPlayer_2 = { 135, 65, 215, 85 }; //size: 80x20
//setting character
	Rectangle ch1 = { 30, 115, 62, 147 }; //size: 32x32
	Rectangle ch2 = { 72, 115, 104, 147 }; //size: 32x32
	Rectangle ch3 = { 114, 115, 146, 147 }; //size: 32x32
	Rectangle ch4 = { 156, 115, 188, 147 }; //size: 32x32
	Rectangle ch5 = { 198, 115, 230, 147 }; //size: 32x32
//setting hilight when choosen
	Rectangle hil_ch1 = { 28, 113, 64, 149 }; //size: 36x36
	Rectangle hil_ch2 = { 70, 113, 106, 149 }; //size: 36x36
	Rectangle hil_ch3 = { 112, 113, 148, 149 }; //size: 36x36
	Rectangle hil_ch4 = { 154, 113, 190, 149 }; //size: 36x36
	Rectangle hil_ch5 = { 196, 113, 232, 149 }; //size: 36x36
//setting Button
	Rectangle bMap_1 = { 30, 175, 90, 195 }; //size: 60x20
	Rectangle bMap_2 = { 100, 175, 160, 195 }; //size: 60x20
	Rectangle bMap_3 = { 170, 175, 230, 195 }; //size: 60x20

	Rectangle bPlay = { 90, 210, 170, 230 }; //size: 80x20
	Rectangle bNext = { 230, 210, 250, 230 }; //size: 20x20
	Rectangle bBack = { 10, 210, 30, 230 }; //size: 20x20

Image* getCharacterYellowList(int index){

	Image characterYellowList[5] = { playerIcon1Yellow_32, playerIcon2Yellow_32,
			playerIcon3Yellow_32, playerIcon4Yellow_32, playerIcon5Yellow_32 };
	return &characterYellowList[index];
}
Image* getCharacterOrangeList(int index){

	Image characterOrangeList[5] = { playerIcon1Orange_32, playerIcon2Orange_32,
				playerIcon3Orange_32, playerIcon4Orange_32, playerIcon5Orange_32 };
	return &characterOrangeList[index];
}

	Point cursor;
	Point drawPos;

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
//----------------------------------------------------------



Vector2 offset = {OFFSET_X,OFFSET_Y};
Vector2 mapSize = {MAP_WIDTH,MAP_HEIGHT};
Vector2 cellSize = {CELL_SIZE,CELL_SIZE};
Vector2 itemSize = {ITEM_SIZE,ITEM_SIZE};
MapCell map[MAP_HEIGHT][MAP_WIDTH];
Transform walls[4] = {
	{{OFFSET_X-CELL_SIZE/2,OFFSET_Y-CELL_SIZE/2},{CELL_SIZE,CELL_SIZE*MAP_HEIGHT},TopRight},
	{{OFFSET_X-CELL_SIZE/2,OFFSET_Y-CELL_SIZE/2},{CELL_SIZE*MAP_WIDTH,CELL_SIZE},BottomLeft},
	{{OFFSET_X+CELL_SIZE*(MAP_WIDTH-.5),OFFSET_Y+CELL_SIZE*(MAP_HEIGHT-.5)},{CELL_SIZE,CELL_SIZE*MAP_HEIGHT},BottomLeft},
	{{OFFSET_X+CELL_SIZE*(MAP_WIDTH-.5),OFFSET_Y+CELL_SIZE*(MAP_HEIGHT-.5)},{CELL_SIZE*MAP_WIDTH,CELL_SIZE},TopRight},
};
int TimeTotal=0;
int playerCount =2;
Player* players ;
PlayerUI* playerUIs;
Queue bombs ={NULL,NULL,0};
Queue detonateEffects = {NULL,NULL,0};
int32_t buffer[4];
Rectangle bExit = { 108, 190, 203, 215 };
void endPageInit(){
	drawPos.x = 0;
					drawPos.y = 0;
					winnerPage.drawPoint = drawPos;
					drawImageAtPoint(winnerPage, SCREEN_ROTATION);
					Rectangle winnerPic = { 100, 25, 212, 137 };

					Rectangle winnerTextContainer = { 95, 142, 215, 170 };
					ILI9341_Draw_Filled_Rectangle_Coord(winnerTextContainer.x0,
							winnerTextContainer.y0, winnerTextContainer.x1,
							winnerTextContainer.y1, YELLOW);

		//			Rectangle bReplay = { 55, 175, 150, 200 };
		//			Rectangle bNext = { 155, 175, 250, 200 };


		//			ILI9341_Draw_Filled_Rectangle_Coord(bReplay.x0, bReplay.y0,
		//					bReplay.x1, bReplay.y1, CYAN);
		//			ILI9341_Draw_Filled_Rectangle_Coord(bNext.x0, bNext.y0, bNext.x1,
		//					bNext.y1, GREEN);
					ILI9341_Draw_Filled_Rectangle_Coord(bExit.x0, bExit.y0, bExit.x1,
							bExit.y1, ORANGE);

		//			ILI9341_Draw_Text("<REPLAY", 60, 179, BLACK, 2, CYAN);
		//			ILI9341_Draw_Text("NEXT>", 175, 179, BLACK, 2, GREEN);
					ILI9341_Draw_Text("EXIT", 133, 195, RED, 2, ORANGE);
					//�����褹����
					if(stateWin == 0){
						ILI9341_Draw_Text("WINNER", 103, 143, BLUE, 3, YELLOW);
						drawPos.x = winnerPic.x0;
						drawPos.y = winnerPic.y0;
						character112List[playerWinner[0]->imageIndex].drawPoint = drawPos;
						drawImageAtPoint(character112List[playerWinner[0]->imageIndex], SCREEN_ROTATION);
					}
					//����
					else if(stateWin > 0){
						ILI9341_Draw_Text("DRAW", 120, 143, BLUE, 3, YELLOW);
						//���� 2
						if(stateWin == 1){
							drawPos.x = winnerPic.x0;
							drawPos.y = winnerPic.y0+28;
							character56List[playerWinner[0]->imageIndex].drawPoint = drawPos;
							drawImageAtPoint(character56List[playerWinner[0]->imageIndex], SCREEN_ROTATION);
							drawPos.x = winnerPic.x1-56;
							drawPos.y = winnerPic.y0+28;
							character56List[playerWinner[1]->imageIndex].drawPoint = drawPos;
							drawImageAtPoint(character56List[playerWinner[1]->imageIndex], SCREEN_ROTATION);
						}
						//���� 3
						if(stateWin == 2){
							drawPos.x = winnerPic.x0;
							drawPos.y = winnerPic.y0;
							character56List[playerWinner[0]->imageIndex].drawPoint = drawPos;
							drawImageAtPoint(character56List[playerWinner[0]->imageIndex], SCREEN_ROTATION);
							drawPos.x = winnerPic.x0+56;
							drawPos.y = winnerPic.y0;
							character56List[playerWinner[1]->imageIndex].drawPoint = drawPos;
							drawImageAtPoint(character56List[playerWinner[1]->imageIndex], SCREEN_ROTATION);
							drawPos.x = winnerPic.x0+32;
							drawPos.y = winnerPic.y0+56;
							character56List[playerWinner[2]->imageIndex].drawPoint = drawPos;
							drawImageAtPoint(character56List[playerWinner[2]->imageIndex], SCREEN_ROTATION);

						}
						//���� 4
						if(stateWin == 3){
							drawPos.x = winnerPic.x0;
							drawPos.y = winnerPic.y0;
							character56List[playerWinner[0]->imageIndex].drawPoint = drawPos;
							drawImageAtPoint(character56List[playerWinner[0]->imageIndex], SCREEN_ROTATION);
							drawPos.x = winnerPic.x0+56;
							drawPos.y = winnerPic.y0;
							character56List[playerWinner[1]->imageIndex].drawPoint = drawPos;
							drawImageAtPoint(character56List[playerWinner[1]->imageIndex], SCREEN_ROTATION);
							drawPos.x = winnerPic.x0;
							drawPos.y = winnerPic.y0+56;
							character56List[playerWinner[2]->imageIndex].drawPoint = drawPos;
							drawImageAtPoint(character56List[playerWinner[2]->imageIndex], SCREEN_ROTATION);
							drawPos.x = winnerPic.x0+56;
							drawPos.y = winnerPic.y0+56;
							character56List[playerWinner[3]->imageIndex].drawPoint = drawPos;
							drawImageAtPoint(character56List[playerWinner[3]->imageIndex], SCREEN_ROTATION);
						}
					}
}
void IFDisplayScreen1(){
	drawPos.x = 0;
					drawPos.y = 0;
					settingPage_1.drawPoint = drawPos;
					drawImageAtPoint(settingPage_1, SCREEN_ROTATION);
					ILI9341_Draw_Text("Bomber Man", 10, 10, BLACK, 4, WHITE);
					drawPos.x = 157;
					drawPos.y = 20;
					bombIcon_16.drawPoint = drawPos;
					drawImageAtPoint(bombIcon_16, SCREEN_ROTATION);
					//draw container
					ILI9341_Draw_Filled_Rectangle_Coord(c1.x0, c1.y0, c1.x1, c1.y1,
					CYAN);
					ILI9341_Draw_Filled_Rectangle_Coord(c2.x0, c2.y0, c2.x1, c2.y1,
					YELLOW);
					ILI9341_Draw_Filled_Rectangle_Coord(c3.x0, c3.y0, c3.x1, c3.y1,
					GREEN);
					//draw header text
					ILI9341_Draw_Text("Choose Number Of Players", 60, 53, BLACK, 1,
					CYAN);
					ILI9341_Draw_Text("Choose Character", 80, 103, BLACK, 1, YELLOW);
					ILI9341_Draw_Text("Choose Map", 100, 163, BLACK, 1, GREEN);

					ILI9341_Draw_Text("1 Player", 62, 70, 0x843f, 1, CYAN);
					ILI9341_Draw_Text("2 Player", 152, 70, 0x843f, 1, CYAN);
					numPlayerRectangle[0] = bPlayer_1;
					numPlayerRectangle[1] = bPlayer_2;

					charRectangle[0] = ch1;
					charRectangle[1] = ch2;
					charRectangle[2] = ch3;
					charRectangle[3] = ch4;
					charRectangle[4] = ch5;
					//picture
					for (int i = 0;i<5; i++) {
						drawPos.x = charRectangle[i].x0;
						drawPos.y = charRectangle[i].y0;
						getCharacterYellowList(i)->drawPoint = drawPos;
						drawImageAtPoint(*getCharacterYellowList(i), SCREEN_ROTATION);
					}

					ILI9341_Draw_Filled_Rectangle_Coord(bPlay.x0, bPlay.y0, bPlay.x1,
							bPlay.y1, PINK);
					ILI9341_Draw_Text("PLAY", 108, 211, WHITE, 2, PINK);
					charHighlightRectangle[0] = hil_ch1;
					charHighlightRectangle[1] = hil_ch2;
					charHighlightRectangle[2] = hil_ch3;
					charHighlightRectangle[3] = hil_ch4;
					charHighlightRectangle[4] = hil_ch5;

					mapRectangle[0] = bMap_1;
					mapRectangle[1] = bMap_2;
					mapRectangle[2] = bMap_3;
					int player2_character;
					//play & next button
					//		Rectangle bPlay = { 90, 210, 170, 230 }; //size: 80x20
					while (1) {
						if (TP_Touchpad_Pressed()) {
							uint16_t position_array[2];

							if (TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK) {
								if (SCREEN_ROTATION == SCREEN_HORIZONTAL_1) {
									xPos = position_array[1];
									yPos = SCREEN_HEIGHT - position_array[0];
								} else if (SCREEN_ROTATION == SCREEN_HORIZONTAL_2) {
									xPos = SCREEN_WIDTH - position_array[1];
									yPos = position_array[0];
								}
							}
							cursor.x = xPos;
							cursor.y = yPos;
						}
						for (int i = 0;
								i < sizeof(numPlayerState) / sizeof(numPlayerState[0]);
								i++) {
							if (isTouchWithinRectangle(numPlayerRectangle[i], cursor)) {
								numPlayerState[i] = 1;
								numPlayerState[(i == 0) ? 1 : 0] = 0;
							}
							if (numPlayerState[i]) {
								ILI9341_Draw_Filled_Rectangle_Coord(
										(numPlayerRectangle[i]).x0,
										(numPlayerRectangle[i]).y0,
										(numPlayerRectangle[i]).x1,
										(numPlayerRectangle[i]).y1, BLUE);
								if (i == 0) {
									ILI9341_Draw_Text("1 Player", 62, 70, WHITE, 1,
									BLUE);
									ILI9341_Draw_Filled_Rectangle_Coord(
									                                    bNext.x0,
									                                    bNext.y0,
									                                    bNext.x1,
									                                    bNext.y1, WHITE);
								}

								else {
									ILI9341_Draw_Text("2 Player", 152, 70, WHITE, 1,
									BLUE);
								}

							} else {
								ILI9341_Draw_Filled_Rectangle_Coord(
										(numPlayerRectangle[i]).x0,
										(numPlayerRectangle[i]).y0,
										(numPlayerRectangle[i]).x1,
										(numPlayerRectangle[i]).y1, CYAN);
								ILI9341_Draw_Hollow_Rectangle_Coord(
										(numPlayerRectangle[i]).x0,
										(numPlayerRectangle[i]).y0,
										(numPlayerRectangle[i]).x1,
										(numPlayerRectangle[i]).y1, BLUE);
								if (i == 0)
									ILI9341_Draw_Text("1 Player", 62, 70, BLUE, 1,
									CYAN);
								else
									ILI9341_Draw_Text("2 Player", 150, 70, BLUE, 1,
									CYAN);
							}
						}
						for (int i = 0;
								i
										< sizeof(characterState_2)
												/ sizeof(characterState_2[0]); i++) {
							if (characterState_2[i]) {
								ILI9341_Draw_Hollow_Rectangle_Coord(
										(charHighlightRectangle[i]).x0,
										(charHighlightRectangle[i]).y0,
										(charHighlightRectangle[i]).x1,
										(charHighlightRectangle[i]).y1, BLUE);
								player2_character = i;
								break;
							}
						}
						for (int i = 0;
								i < sizeof(characterState) / sizeof(characterState[0]);
								i++) {
							if (player2_character == i)
								continue;
							if (isTouchWithinRectangle(charHighlightRectangle[i],
									cursor)) {
								characterState[i] = 1;
								for (int j = 0;
										j
												< sizeof(characterState)
														/ sizeof(characterState[0]);
										j++) {
									if (j == i)
										continue;
									characterState[j] = 0;
								}
							}
							if (characterState[i])
								ILI9341_Draw_Hollow_Rectangle_Coord(
										(charHighlightRectangle[i]).x0,
										(charHighlightRectangle[i]).y0,
										(charHighlightRectangle[i]).x1,
										(charHighlightRectangle[i]).y1, RED);
							else
								ILI9341_Draw_Hollow_Rectangle_Coord(
										(charHighlightRectangle[i]).x0,
										(charHighlightRectangle[i]).y0,
										(charHighlightRectangle[i]).x1,
										(charHighlightRectangle[i]).y1, YELLOW);
						}
						checkMap(cursor, mapState, mapRectangle);
						//				if (numPlayerState[0]) {
						//					ILI9341_Draw_Filled_Rectangle_Coord(bPlay.x0, bPlay.y0,
						//							bPlay.x1, bPlay.y1, PINK);
						//					ILI9341_Draw_Text("PLAY", 108, 211, WHITE, 2, PINK);
						//					if (isTouchWithinRectangle(bPlay, cursor)) {
						//						if (checkStartGame())
						//							break;
						//					}
						//				}
						if (numPlayerState[1]) {
							ILI9341_Draw_Filled_Rectangle_Coord(bNext.x0, bNext.y0,
									bNext.x1, bNext.y1, PURPLE);
							ILI9341_Draw_Text(">", 238, 211, WHITE, 2, PURPLE);
							if (isTouchWithinRectangle(bNext, cursor)) {
								break;
							}
						}
						if (checkStartGame(numPlayerState, mapState, characterState,
								characterState_2,&displayScreen)) {
							if (isTouchWithinRectangle(bPlay, cursor)){
								displayScreen = 2;
								startGame();
								break;
							}
						}

						HAL_Delay(100);
					}
}
void IFDisplayScreen2(){
	int player1_character;
					drawPos.x = 250;
					drawPos.y = 0;
					settingPage_2.drawPoint = drawPos;
					drawImageAtPoint(settingPage_2, SCREEN_ROTATION);
					ILI9341_Draw_Filled_Rectangle_Coord(c2.x0, c2.y0, c2.x1, c2.y1,
							0xFBE0);
					ILI9341_Draw_Text("Choose Character Of Player 2", 50, 103, BLACK, 1,
							0xFBE0);
					//			ILI9341_Draw_Filled_Rectangle_Coord(ch1.x0, ch1.y0, ch1.x1, ch1.y1,
					//			RED);
					//			ILI9341_Draw_Filled_Rectangle_Coord(ch2.x0, ch2.y0, ch2.x1, ch2.y1,
					//			RED);
					//			ILI9341_Draw_Filled_Rectangle_Coord(ch3.x0, ch3.y0, ch3.x1, ch3.y1,
					//			RED);
					//			ILI9341_Draw_Filled_Rectangle_Coord(ch4.x0, ch4.y0, ch4.x1, ch4.y1,
					//			RED);
					//			ILI9341_Draw_Filled_Rectangle_Coord(ch5.x0, ch5.y0, ch5.x1, ch5.y1,
					//			RED);
					for (int i = 0;i<5; i++) {
						drawPos.x = charRectangle[i].x0;
						drawPos.y = charRectangle[i].y0;
						getCharacterOrangeList(i)->drawPoint = drawPos;
						drawImageAtPoint(* getCharacterOrangeList(i), SCREEN_ROTATION);
					}
					ILI9341_Draw_Filled_Rectangle_Coord(bNext.x0 - 2, bNext.y0 - 2,
							bNext.x1 + 2, bNext.y1 + 2, WHITE);
					//			ILI9341_Draw_Filled_Rectangle_Coord(bPlay.x0, bPlay.y0, bPlay.x1,
					//					bPlay.y1, PINK);
					//			ILI9341_Draw_Text("PLAY", 108, 211, WHITE, 2, PINK);
					ILI9341_Draw_Filled_Rectangle_Coord(bBack.x0, bBack.y0, bBack.x1,
							bBack.y1, PURPLE);
					ILI9341_Draw_Text("<", 15, 211, WHITE, 2, PURPLE);
					while (1) {
						for (int i = 0;
								i < sizeof(characterState) / sizeof(characterState[0]);
								i++) {
							if (characterState[i]) {
								ILI9341_Draw_Hollow_Rectangle_Coord(
										(charHighlightRectangle[i]).x0,
										(charHighlightRectangle[i]).y0,
										(charHighlightRectangle[i]).x1,
										(charHighlightRectangle[i]).y1, RED);
								player1_character = i;
								break;
							}
						}
						if (TP_Touchpad_Pressed()) {
							uint16_t position_array[2];

							if (TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK) {
								if (SCREEN_ROTATION == SCREEN_HORIZONTAL_1) {
									xPos = position_array[1];
									yPos = SCREEN_HEIGHT - position_array[0];
								} else if (SCREEN_ROTATION == SCREEN_HORIZONTAL_2) {
									xPos = SCREEN_WIDTH - position_array[1];
									yPos = position_array[0];
								}
							}
							cursor.x = xPos;
							cursor.y = yPos;
						}
						for (int i = 0;
								i
										< sizeof(characterState_2)
												/ sizeof(characterState_2[0]); i++) {
							if (player1_character == i)
								continue;
							if (isTouchWithinRectangle(charHighlightRectangle[i],
									cursor)) {
								characterState_2[i] = 1;
								for (int j = 0;
										j
												< sizeof(characterState_2)
														/ sizeof(characterState_2[0]);
										j++) {
									if (j == i)
										continue;
									characterState_2[j] = 0;
								}
							}
							if (characterState_2[i])
								ILI9341_Draw_Hollow_Rectangle_Coord(
										(charHighlightRectangle[i]).x0,
										(charHighlightRectangle[i]).y0,
										(charHighlightRectangle[i]).x1,
										(charHighlightRectangle[i]).y1, BLUE);
							else
								ILI9341_Draw_Hollow_Rectangle_Coord(
										(charHighlightRectangle[i]).x0,
										(charHighlightRectangle[i]).y0,
										(charHighlightRectangle[i]).x1,
										(charHighlightRectangle[i]).y1, 0xFBE0);
						}
						checkMap(cursor, mapState, mapRectangle);
						if (isTouchWithinRectangle(bBack, cursor)) {
							displayScreen = 0;
							break;
						}
						if (isTouchWithinRectangle(numPlayerRectangle[0], cursor)) {
							displayScreen = 0;
							numPlayerState[0] = 1;
							numPlayerState[1] = 0;
							for (int i = 0;
									i
											< sizeof(characterState_2)
													/ sizeof(characterState_2[0]);
									i++) {
								characterState_2[i] = 0;
							}
							break;
						}
						//				if (isTouchWithinRectangle(bPlay, cursor)) {
						//					if (checkStartGame())
						//						break;
						//				}
						if (checkStartGame(numPlayerState, mapState, characterState,
								characterState_2, &displayScreen)) {
							ILI9341_Draw_Filled_Rectangle_Coord(bPlay.x0, bPlay.y0,
									bPlay.x1, bPlay.y1, PINK);
							ILI9341_Draw_Text("PLAY", 108, 211, WHITE, 2, PINK);
							if (isTouchWithinRectangle(bPlay, cursor)){
								displayScreen = 2;
								startGame();
								break;
							}
						}
						HAL_Delay(100);
					}

}
void IFDisplayScreen3(TIM_HandleTypeDef *htim){

	playerWinnerCount=0;
		if(htim==&htim1){
			TimeTotal += 20;
			int deadFlagTotal = 0;
			for(int i=0;i<playerCount;i++){
				if(players[i].deadFlag)
					deadFlagTotal ++;
				else
					playerWinner[playerWinnerCount++]=&players[i];
			}
			if(deadFlagTotal+1<playerCount){
				HAL_ADC_Start_DMA(&hadc1,(uint32_t *)buffer,4);
			}
			else{
				stateWin = playerWinnerCount-1;
				endPageInit();
				displayScreen =3;
				return;
			}
		}
		else if(htim==&htim2){
			clock++;
			if(clock>GAME_TIME){

				displayScreen =3;
				endPageInit();
			}
		}
}
void IFDisplayScreen4(){
	Point cursur;
	if (TP_Touchpad_Pressed()) {
								uint16_t position_array[2];

								if (TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK) {
									if (SCREEN_ROTATION == SCREEN_HORIZONTAL_1) {
										xPos = position_array[1];
										yPos = SCREEN_HEIGHT - position_array[0];
									} else if (SCREEN_ROTATION == SCREEN_HORIZONTAL_2) {
										xPos = SCREEN_WIDTH - position_array[1];
										yPos = position_array[0];
									}
								}
								cursor.x = xPos;
								cursor.y = yPos;
								if(isTouchWithinRectangle(bExit, cursur ))
										displayScreen =1;
	}

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (displayScreen == 0) {
		IFDisplayScreen1();
	}
	if(displayScreen == 1){
		IFDisplayScreen2();
	}
	if (displayScreen == 2) {
		IFDisplayScreen3(htim);
	}
	if (displayScreen == 3) {
		IFDisplayScreen4();
	}

}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	int time=TimeTotal;

	while(bombs.size&&((Bomb*)bombs.front->value)->detonateTime<time){

		detonate((Bomb*)bombs.front->value);
	}
	while(detonateEffects.size&&((DetonateEffect*)detonateEffects.front->value)->deconstructTime<time){

		clearObject(((DetonateEffect*)detonateEffects.front->value)->index,true);
	}
	for(int i=0;i<playerCount;i++){
		if(players[i].immortalFlag&&time-players[i].initialImmortalTime>IMMORTAL_DURATION)
			players[i].immortalFlag = false;
	}
	updateMap();
	for(int i=0,humanPlayerCount =0;i<playerCount;i++){
		if(players[i].botFlag){
			if(!players[i].deadFlag){
				Vector2 dst=Sub( toPosition(findNextIndex(&players[i])) ,players[i].transform.position);
				dst.x = clamp(-1,dst.x,1);
				dst.y = clamp(-1,dst.y,1);
				Move(&players[i],dst);
			}
		}
		else{
			int num =(humanPlayerCount == 0)?0:2;
			if(humanPlayerCount<2){
				if(!players[i].deadFlag)
					Move(&players[i],Vector2_init(mappingValue(buffer[num],0,0xfff,-3,3) ,mappingValue(buffer[num+1],0,0xfff,3,-3)));
			}

			humanPlayerCount++;
		}
	}

	drawPlayers(players,playerCount);
	for(int i=0;i<playerCount;i++)
		DrawPlayerUI(&playerUIs[i]);
	Vector2 clockPosition = Vector2_init(UI_MARGIN,10);
	int timeLeft = GAME_TIME - clock;
	int minute = timeLeft/60,second=timeLeft%60;
	if(second<10)
		sprintf(str,"%d:0%d",minute,second);
	else
		sprintf(str,"%d:%d",minute,second);
	ILI9341_Draw_Text(str,clockPosition.x,clockPosition.y, WHITE, 2, BLACK);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	int humanTotal = 0;
	int val=-1;
	if(GPIO_Pin==GPIO_PIN_3){ val =1; }
	else if(GPIO_Pin==GPIO_PIN_4){ val =2; }
	if((GPIO_Pin==GPIO_PIN_3||GPIO_Pin==GPIO_PIN_4)&&val!=-1){
		for(int i=0;i<playerCount;i++){
			if(!players[i].botFlag){
				humanTotal++;
				if(humanTotal == val){
					placeBomb(&players[i]);
					break;
				}
			}
		}
	}
	if(GPIO_Pin==GPIO_PIN_10&&displayScreen==3){
		//displayScreen = 0;

	}
}
void SetPlayers(int humanCount,int player1Index,int player2Index,bool* isBot){
	playerCount = 2;
	players = calloc(playerCount,sizeof(Player));
	playerUIs = calloc(playerCount,sizeof(PlayerUI));
	bool b[5] = {false,false,false,false};
	int ind[4]={0,0,0,0};
	Vector2 startPosition[4]={
		Vector2_init(0,0),
		Vector2_init(0,MAP_HEIGHT-1),
		Vector2_init(MAP_WIDTH-1,0),
		Vector2_init(MAP_WIDTH-1,MAP_HEIGHT-1)
	};
	ind[0]=player1Index;
	b[player1Index-1]=true;
	if(humanCount==2){
		ind[1]=player2Index;
		b[player2Index-1] = true;
	}
	for(int i=1;i<playerCount;i++){
		if(ind[i]==0){
			for(int j=0;j<5;j++){
				if(!b[j]){
					ind[i] = j+1;
					b[j]=true;
					break;
				}
			}
		}
	}
	for(int i=0;i<playerCount;i++){
		players[i] = Player_init(startPosition[i],ind[i]-1,isBot[i]);

	}
	for(int i=0;i<playerCount;i++){
		playerUIs[i] = PlayerUI_init(&players[i],Vector2_init(5,40+(16+UI_MARGIN)*3*i));
	}
	Transform UI_Bg =Transform_init(Vector2_init(0,0),Vector2_init(5+(16+UI_MARGIN)*5,240),TopLeft);
	Draw(&UI_Bg,BLACK);
}

void startGame(){
	int playerCount;
							for(int i=0;i<2;i++){
								if(numPlayerState[i]){
									playerCount = i+1;
								}
							}
							int player1Index;
							for(int i=0;i<5;i++){
								if(characterState[i]){
									player1Index = i+1;
								}
							}
							int player2Index;
							for(int i=0;i<5;i++){
								if(characterState_2[i]){
									player2Index = i+1;
								}
							}
							int mapIndex;
							for(int i=0;i<3;i++){
								if(mapState[i]){
									mapIndex = i;
								}
							}
	for(int i=0;i<mapSize.y;i++){
		for(int j=0;j<mapSize.x;j++){
			Vector2 index = Vector2_init(j,i);
			map[i][j].floor = Transform_init(toPosition(index),cellSize,MiddleCenter);
		}
	}
	ILI9341_Fill_Screen(0x5AED);
	setMap(mapIndex);

	bool isBot[4]={false,false,true,true};
	 isBot[1] = playerCount==1?true:false;
	SetPlayers(playerCount,player1Index,player2Index,isBot);
	HAL_TIM_Base_Start_IT(&htim2);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_RNG_Init();
  MX_ADC1_Init();
  MX_SPI5_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  ILI9341_Init();//initial driver setup to drive ili9341
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  Image arr1[5]	= { playerIcon1_56, playerIcon2_56,playerIcon3_56, playerIcon4_56, playerIcon5_56 };
  Image arr2[5] = { playerIcon1_112, playerIcon2_112,playerIcon3_112, playerIcon4_112, playerIcon5_112 };
  for(int i=0;i<5;i++){
	  character56List[i]= arr1[i];
	  character112List[i]=arr2[i];
  }
  srand(HAL_GetTick());
  ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
  HAL_TIM_Base_Start_IT(&htim1);
  while (1)
  {

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
