#include "FontConfig.h"                                     

Color3B colorGold(255, 215, 72);                               //金色
Color3B colorYellow(255, 255, 0);                              //黄色
Color3B colorGreen(0, 255, 0);								   //绿色
Color3B colorMagenta(255, 0, 255);							   //紫红色
Color3B colorBlue(0, 0, 255);								   //蓝色
Color3B colorOrange(255, 165, 0);                              //橙色
Color3B colorWhite(255, 255, 255);                             //白色
Color3B colorRed(255, 0, 0);                                   //红色
Color3B colorBlack(0, 0, 0);                                   //黑色
Color3B colorGray(128, 128, 128);                              //灰色

Label* createLabel(const std::string& text, float fontSize, Color3B color)
{	
	Label* label = Label::createWithSystemFont(text, "", fontSize);
	label->setColor(color);
	return label;
}
