/*
 * endGamePage.c
 *
 *  Created on: Oct 24, 2023
 *      Author: vorapolpromwan
 */

#include "endGamePage.h"
#include "ILI9341_Touchscreen.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "spi.h"
EndPage EndPage_init(int count,Player* player){
	EndPage endPage;
	endPage.winPlayerCount = count;

	for(int i=0;i<count;i++){
		 endPage.indexs[i]=player[i].imageIndex;
	}
	Vector2 position = Vector2_init(SCREEN_WIDTH/2,SCREEN_HEIGHT/2-50);
	endPage.imageCenterPosition = position;
	position.y+=70;
	endPage.textResultTransform = Transform_init(position,Vector2_init(50,20),TopCenter);
	position.y+=endPage.textResultTransform.size.y + PAGE_SPACE;
	endPage.exitButtonTransform =  Transform_init(position,Vector2_init(50,20),TopCenter);
	return endPage;
}
void drawEndGame(EndPage* endPage){
	Transform transform = Transform_init(Vector2_init(0,0),Vector2_init(320,240),TopLeft);
	Transform trans[4];
	int halfSpace = PAGE_SPACE/2;
	DrawImage(&transform,winPage);
	switch(endPage->winPlayerCount){
	case 1:
		trans[0] = Transform_init(endPage->imageCenterPosition,Vector2_init(112,112),MiddleCenter);
		break;
	case 2:
		trans[0] = Transform_init(Add(endPage->imageCenterPosition,Vector2_init(-halfSpace,0)) ,
				Vector2_init(56,56),MiddleRight);
		trans[1] = Transform_init(Add(endPage->imageCenterPosition,Vector2_init(halfSpace,0)),
				Vector2_init(56,56),MiddleLeft);
		break;
	case 3:
		trans[0] = Transform_init(Add(endPage->imageCenterPosition,Vector2_init(-halfSpace,-halfSpace)) ,
				Vector2_init(56,56),BottomRight);
		trans[1] = Transform_init(Add(endPage->imageCenterPosition,Vector2_init(halfSpace,-halfSpace)),
				Vector2_init(56,56),BottomLeft);
		trans[2] = Transform_init(Add(endPage->imageCenterPosition,Vector2_init(0,halfSpace)),
						Vector2_init(56,56),TopCenter);
		break;
	case 4:
		trans[0] = Transform_init(Add(endPage->imageCenterPosition,Vector2_init(-halfSpace,-halfSpace)) ,
				Vector2_init(56,56),BottomRight);
		trans[1] = Transform_init(Add(endPage->imageCenterPosition,Vector2_init(halfSpace,-halfSpace)),
				Vector2_init(56,56),BottomLeft);
		trans[2] = Transform_init(Add(endPage->imageCenterPosition,Vector2_init(-halfSpace,halfSpace)) ,
				Vector2_init(56,56),TopRight);
		trans[3] = Transform_init(Add(endPage->imageCenterPosition,Vector2_init(halfSpace,halfSpace)),
				Vector2_init(56,56),TopLeft);
		break;
	}
	if(endPage->winPlayerCount==1)
		DrawImage(&trans[0],getImage112(endPage->indexs[0]));
	else{

		for(int i=0;i<endPage->winPlayerCount;i++){
			DrawImage(&trans[i],getImage56(endPage->indexs[i]));
		}
	}
	Draw(&endPage->textResultTransform,BLUE);
	Draw(&endPage->exitButtonTransform,BLACK);
}
