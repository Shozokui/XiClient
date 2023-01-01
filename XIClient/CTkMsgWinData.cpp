#include "CTkMsgWinData.h"
#include "TextRenderer.h"
#include <string>
using namespace FFXI;

unsigned char CTkMsgWinData::SomeValues[8] = {
	0x81, 0x40, 0, 0, 0, 0, 0, 0
};
CTkMsgWinData::CTkMsgWinData() {
	this->field_6403C = 0;
	this->InitAll();
}

int FFXI::CTkMsgWinData::fsGetFontWidth(unsigned char* a1)
{
	if (Text::TextRenderer::IsOneByteCode(a1))
		return Text::TextRenderer::SJisFontWidthGet(*a1);
	unsigned short doublefont = a1[0];
	doublefont <<= 8;
	doublefont |= a1[1];
	return Text::TextRenderer::SJisFontWidthGet(doublefont);
}

int FFXI::CTkMsgWinData::fsGetStrWidth(unsigned char* a1)
{
	unsigned char* v1 = a1;
	int v2 = 0;
	for (unsigned char i = *a1; *v1; ++v2) {
		switch (i)
		{
		case 0x7Fu:
			switch (v1[1])
			{
			case 0xFB:
			case 0xFC:
			case 0x31u:
			case 0x32u:
			case 0x33u:
			case 0x37u:
				v1 += 2;
				break;
			case 0x34u:
			case 0x35u:
			case 0x36u:
				v1 += 3;
				break;
			case 0x38u:
				v1 += 4;
				break;
			default:
				++v1;
				break;
			}
			break;
		case 0x1Eu:
			v1 += 2;
			break;
		case 0x1Fu:
			v1 += 2;
			break;
		default:
			v1 += Text::TextRenderer::GetCodeLen(v1);
			break;
		}
		i = *v1;
	}
	return v2;
}

void FFXI::CTkMsgWinData::InitAll()
{
	this->Init();
	this->field_0 = 0;
	this->field_4 = 0;
	this->field_8 = 0;
	this->field_C = 0;
	this->field_10 = 0;
	this->field_64020 = 0;
	this->field_64024 = 0;
	this->field_64048 = 0;
	this->field_6404A = CTkMsgWinData::fsGetFontWidth(CTkMsgWinData::SomeValues);
	this->field_6404C = CTkMsgWinData::fsGetStrWidth(CTkMsgWinData::SomeValues);
}

void FFXI::CTkMsgWinData::Init()
{
	memset(this->field_20, 0, sizeof(this->field_20));
	this->field_14 = 0;
	this->field_16 = 0;
	this->field_1A = 0;
	this->field_18 = 8;
}
