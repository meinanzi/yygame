#ifndef __FontConfig_H__
#define __FontConfig_H__

#include "cocos2d.h"

USING_NS_CC;

extern Color3B colorGold;          //金色
extern Color3B colorYellow;        //黄色
extern Color3B colorGreen;		   //绿色
extern Color3B colorMagenta;	   //紫红色
extern Color3B colorBlue;		   //蓝色
extern Color3B colorOrange;        //橙色
extern Color3B colorWhite;         //白色
extern Color3B colorRed;           //红色
extern Color3B colorBlack;         //黑色
extern Color3B colorGray;          //灰色

Label* createLabel(const std::string& text, float fontSize, Color3B color = colorGold);

#endif	//__FontConfig_H__
                                                  