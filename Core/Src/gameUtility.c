/*
 * gameUtility.c
 *  Created on: Oct 14, 2023
 *
 *      Author: vorapolpromwan
 */
#include "gameUtility.h"
#include "stdio.h"
#include "map.h"
#include "ILI9341_Touchscreen.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "spi.h"

extern Vector2 offset;
extern Vector2 mapSize;
extern Vector2 cellSize;
extern Vector2 itemSize;
extern MapCell map[MAP_HEIGHT][MAP_WIDTH];
extern Queue bombs;
extern Queue detonateEffects;
extern Transform walls[4];
extern char str[90];
extern int playerCount;
extern Player* players ;
extern int TimeTotal;
extern int clock;

//Vector Start
printVector(Vector2 v){
	char str[40];
	sprintf(str,"vector x: %d y: %d\n",v.x,v.y);
	printOut(str);
}
Vector2 Vector2_init(int x,int y){
	Vector2 v = {x,y};
	return v;
}
Vector2 Add(Vector2 v1,Vector2 v2){
	v1.x+=v2.x;
	v1.y+=v2.y;
	return v1;
}
Vector2 Sub(Vector2 v1,Vector2 v2){
	v1.x-=v2.x;
	v1.y-=v2.y;
	return v1;
}
Vector2 Multiply_int(Vector2 v1,int m){
	v1.x*=m;
	v1.y*=m;
	return v1;
}
Vector2 Multiply_vector(Vector2 v1,Vector2 v2){
	v1.x*=v2.x;
	v1.y*=v2.y;
	return v1;
}
Vector2 Divide_int(Vector2 v1,int d){
	v1.x/=d;
	v1.y/=d;
	return v1;
}
Vector2 Divide_vector(Vector2 v1,Vector2 v2){
	v1.x/=v2.x;
	v1.y/=v2.y;
	return v1;
}
bool Equal(Vector2 v1,Vector2 v2){
	return (v1.x == v2.x)&&(v1.y==v2.y);
}
Vector2 getOffsetOrigin(){
	return Sub(offset,Divide_int(cellSize,2)) ;
}
Vector2 toIndex(Vector2 position){
	return Divide_vector(Sub(position,getOffsetOrigin()),cellSize);
}
Vector2 toPosition(Vector2 index){
	return Add(offset,Multiply_vector(cellSize,index));
}
//Vector End

//Transform Start
Transform Transform_init(Vector2 position,Vector2 size,PivotType type){
	Transform trans;
	trans.position = position;
	trans.size = size;
	trans.pivot = type;
	return trans;
}
int getLeft(Transform* trans){
	switch(trans->pivot%3){
	case 0 :
		return trans->position.x;
		break;
	case 1 :
		return trans->position.x - trans->size.x/2;
		break;
	case 2 :
		return trans->position.x - trans->size.x;
		break;
	}
}
int getRight(Transform* trans){
	return getLeft(trans)+trans->size.x;
}
int getTop(Transform* trans){
	switch(trans->pivot/3){
		case 0 :
			return trans->position.y;
			break;
		case 1 :
			return trans->position.y - trans->size.y/2;
			break;
		case 2 :
			return trans->position.y - trans->size.y;
			break;
		}
}
int getBottom(Transform* trans){
	return getTop(trans)+trans->size.y;
}
Vector2 getTransformOrigin(Transform* transform){
	return Vector2_init(getLeft(transform),getTop(transform));
}

void Draw(Transform* trans,uint16_t color){
	Vector2 origin = getTransformOrigin(trans);
	Vector2 endPos = Add(origin,trans->size) ;
	ILI9341_Draw_Filled_Rectangle_Coord(origin.x,origin.y,endPos.x,endPos.y,color);
}
void DrawImage(Transform* transform,uint8_t* arr)
{
    Vector2 origin = getTransformOrigin(transform);
    Vector2 endPoint = Add(origin,transform->size);

    ILI9341_Set_Address(origin.x, origin.y, endPoint.x-1, endPoint.y-1);

    HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);

    unsigned char Temp_small_buffer[BURST_MAX_SIZE];
    uint32_t counter = 0;
    for (uint32_t i = 0; i < transform->size.x * transform->size.y * 2 / BURST_MAX_SIZE; i++)
    {
        for (uint32_t k = 0; k < BURST_MAX_SIZE; k++)
        {
            Temp_small_buffer[k] = arr[counter + k];
        }
        HAL_SPI_Transmit(HSPI_INSTANCE, (unsigned char *)Temp_small_buffer, BURST_MAX_SIZE, 10);
        counter += BURST_MAX_SIZE;
    }
    HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
}
bool isCollision(Transform *trans1,Transform *trans2,Vector2 *vectorOutput){
	if(trans1==NULL||trans2==NULL)
		return false;
	int r1 = getRight(trans1),r2=getRight(trans2),l1=getLeft(trans1),l2=getLeft(trans2);
	int t1 = getTop(trans1),t2=getTop(trans2),b1=getBottom(trans1),b2=getBottom(trans2);
	Vector2 v = {0,0};
	if((inBetween(l2+1,r1,r2)||inBetween(l2,l1,r2-1)||inBetween(l1,l2,r1-1))&&
			(inBetween(t2+1,b1,b2)||inBetween(t2,t1,b2-1)||inBetween(t1,t2,b1-1))){
		int dst1=l2-r1,dst2=r2-l1;
		int dstX = abs(dst1)<abs(dst2)?dst1:dst2;
		dst1=t2-b1,dst2=b2-t1;
		int dstY = abs(dst1)<abs(dst2)?dst1:dst2;
		*vectorOutput = abs(dstX)<abs(dstY)?Vector2_init(dstX,0) :Vector2_init(0,dstY);
		return true;
	}
	return false;
}
void Move(Player* player,Vector2 moveDst){
	Vector2 dst;
	player->transform.position = Add(player->transform.position,moveDst);
	Vector2 index = getPlayerIndex(player);
	if(index.x>0)
		CheckPlayerCollisionAt(player,Add(index,Vector2_init(-1,0)));
	if(index.y>0)
		CheckPlayerCollisionAt(player,Add(index,Vector2_init(0,-1)));
	if(index.x<MAP_WIDTH-1)
			CheckPlayerCollisionAt(player,Add(index,Vector2_init(1,0)));
	if(index.y<MAP_HEIGHT-1)
		CheckPlayerCollisionAt(player,Add(index,Vector2_init(0,1)));
	if(index.x>0){
		if(index.y>0){
			CheckPlayerCollisionAt(player,Add(index,Vector2_init(-1,-1)));
		}
		if(index.y<MAP_HEIGHT-1){
			CheckPlayerCollisionAt(player,Add(index,Vector2_init(-1,1)));
		}
	}
	if(index.x<MAP_WIDTH-1){
		if(index.y>0){
			CheckPlayerCollisionAt(player,Add(index,Vector2_init(1,-1)));
		}
		if(index.y<MAP_HEIGHT-1){
			CheckPlayerCollisionAt(player,Add(index,Vector2_init(1,1)));
		}
	}
	for(int i=0;i<4;i++){
		if(isCollision(&player->transform,&walls[i],&dst)){
			player->transform.position = Add(player->transform.position,dst);
		}
	}
	for(int i=0;i<playerCount;i++){
		if(player!=&players[i]&&isCollision(&player->transform,&getCell(getPlayerIndex(&players[i]))->floor,&dst)){
			player->transform.position = Add(player->transform.position,dst);
		}
	}
	ObjectType type = getCell(index)->objType;
	switch(type){
		case SpeedItem :
			if(player->speed<5)
				player->speed++;
			clearObject(index,false);
			break;
		case BombAmountItem:
			if(player->maxBombCount<5)
				player->maxBombCount++;
			clearObject(index,false);
			break;
		case BombRangeItem:
			if(player->bombRange<5)
				player->bombRange++;
			clearObject(index,false);
			break;
		case HealthItem:
			if(player->health<5)
				player->health++;
			clearObject(index,false);
			break;
		case DetonateEffectObject:
			if(!player->immortalFlag){
				player->health--;
				player->immortalFlag = true;
				player->initialImmortalTime = TimeTotal;
				if(player->health<=0)
					player->deadFlag = true;
			}
			break;
		default:break;
	}

}
bool CheckPlayerCollisionAt(Player* player,Vector2 index){
	Vector2 dst;
	if((
		getCell(index)->objType == BombObject||
		getCell(index)->objType == BreakableWall||
		getCell(index)->objType == UnbreakableWall)&&
		isCollision(&player->transform,getCell(index),&dst)
	){
		player->transform.position = Add(player->transform.position,dst);
	}
}
void drawPlayers(Player* players,int count){
	Vector2 playerIndexs[count];
	for(int i=0;i<count;i++){
		playerIndexs[i]=getPlayerIndex(&players[i]);
	}
	for(int i=0;i<count;i++){
		Player* player = &players[i];
		if(!player->deadFlag){
			int delta = (TimeTotal-player->initialImmortalTime)%BLINK_PERIOD;
			Vector2 playerIndex = playerIndexs[i];
			Vector2 minIndex = Vector2_init(playerIndex.x>0?playerIndex.x-1:0,playerIndex.y>0?playerIndex.y-1:playerIndex.y);
			Vector2 maxIndex = Vector2_init(playerIndex.x<MAP_WIDTH-1?playerIndex.x+1:playerIndex.x,playerIndex.y<MAP_HEIGHT-1?playerIndex.y+1:playerIndex.y);
				for(Vector2 index = minIndex;index.y<=maxIndex.y;index.y++,index.x=minIndex.x){
					for(;index.x<=maxIndex.x;index.x++){
						bool flag = false;
						for(int j=0;j<count;j++){
							if(Equal(playerIndexs[j],index)){
								flag =true;
							}
						}
					if(!flag){
						drawCell(index);
					}

				}
			}
			if(!player->immortalFlag||delta>BLINK_PERIOD/2){

				MapCell* cell= getCell(playerIndexs[i]);
				if(cell->objType == BombObject)
					DrawImage(&cell->floor,player->imgPtr2);
				else
					DrawImage(&cell->floor,player->imgPtr1);
			}
			else{
				drawCell(playerIndexs[i]);
			}
		}
	}
	/**/
}
//Transform End

//Map Cell Start
void drawCell(Vector2 index){
	MapCell* cell = getCell(index);
	switch(cell->objType){
		case None:
			DrawImage(&cell->floor,&grassImage);
			break;
		case BombObject:
			DrawImage(cell->objTrans,&bombImage);
			break;
		case DetonateEffectObject:
			Draw(cell->objTrans,ORANGE);
			break;
		case UnbreakableWall:
			DrawImage(cell->objTrans,&wallImage1);
			break;
		case BreakableWall:
			DrawImage(cell->objTrans,&wallImage2);
			break;
		case HealthItem:
			DrawImage(cell->objTrans,&heartImage);
			break;
		case BombAmountItem:
			DrawImage(cell->objTrans,&addBombImage);
			break;
		case BombRangeItem:
			DrawImage(cell->objTrans,&potionImage);
			break;
		case SpeedItem:
			DrawImage(cell->objTrans,&shoeImage);
			break;
	}
}
void setMap(int num){
	int *mapArr;
	if(num==1){
		mapArr = map1;
	}
	else if(num == 2){
		mapArr = map2;
	}
	else if(num == 3){
		mapArr = map3;
	}
	for(int i = 0;i<MAP_HEIGHT;i++){
		for(int j=0;j<MAP_WIDTH;j++){
			Vector2 index = Vector2_init(j,i);
			if(mapArr[i*MAP_WIDTH+j]){
				setObject(index,(ObjectType)mapArr[i*MAP_WIDTH+j],NULL);
			}
			else{
				drawCell(index);
			}
		}
	}
}
void setObject(Vector2 index,ObjectType type,Player* player){
	clearObject(index,false);
	MapCell *cell =&map[index.y][index.x];
	switch(type){
		case None: break;
		case BombObject:
			player->currentUseBomb++;
			Bomb* bomb = malloc(sizeof(Bomb));
			bomb->index = index;
			bomb->range = player->bombRange;
			bomb->detonateTime = TimeTotal+BOMB_DELAY;
			bomb->player = player;
			QueueElement* element1 = enqueue(&bombs,(void*)bomb);
			cell->objPtr = (void*)element1;
			cell->objTrans = malloc(sizeof(Transform));
			*cell->objTrans = Transform_init(toPosition(index),cellSize,MiddleCenter);
			break;
		case DetonateEffectObject:
			DetonateEffect* detonateEffect = malloc(sizeof(DetonateEffect));
			detonateEffect->index = index;
			detonateEffect->deconstructTime = TimeTotal+DETONATE_DURATION;
			detonateEffect->drop = cell->objType;
			QueueElement* element2 = enqueue(&detonateEffects,(void*)detonateEffect);
			cell->objPtr = (void*)element2;
			cell->objTrans = malloc(sizeof(Transform));
			*cell->objTrans = Transform_init(toPosition(index),cellSize,MiddleCenter);
		case UnbreakableWall:
		case BreakableWall :
			cell->objTrans = malloc(sizeof(Transform));
			*cell->objTrans = Transform_init(toPosition(index),cellSize,MiddleCenter);
			break;
		case SpeedItem:
		case BombAmountItem:
		case BombRangeItem:
		case HealthItem :
			cell->objTrans = malloc(sizeof(Transform));
			*cell->objTrans = Transform_init(toPosition(index),cellSize,MiddleCenter);
			break;
	}
	cell->objType = type;
	drawCell(index);
}
void clearObject(Vector2 index,bool redrawFlag){
	MapCell *cell =&map[index.y][index.x];
	if(cell->objType != None){
		Transform* objTrans = cell->objTrans;
		void* objPtr = cell->objPtr;
		ObjectType objType = cell->objType;
		cell->objTrans = NULL;
		cell->objPtr = NULL;
		cell->objType = None;
		switch(objType){
			case BombObject:
				QueueElement* element=(QueueElement*)objPtr;
				((Bomb*)element->value)->player->currentUseBomb--;
				DeleteQueueElement(&bombs,element);
				break;
			case DetonateEffectObject:
				ObjectType drop = ((DetonateEffect*)((QueueElement*)objPtr)->value)->drop;
				DeleteQueueElement(&detonateEffects,(QueueElement*)objPtr);
				setObject(index,drop,NULL);
				break;
			case BreakableWall:
				int random = (int)rand()%100;
				if(random<=ITEM_DROP_CHANCE){
					int dropItemCount = 4;
					int range=ITEM_DROP_CHANCE/dropItemCount, mxRange=range;
					ObjectType itemDropTypes[]={
							SpeedItem,
							BombAmountItem,
							BombRangeItem,
							HealthItem,
					};
					for(int i=0;i<dropItemCount;i++,mxRange+=range){
						if(random<=mxRange){
							setObject(index,itemDropTypes[i],NULL);
							break;
						}
					}
				}
				break;
			default:
				if(objPtr != NULL)
					free(objPtr);
				break;
		}
		free(objTrans);

		if(redrawFlag)
			drawCell(index);
	}
}
placeBomb(Player* player){
	if(player!= NULL && getCell(getPlayerIndex(player))->objType != BombObject&&player->currentUseBomb<player->maxBombCount){
		setObject(getPlayerIndex(player),BombObject,player);
	}
}
MapCell* getCell(Vector2 index){
	if(inBetween(0,index.x,mapSize.x-1)&&inBetween(0,index.y,mapSize.y-1)){
		return &map[index.y][index.x];
	}
	else{
		return NULL;
	}
}
detonate(Bomb* bomb){
	if(bomb == NULL)
		return;
	int range = bomb->range;
	Vector2 startIndex = bomb->index;
	Vector2 index = startIndex;
	setObject(startIndex,DetonateEffectObject,NULL);
	for(int i=0;i<4;i++){
		index = startIndex;
		Vector2 increment = Vector2_init(i&0b10?(i&0b1?1:-1):0,i&0b10?0:(i&0b1?1:-1));
		index = Add(index,increment);
		MapCell* cell = getCell(index);
		for(int j=0;j<range&&cell!=NULL;j++,index=Add(index,increment),cell = getCell(index)){
			switch(cell->objType){
				case BreakableWall:
					setObject(index ,DetonateEffectObject,NULL);
				case UnbreakableWall:
					j=range;
					break;
				case BombObject:
					detonate((Bomb*)((QueueElement*)cell->objPtr)->value);
					break;
				default:
					setObject(index ,DetonateEffectObject,NULL);
					break;
			}
		}
	}
}
//Map Cell End

//Player Start
Player Player_init(Vector2 index,int imgIndex,bool botFlag){
	char *img16[5]={
			playerImage1,
			playerImage2,
			playerImage3,
			playerImage4,
			playerImage5
	};
	char *img16Bomb[5]={
		playerImage1Bomb,
		playerImage2Bomb,
		playerImage3Bomb,
		playerImage4Bomb,
		playerImage5Bomb
	};
		char *img56[5]={
			playerImage1White_56,
			playerImage2White_56,
			playerImage3White_56,
			playerImage4White_56,
			playerImage5White_56
		};
		char *img112[5]={
			playerImage1_112,
			playerImage2_112,
			playerImage3_112,
			playerImage4_112,
			playerImage5_112,
		};
	Player player;
	player.transform = Transform_init(toPosition(index),Vector2_init(PLAYER_WIDTH,PLAYER_HEIGHT) ,MiddleCenter);
	player.health = 2;
	player.speed =1;
	player.maxBombCount = 1;
	player.bombRange =1;
	player.currentUseBomb =0;
	player.deadFlag = false;
	player.imgPtr1 = img16[imgIndex];
	player.imgPtr2 = img16Bomb[imgIndex];
	player.imgPtr3 = img56[imgIndex];
	player.imgPtr4 = img112[imgIndex];
	player.imageIndex = imgIndex;
	player.botFlag = botFlag;
	return player;
}
Vector2 getPlayerIndex(Player* player){
	return toIndex(player->transform.position);
}
//Player End

bool dangerMap[MAP_HEIGHT][MAP_WIDTH];
bool walkMap[MAP_HEIGHT][MAP_WIDTH];
int bombRangeMap[MAP_HEIGHT][MAP_WIDTH][4];
typedef enum DirectionEnum{
	Up,Down,Left,Right
}Direction;
Vector2 getIndexAtDirection(Vector2 index,Direction direction){
	if(direction&0b10)
		return Add(index,Vector2_init(direction&0b1?1:-1,0));
	else
		return Add(index,Vector2_init(0,direction&0b1?1:-1));
}
void updateMap(){
	for(Vector2 index = Vector2_init(0,0);index.y<MAP_HEIGHT;index.y++){
		for(index.x=0;index.x<MAP_WIDTH;index.x++){
			bombRangeMap[index.y][index.x][Up]=0;
			bombRangeMap[index.y][index.x][Down]=0;
			bombRangeMap[index.y][index.x][Left]=0;
			bombRangeMap[index.y][index.x][Right]=0;
			dangerMap[index.y][index.x] = false;
			ObjectType type=getCell(index)->objType;
			switch(type){
				case BreakableWall:
				case UnbreakableWall:
				case BombObject:
					walkMap[index.y][index.x]=true;
					break;
				case DetonateEffectObject:
					dangerMap[index.y][index.x]=true;
				default:
					walkMap[index.y][index.x]=false;
					break;
			}
		}
	}
	for(Vector2 index = Vector2_init(0,0);index.y+1<MAP_HEIGHT;index.y++){
		for(index.x=0;index.x+1<MAP_WIDTH;index.x++){
			ObjectType type =getCell(index)->objType;
			if(type == BreakableWall){
				bombRangeMap[index.y][index.x+1][Left]=1;
				bombRangeMap[index.y+1][index.x][Up]=1;
			}
			else if(type != UnbreakableWall){
				if(bombRangeMap[index.y][index.x][Left]>0){
					bombRangeMap[index.y][index.x+1][Left]=bombRangeMap[index.y][index.x][Left]+1;
				}
				if(bombRangeMap[index.y][index.x][Up]>0){
					bombRangeMap[index.y+1][index.x][Up]=bombRangeMap[index.y][index.x][Up]+1;
				}
			}
		}
	}
	for(Vector2 index = Vector2_init(0,MAP_HEIGHT-1);index.x+1<MAP_WIDTH;index.x++){
		ObjectType type =getCell(index)->objType;
		if(type == BreakableWall){
			bombRangeMap[index.y][index.x+1][Left]=1;
		}
		else if(type != UnbreakableWall&&bombRangeMap[index.y][index.x][Left]){
			bombRangeMap[index.y][index.x+1][Left]=bombRangeMap[index.y][index.x][Left]+1;
		}
	}
	for(Vector2 index = Vector2_init(MAP_WIDTH-1,0);index.y+1<MAP_HEIGHT;index.y++){
		ObjectType type =getCell(index)->objType;
		if(type == BreakableWall){
			bombRangeMap[index.y+1][index.x][Up]=1;
		}
		else if(type != UnbreakableWall&&bombRangeMap[index.y][index.x][Up]){
			bombRangeMap[index.y+1][index.x][Up]=bombRangeMap[index.y][index.x][Up]+1;
		}
	}
	for(Vector2 index = Vector2_init(MAP_WIDTH-1,MAP_HEIGHT-1);index.y>0;index.y--){
			for(index.x=MAP_WIDTH-1;index.x>0;index.x--){
				ObjectType type =getCell(index)->objType;
				if(type == BreakableWall){
					bombRangeMap[index.y][index.x-1][Right]=1;
					bombRangeMap[index.y-1][index.x][Down]=1;
				}
				else if(type != UnbreakableWall){
					if(bombRangeMap[index.y][index.x][Right]){
						bombRangeMap[index.y][index.x-1][Right]=bombRangeMap[index.y][index.x][Right]+1;
					}
					if(bombRangeMap[index.y][index.x][Down]){
						bombRangeMap[index.y-1][index.x][Down]=bombRangeMap[index.y][index.x][Down]+1;
					}
				}
			}
		}
	for(Vector2 index = Vector2_init(MAP_WIDTH-1,0);index.x>0;index.x--){
			ObjectType type =getCell(index)->objType;
			if(type == BreakableWall){
				bombRangeMap[index.y][index.x-1][Right]=1;
			}
			else if(type != UnbreakableWall&&bombRangeMap[index.y][index.x][Right]){
				bombRangeMap[index.y][index.x-1][Right]=bombRangeMap[index.y][index.x][Right]+1;
			}
		}
		for(Vector2 index = Vector2_init(0,MAP_HEIGHT-1);index.y>0;index.y--){
			ObjectType type =getCell(index)->objType;
			if(type == BreakableWall){
				bombRangeMap[index.y-1][index.x][Down]=1;
			}
			else if(type != UnbreakableWall&&bombRangeMap[index.y][index.x][Down]){
				bombRangeMap[index.y-1][index.x][Down]=bombRangeMap[index.y][index.x][Down]+1;
			}
		}
	QueueElement* element = bombs.front;
	while(element != NULL){
		Bomb* bomb = (Bomb*)element->value;
		Vector2 startIndex = bomb->index,index;
		int range = bomb->range;
		element =element->next;
		dangerMap[startIndex.y][startIndex.x]=true;
		for(int i=0;i<4;i++){
			index = getIndexAtDirection(startIndex,(Direction)i);
			for(int j=0;j<range&&inBetween(0,index.x,MAP_WIDTH-1)&&inBetween(0,index.y,MAP_HEIGHT-1)&&!walkMap[index.y][index.x];j++,index=getIndexAtDirection(index,(Direction)i)){
				dangerMap[index.y][index.x]=true;
			}
		}
	}
	for(int i=0;i<playerCount;i++){
		Vector2 index=getPlayerIndex(&players[i]);
		walkMap[index.y][index.x]=true;
	}

}
typedef struct BFSValueStruct{
	Vector2 index;
	int weight;
	Direction startDirection;
}BFSValue;
BFSValue* BFSValue_init(Vector2 index,int weight,Direction startDirection){
	BFSValue* val=(BFSValue*)malloc(sizeof(BFSValue ));
	val->index = index;
	val->weight = weight;
	val->startDirection = startDirection;
	return val;
}
Vector2 findNextIndex(Player* player){
	Vector2 startIndex = getPlayerIndex(player);
	bool havePasts[MAP_HEIGHT][MAP_WIDTH];
	for(int i=0;i<MAP_HEIGHT;i++){
		for(int j=0;j<MAP_WIDTH;j++){
			havePasts[i][j]=false;
		}
	}
	havePasts[startIndex.y][startIndex.x]=true;
	if(dangerMap[startIndex.y][startIndex.x]){
		Queue q = {NULL,NULL,0};
		Vector2 index;
		for(int i=0;i<4;i++){
			index = getIndexAtDirection(startIndex,(Direction)i);
			if(inBetween(0,index.y,MAP_HEIGHT-1)&&inBetween(0,index.x,MAP_WIDTH-1)&&
					!walkMap[index.y][index.x]&&!havePasts[index.y][index.x]&&
					(getCell(index)->objType!=DetonateEffectObject||player->immortalFlag)){
				if(!dangerMap[index.y][index.x]){
					while(q.size!=0)
						dequeue(&q);
					return index;
				}
				else{
					havePasts[index.y][index.x]=true;
					enqueue(&q,BFSValue_init(index,1,(Direction)i));
				}
			}
		}
		while(q.size!=0){
			BFSValue* val = (BFSValue*)q.front->value;
			for(int i=0;i<4;i++){
				index = getIndexAtDirection(val->index,(Direction)i);
				if(inBetween(0,index.y,MAP_HEIGHT-1)&&inBetween(0,index.x,MAP_WIDTH-1)
						&&!walkMap[index.y][index.x]&&!havePasts[index.y][index.x]&&
						(getCell(index)->objType!=DetonateEffectObject||player->immortalFlag)){
					if(!dangerMap[index.y][index.x]){
						while(q.size!=0)
							dequeue(&q);
						return getIndexAtDirection(startIndex,val->startDirection);
					}
					else{
						havePasts[index.y][index.x]=true;
						enqueue(&q,BFSValue_init(index,val->weight+1,val->startDirection));
					}
				}
			}
			dequeue(&q);
		}
		return startIndex;
	}
	else{
		int scoreMap[MAP_HEIGHT][MAP_WIDTH];
		for(int i=0;i<MAP_HEIGHT;i++){
			for(int j=0;j<MAP_WIDTH;j++){
				scoreMap[i][j]=0;
				for(int k=0;k<4;k++){
					if(bombRangeMap[i][j][k]&&player->bombRange>=bombRangeMap[i][j][k]){
						scoreMap[i][j]+=5;
					}
				}
				if(map[i][j].objType==SpeedItem||
					map[i][j].objType==BombAmountItem||
					map[i][j].objType==BombRangeItem||
					map[i][j].objType==HealthItem){
					scoreMap[i][j]+=15;
				}

			}
		}
		for(int i=0;i<playerCount;i++){
			if((&players[i])!=player){
				for(int j=0;j<4;j++){
					Vector2 index = getIndexAtDirection(getPlayerIndex(&players[i]),(Direction)j);
					for(int k=0;k<player->bombRange&&inBetween(0,index.y,MAP_HEIGHT-1)&&inBetween(0,index.x,MAP_WIDTH-1)&&(!walkMap[index.y][index.x]||Equal(index,startIndex) );k++,index=getIndexAtDirection(index,(Direction)j)){
						scoreMap[index.y][index.x]+=10-2*k;
					}
				}
			}
		}
		Vector2 resultIndex=startIndex;
		Direction resultDirection;
		int mxResult=scoreMap[startIndex.y][startIndex.x];
		Queue q = {NULL,NULL,0};
		Vector2 index;
		for(int i=0;i<4;i++){
			index = getIndexAtDirection(startIndex,(Direction)i);
			if(inBetween(0,index.y,MAP_HEIGHT-1)&&inBetween(0,index.x,MAP_WIDTH-1)&&
					!walkMap[index.y][index.x]&&
					!havePasts[index.y][index.x]&&
					!dangerMap[index.y][index.x]){
				int newResult =scoreMap[index.y][index.x]-2;
				if(mxResult<newResult){
					mxResult = newResult;
					resultIndex = index;
					resultDirection = i;
				}
				havePasts[index.y][index.x]=true;
				enqueue(&q,BFSValue_init(index,2,(Direction)i));
			}
		}
		while(q.size!=0){
			BFSValue* val = (BFSValue*)q.front->value;
			for(int i=0;i<4;i++){
				index = getIndexAtDirection(val->index,(Direction)i);
				if(inBetween(0,index.y,MAP_HEIGHT-1)&&inBetween(0,index.x,MAP_WIDTH-1)&&
						!walkMap[index.y][index.x]&&
						!havePasts[index.y][index.x]&&
						!dangerMap[index.y][index.x]){
					int newResult =scoreMap[index.y][index.x]-(val->weight*2);
					if(scoreMap[index.y][index.x]>0 && newResult<=0){
						newResult =1;
					}
					if(mxResult<newResult){
						mxResult = newResult;
						resultIndex = index;
						resultDirection = val->startDirection;
					}
					havePasts[index.y][index.x]=true;
					enqueue(&q,BFSValue_init(index,val->weight+1,val->startDirection));
				}
			}
			dequeue(&q);
		}
		if(Equal(resultIndex , startIndex) &&players->currentUseBomb<players->maxBombCount&&mxResult!=0){
			placeBomb(player);
		}

		if(Equal(resultIndex , startIndex))
			return startIndex;
		else{
			return getIndexAtDirection(startIndex,resultDirection) ;
		}
	}
}
PlayerUI PlayerUI_init(Player* player,Vector2 offset){
	PlayerUI playerUI;
	int x = 16 + UI_MARGIN;
	int y = 16 + UI_MARGIN;

	playerUI.player = player;
	playerUI.charactorTrans = Transform_init(Add(offset,Vector2_init(0,y)),Vector2_init(16,16),TopLeft);

	playerUI.speedImgTrans = Transform_init(Add(offset,Vector2_init(x,y)) ,Vector2_init(16,16),TopLeft);
	playerUI.bombAmountImgTrans = Transform_init(Add(offset,Vector2_init(x*2,y)) ,Vector2_init(16,16),TopLeft);
	playerUI.bombRangeImgTrans = Transform_init(Add(offset,Vector2_init(x*3,y)) ,Vector2_init(16,16),TopLeft);
	playerUI.healthImgTrans = Transform_init(Add(offset,Vector2_init(x*4,y)) ,Vector2_init(16,16),TopLeft);

	playerUI.startTextPos = Add(offset,Vector2_init(x,0));
	return playerUI;
}
void DrawPlayerUI(PlayerUI* playerUI){
	char str[10];

	DrawImage(&playerUI->charactorTrans,playerUI->player->imgPtr1);

	Vector2 pos =playerUI->startTextPos;
	DrawImage(&playerUI->speedImgTrans,shoeImage);
	sprintf(str,"%d",playerUI->player->speed);
	ILI9341_Draw_Text(str,pos.x,pos.y, WHITE, 2, BLACK);

	pos.x+= 16+UI_MARGIN;
	DrawImage(&playerUI->bombAmountImgTrans,addBombImage);
	sprintf(str,"%d",playerUI->player->maxBombCount);
	ILI9341_Draw_Text(str,pos.x,pos.y, GREEN, 2, BLACK);

	pos.x+= 16+UI_MARGIN;
	DrawImage(&playerUI->bombRangeImgTrans,potionImage);
	sprintf(str,"%d",playerUI->player->bombRange);
	ILI9341_Draw_Text(str,pos.x,pos.y, BLUE, 2, BLACK);

	pos.x+= 16+UI_MARGIN;
	DrawImage(&playerUI->healthImgTrans,heartImage);
	sprintf(str,"%d",playerUI->player->health);
	ILI9341_Draw_Text(str,pos.x,pos.y, RED, 2, BLACK);
}
