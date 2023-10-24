/*
 * gameUtility.h
 *
 *  Created on: Oct 14, 2023
 *      Author: vorapolpromwan
 */
#pragma once
#include <main.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include "utility.h"
#include "icons.h"
#define MAP_WIDTH 13
#define MAP_HEIGHT 13
#define CELL_SIZE 16
#define ITEM_SIZE 18
#define OFFSET_X 104
#define OFFSET_Y 30
#define PLAYER_WIDTH 10
#define PLAYER_HEIGHT 10
#define BOMB_DELAY 3000
#define DETONATE_DURATION 1000
#define IMMORTAL_DURATION 3000
#define BLINK_PERIOD 700
#define ITEM_DROP_CHANCE 60
#define UI_MARGIN 2
#define PAGE_SPACE 10
typedef enum PivotTypeEnum{
	TopLeft=0,
	TopCenter=1,
	TopRight=2,
	MiddleLeft=3,
	MiddleCenter=4,
	MiddleRight=5,
	BottomLeft=6,
	BottomCenter=7,
	BottomRight=8
}PivotType;
typedef struct Vector2Struct{
	int x,y;
}Vector2;
Vector2 Vector2_init(int x,int y);
Vector2 Add(Vector2 v1,Vector2 v2);
Vector2 Sub(Vector2 v1,Vector2 v2);
Vector2 Multiply_int(Vector2 v1,int m);
Vector2 Multiply_vector(Vector2 v1,Vector2 v2);
Vector2 Divide_int(Vector2 v1,int d);
Vector2 Divide_vector(Vector2 v1,Vector2 v2);
bool Equal(Vector2 v1,Vector2 v2);
void printVector(Vector2 v);
typedef struct TransformStruct{
	Vector2 position;
	Vector2 size;
	PivotType pivot;
}Transform;
Transform Transform_init(Vector2 position,Vector2 size,PivotType type);
int getLeft(Transform* trans);
int getRight(Transform* trans);
int getTop(Transform* trans);
int getBottom(Transform* trans);
Vector2 getTransformOrigin(Transform* transform);
void Draw(Transform* trans,uint16_t color);
void DrawImage(Transform* transform,uint8_t* arr);
Vector2 toIndex(Vector2 position);
Vector2 toPosition(Vector2 index);
bool isCollision(Transform *trans1,Transform *trans2,Vector2 *vectorOutput);
typedef struct PlayerStruct{
	Transform transform;
	int health;
	int speed;
	int maxBombCount;
	int bombRange;
	int currentUseBomb;
	bool immortalFlag;
	int initialImmortalTime;
	bool deadFlag;
	char* imgPtr1;
	char* imgPtr2;
	char* imgPtr3;
	char* imgPtr4;
	int imageIndex;
	bool botFlag;
}Player;
Player Player_init(Vector2 index,int imgIndex,bool botFlag);
Vector2 getPlayerIndex(Player* player);
void drawPlayers(Player* players,int count);
bool CheckPlayerCollisionAt(Player* player,Vector2 index);
void Move(Player* player,Vector2 moveDst);
typedef enum ObjectTypeEnum{
	None=0,
	UnbreakableWall=1,
	BreakableWall=2,
	BombObject=3,
	DetonateEffectObject=4,
	SpeedItem=5,
	BombAmountItem=6,
	BombRangeItem=7,
	HealthItem=8
}ObjectType;
typedef struct MapCellStruct{
	Transform floor;
	Transform* objTrans;
	ObjectType objType;
	void *objPtr;
}MapCell;
MapCell* getCell(Vector2 index);
void drawCell(Vector2 index);
void setObject(Vector2 index,ObjectType type,Player* player);
void setMap(int num);
placeBomb(Player* player);
typedef struct BombStruct{
	Vector2 index;
	int detonateTime;
	int range;
	Player* player;
}Bomb;
detonate(Bomb* bomb);
typedef struct DetonateEffectStruct{
	Vector2 index;
	int deconstructTime;
	ObjectType drop;
}DetonateEffect;

void updateMap();
Vector2 findNextIndex(Player* player);
typedef struct PlayerUIStruct{
	Player* player;
	Transform charactorTrans;
	Transform healthImgTrans;
	Transform speedImgTrans;
	Transform bombAmountImgTrans;
	Transform bombRangeImgTrans;
	Vector2 startTextPos;
}PlayerUI;
PlayerUI PlayerUI_init(Player* player,Vector2 offset);
void DrawPlayerUI(PlayerUI* playerUI);
#ifndef SRC_GAMEUTILITY_H_
#define SRC_GAMEUTILITY_H_



#endif /* SRC_GAMEUTILITY_H_ */
