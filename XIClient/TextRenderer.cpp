#include "TextRenderer.h"
#include "TkRect.h"
#include "XIString.h"
#include "Enums.h"
#include "Values.h"
#include "Strings.h"
#include "Globals.h"
#include "MojiDraw.h"
#include "CDx.h"
#include <string.h>
#include "TkManager.h"
#include "YkManager.h"
#include "_49SubList.h"
#include "CTkMsbDataList.h"

using namespace FFXI::Text;
using namespace FFXI::CTk;
void FFXI::Text::TextRenderer::PrepareDeviceForText(int a1, int a2, int a3, int a4)
{
    CYy::CDx* CDx = FFXI::CYy::CDx::instance;
    if (a1 == 0) {
        CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    }
    else {
        CDx->DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE2X);
        CDx->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
        CDx->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

        if (!a2) {
            if (!a3) {
                if (!a4 || a4 == 1 || a4 == 2)
                    ;//print debug message
            }
            else if (a3 == 1) {
                if (a4 == 0) {
                    CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
                    CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
                    CDx->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);
                }
                else if (a4 == 1) {
                    CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                    CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
                }
                else if (a4 == 2) {
                    ;//print debug msg
                }
            }
            else if (a3 == 2) {
                if (a4 == 0) {
                    ;//print debug message
                }
                else if (a4 == 1) {
                    CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                    CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
                }
                else if (a4 == 2) {
                    CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                    CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
                }
            }
        }
        else if (a2 == 1) {
            if (a3 == 0) {
                if (a4 == 0) {
                    CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
                    CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
                }
                else if (a4 == 1) {
                    CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                    CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
                    CDx->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
                }
                else if (a4 == 2) {
                    ;//print debug message
                }
            }
            else if (a3 == 1) {
                if (!a4 || a4 == 1 || a4 == 2)
                    ;//print debug message
            }
            else if (a3 == 2) {
                if (a4 == 0) {
                    CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
                    CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
                }
                else if (a4 == 1) {
                    CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
                    CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
                }
                else if (a4 == 2) {
                    CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
                    CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
                }
            }
        }
        else if (a2 == 2) {
            if (a3 == 0) {
                if (a4 == 0) {
                    ; //print debug message
                }
                else if (a4 == 1) {
                    CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                    CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
                    CDx->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
                }
                else if (a4 == 2) {
                    ;//print debug message
                }
            }
            else if (a3 == 1) {
                if (a4 == 0) {
                    CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
                    CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
                    CDx->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);
                }
                else if (a4 == 1) {
                    CDx->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
                    CDx->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
                }
                else if (a4 == 2) {
                    ;//print debug message
                }
            }
            else if (a3 == 2) {
                if (!a4 || a4 == 1 || a4 == 2)
                    ;//print debug msg
            }
        }
    }

    CDx->DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
}
void FFXI::Text::TextRenderer::PrepareDeviceForText2()
{
    FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    FFXI::CYy::CDx::instance->DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
}
void FFXI::Text::TextRenderer::RenderInButtonMenu(const char* a1, TKRECT* a2)
{
    if (!a1) return;

    //Reduce the height of the box so the text doesn't 
    //render over the buttons
	TKRECT adjusted = *a2;
	adjusted.Bottom -= 28;

    SjisFontDrawInRectHSpace(a1, &adjusted, 0);
}

int FFXI::Text::TextRenderer::GetSomeValue(const char* a2)
{
    if (a2 == nullptr)
        return 0;

    _49SubList*** list = FFXI::Yk::YkManager::GetMenuRes(0);
    if (list == nullptr)
        return 0;

    int pos = 0;
    unsigned char cur{}, prev{};
    int sum{ 0 };
    while (a2[pos] != 0) {
        cur = a2[pos];
        if (pos != 0) {
            prev = a2[pos - 1];
        }
        else {
            prev = 0;
        }
        
        if (cur != 0x85) {
            _49SubList* sl{ nullptr };
            if (prev == 0x85) {
                if (cur == 0xDA) {
                    sl = (*list)[95];
                }
                else if (cur == 0xC8) {
                    sl = (*list)[96];
                }
                else {
                    int v7 = cur - ' ';
                    if (v7 >= 0 && v7 <= 0x8B) {
                        sl = (*list)[v7];
                    }
                }
            }
            else {
                int v7 = cur - ' ';
                if (v7 >= 0 && v7 <= 0x8B) {
                    sl = (*list)[v7];
                }
            }
            
            if (sl != nullptr) {
                sum += sl->field_22 - sl->field_20 - 1;
            }
        }
        pos += 1;
    }

    return sum;
}

bool FFXI::Text::TextRenderer::IsOneByteCode(unsigned char* a1)
{
    return !IsTwoByteCode(*a1);
}

bool FFXI::Text::TextRenderer::IsTwoByteCode(unsigned int a1)
{
    if (a1 >= 0x81 && a1 <= 0x9F)
        return 1;
    if (a1 >= 0xE0 && a1 <= 0xEF)
        return 1;
    return a1 >= 0xFA && a1 <= 0xFC;
}

int FFXI::Text::TextRenderer::GetCodeLen(unsigned char* a1)
{
    return IsTwoByteCode(*a1) + 1;
}

short FFXI::Text::TextRenderer::GetSqCode(unsigned char** a1)
{
    unsigned char* v1 = *a1;
    if (IsOneByteCode(v1)) {
        (*a1) += 1;
        return (*v1) - ' ';
    }
    else {
        (*a1) += 2;
        return SJis2Sq(v1);
    }
}

int FFXI::Text::TextRenderer::SJisGetLineWidth(const char* a1)
{
    const char* v1 = strchr(a1, Constants::Values::SJIS_NEWLINE_CHAR);

    if (v1)
        return v1 - a1 + 1;
    
    return strlen(a1);
}

unsigned short FFXI::Text::TextRenderer::SJis2Sq(unsigned char* a1)
{
    unsigned int v1 = a1[0];
    unsigned int v2 = a1[1];

    unsigned int v3{}, v5{}, v6{};
    int v4{};
    if (v1 > 0x9F)
        v3 = v1 - 0xB1;
    else
        v3 = v1 - 0x71;

    v4 = v3 * 2;
    v4 |= 1;

    if (v2 > 0x7F)
        --v2;

    if (v2 < 0x9E)
        v5 = v2 - 0x1F;
    else {
        ++v4;
        v5 = v2 - 0x7D;
    }

    v6 = v5 | (v4 << 8);

    if (v6 >= 0x9320)
        return 203;
    if (v6 >= 0x7921)
        return v5 + 94 * v4 - 3281;
    if (v6 >= 0x5021)
        return v5 + 94 * v4 - 3564;
    if (v6 >= 0x3021)
        return v5 + 94 * v4 - 3521;
    if (v6 >= 0x2F21)
        return v5 + v4 + 542;
    if (v6 >= 0x2E21)
        return v5 + v4 + 625;
    if (v6 >= 0x2D5F)
        return v5 + v4 + 849;
    if (v6 >= 0x2D40)
        return v5 + v4 + 857;
    if (v6 >= 0x2D21)
        return v5 + v4 + 858;
    if (v6 >= 0x2A21)
        return v5 + v4 + 547;
    if (v6 >= 0x2921)
        return v5 + v4 + 630;
    if (v6 >= 0x2821)
        return v5 + v4 + 515;
    if (v6 >= 0x2751)
        return v5 + v4 + 435;
    if (v6 >= 0x2721)
        return v5 + v4 + 450;
    if (v6 >= 0x2641)
        return v5 + v4 + 395;
    if (v6 >= 0x2621)
        return v5 + v4 + 403;
    if (v6 >= 0x2521)
        return v5 + v4 + 318;
    if (v6 >= 0x2421)
        return v5 + v4 + 236;
    if (v6 >= 0x2361)
        return v5 + v4 + 147;
    if (v6 >= 0x2341)
        return v5 + v4 + 153;
    if (v6 >= 0x2330)
        return v5 + v4 + 160;
    if (v6 == 8831)
        return 242;
    if (v6 >= 0x2272)
        return v5 + v4 + 86;
    if (v6 >= 0x225C)
        return v5 + v4 + 93;
    if (v6 >= 0x224A)
        return v5 + v4 + 104;
    if (v6 < 0x223A)
        return v5 + 94 * v4 - 3039;
    return v5 + v4 + 112;
}

int FFXI::Text::TextRenderer::SJisFontWidthGet(unsigned short a1)
{
    if (a1 < ' ')
        return 0;

    if (a1 < 0x7Fu)
        return mojiwidth_ALL[a1 - ' '];

    unsigned char flipByte[2];
    flipByte[0] = (a1 >> 8) & 0xFF;
    flipByte[1] = a1 & 0xFF;

    unsigned short v2 = SJis2Sq(flipByte);
    if (v2 < 0x2115 || v2 > 0x2125) {
        if (v2 >= 0x440)
            return 14;
        else
            return mojiwidth_ALL[v2];
    }
    else {
        throw "What is this";
        if (!Text::MojiDraw::g_pFontUsGaiji) {
            Text::MojiDraw::g_pFontUsGaiji = CTk::TkManager::g_CTkMenuDataList.GetSubLists(Constants::Strings::FontUsGaiji);
            if (!Text::MojiDraw::g_pFontUsGaiji)
                return 0;
        }

        _49SubList* v5 = (*Text::MojiDraw::g_pFontUsGaiji)[v2 - 0x2115];
        return v5->field_22 - v5->field_20;
    }
}

void FFXI::Text::TextRenderer::SjisFontTextGetSizeHSpace(short* a1, short* a2, const char* a3, TKRECT* a4, int a5)
{
    short v7 = a4->Right - a4->Left;
    short v8{};

    short getSize[2] = { 0, 0 };
    SjisFontTextSizeGetStrHSpace(a3, getSize, a5);
    if (getSize[0] < v7) {
        *a1 = a4->Left + (v7 - getSize[0]) / 2;
        v8 = getSize[1];
    }
    else {
        v8 = SjisFontTextHeightGetStrWidthHSpace(a3, v7, a5);
        *a1 = a4->Left;
    }
    if (v8 <= (a4->Bottom - a4->Top))
        *a2 = a4->Top + (a4->Bottom - a4->Top - v8) / 2;
    else
        *a2 = a4->Top;
}

void FFXI::Text::TextRenderer::SjisFontTextSizeGetStrHSpace(const char* a1, short* a2, int a3)
{
    a2[0] = 0;
    a2[1] = 0;

    if (!a1) return;

    const char* pos = a1;
    int v8 = 0;
    int LineCount = 0;
    while (*pos) {
        const char* v6 = strchr(pos, Constants::Values::SJIS_NEWLINE_CHAR);
        if (v6)
            v8 = v6 - pos;
        else
            v8 = strlen(pos);

        FFXI::Text::XIString v11(pos, v8);
        const char* v9 = v11.c_str();
        int v10 = SjisFontWidthGetStr(v9);
        if (v10 > a2[0])
            a2[0] = v10;
        if (v6)
            pos = v6 + 1;
        else
            pos += v8;
        ++LineCount;
    }

    a2[1] = 16 * LineCount;
    if (LineCount > 0)
        a2[1] = 16 * LineCount + a3 * (LineCount - 1);
}

int FFXI::Text::TextRenderer::SjisFontWidthGetStr(const char* a1)
{
    return SjisFontWidthGetStrMax0(a1);
}

int FFXI::Text::TextRenderer::SjisFontWidthGetStrMax0(const char* a1)
{
    return SjisFontWidthGetStrMax(a1, 0);
}

int FFXI::Text::TextRenderer::SjisFontWidthGetStrMax(const char* a1, unsigned int a2)
{
    if (!a1) return -1;
    unsigned int pos = 0;
    int v9 = 0;
    while (a1[pos]) {
        if (a2 && pos >= pos)
            break;
        if (a1[pos] == 4)
            ++pos;
        else {
            int v7 = 0;
            if (IsOneByteCode((unsigned char*)a1 + pos)) {
                v7 = SJisFontWidthGet(a1[pos]) + v9;
                ++pos;
            }
            else {
                unsigned short byteSwap;
                byteSwap = a1[pos];
                byteSwap <<= 8;
                byteSwap |= a1[pos + 1];
                v7 = SJisFontWidthGet(byteSwap) + v9;
                pos += 2;
            }
            v9 = v7;
        }
    }

    return v9;
}

int FFXI::Text::TextRenderer::SjisFontTextHeightGetStrWidthHSpace(const char* a1, short a2, int a3)
{
    short getValues[2] = { 0, 0 };
    SjisFontTextSizeGetStrHSpace(a1, getValues, a3);

    if (a2 >= getValues[0])
        return getValues[1];

    int v12 = 0;

    int lineLen{};

    const char* pos = a1;
    const char* posBackup = pos;

    int RegionCode = FFXI::GlobalStruct::g_GlobalStruct.GetRegionCode();
    while ((lineLen = SJisGetLineWidth(pos)) != 0) {
        unsigned int StrWidth = SjisFontCharNumGetStrWidth(pos, lineLen, a2);
        int v7 = 0;
        while (StrWidth < lineLen - v7) {
            if (RegionCode != Constants::Enums::LanguageCode::Japanese) {
                int v10 = StrWidth - 1;
                if (v10 >= 0) {
                    while (true) {
                        char v11 = pos[v10];
                        if (v11 == ' ' || v11 == '-') {
                            StrWidth = v10 + 1;
                            break;
                        }
                        if ((--v10 & 0x80000000) != 0) {
                            break;
                        }
                    }
                }
            }

            v7 += StrWidth;
            pos = posBackup + v7;
            v12 += 16;
            StrWidth = SjisFontCharNumGetStrWidth(pos, lineLen - v7, a2);
        }

        pos = posBackup;
        pos += lineLen;
        v12 += a3 + 8;
        posBackup = pos;
    }
    return v12;
}

void FFXI::Text::TextRenderer::SjisFontDrawInRectHSpace(const char* a1, CTk::TKRECT* a2, int a3)
{
    short v1{}, v2{};
    SjisFontTextGetSizeHSpace(&v1, &v2, a1, a2, a3);
    SjisFontDrawWidthHSpace(v1, v2, a1, a2->Right - a2->Left, a3);
}

unsigned int FFXI::Text::TextRenderer::SjisFontCharNumGetStrWidth(const char* a1, int a2, int a3)
{
    if (!a1) return 0xFFFFFFFF;

    int pos = 0;
    int v11 = 0;
    int v10 = 0;

    if (*a1) {
        while (!a2 || pos < a2) {
            const char* v7 = a1 + pos;
            if (a1[pos] == 4) {
                ++pos;
            }
            else {
                int v8{};
                if (IsOneByteCode((unsigned char*)a1 + pos)) {
                    v8 = SJisFontWidthGet(*v7) + v11;
                    ++pos;
                }
                else {
                    unsigned short v9{};
                    v9 = a1[pos];
                    v9 <<= 8;
                    v9 |= a1[pos + 1];
                    v8 = SJisFontWidthGet(v9) + v11;
                    pos += 2;
                }
                v11 = v8;
            }
            if (v11 > a3) {
                a2 = v10;
                break;
            }
            v10 = pos;
            if (!a1[pos])
                break;
        }
    }

    if (v11 > a3)
        return a2;
    return pos;
}

unsigned int FFXI::Text::TextRenderer::SjisFontCharNumGetStrWidth(const char* a1, int a2)
{
    return SjisFontCharNumGetStrWidth(a1, 0, a2);
}

void FFXI::Text::TextRenderer::SjisFontDrawWidthHSpace(short a1, short a2, const char* a3, int a4, int a5)
{
    int LineWidth = 0;
    const char* pos = a3;
    int RegionCode = FFXI::GlobalStruct::g_GlobalStruct.GetRegionCode();
    char v15 = pos[LineWidth];
    while (v15) {
        LineWidth = SJisGetLineWidth(pos);
        int StrWidth = SjisFontCharNumGetStrWidth(pos, a4);
        if (StrWidth < LineWidth) {
            if (RegionCode != Constants::Enums::LanguageCode::Japanese) {
                int v10 = StrWidth - 1;
                if (v10) {
                    while (true) {
                        char v11 = pos[v10];
                        if (v11 == ' ' || v11 == '-') {
                            LineWidth = v10 + 1;
                            if (v10 < 0)
                                LineWidth = StrWidth;
                            break;
                        }
                        if (--v10 < 0) {
                            LineWidth = StrWidth;
                            break;
                        }
                    }

                }
            }
        }

        int len = LineWidth;
        if (pos[LineWidth - 1] == Constants::Values::SJIS_NEWLINE_CHAR)
            --len;

        XIString v16(pos, len);
        const char* v12 = v16.c_str();
        mojishadow(a1, a2, v12, 0, 0x80808080);

        v15 = pos[LineWidth];
        pos += LineWidth;
        a2 += a5 + 16;
    }
}

void FFXI::Text::TextRenderer::mojishadow(short a1, short a2, const char* a3, int a4, D3DCOLOR a5)
{
    TextRenderer::g_MojiDraw->YmDrawString(a1, a2, a3, a4, a5, 68, 1.0, 1.0);
}

void FFXI::Text::TextRenderer::YkDrawString(short availableWidth, short x, short y, const char* a4, D3DCOLOR a5)
{
    if (a4 == nullptr) return;

    int strLen = strlen(a4);
    if (strLen <= 0) return;

    _49SubList*** pppsub = Yk::YkManager::GetMenuRes(0);

    //Ellipses (...)
    _49SubList* ellipses = (*pppsub)[108];
    _49SubList* character{ nullptr };

    int textXPos = x;
    int textWidth = 0;
    unsigned char thischar{ 0 }, prevchar{ 0 };

    if (a5 == 0x80808080) {
        for (int strPos = 0; strPos < strLen; ++strPos) {
          
            if (strPos > 0)
                prevchar = a4[strPos - 1];
            else
                prevchar = 0;
            
            thischar = a4[strPos];
            if (thischar == 0x85)
                continue;

            if (prevchar == 0x85 && (thischar == 0xDA || thischar == 0xC8)) {
                if (thischar == 0xDA) 
                    character = (*pppsub)[95];
                else
                    character = (*pppsub)[96];
            }
            else {
                int v13 = thischar - 32;
                if (v13 < 0 || v13 > 0x8B)
                    continue;
                character = (*pppsub)[v13];
            }

            if (character) {
                int charWidth = character->field_22 - character->field_20 - 1;
                textWidth += charWidth;
                if ((textWidth + 8) >= availableWidth && (a4[strPos + 1] || charWidth > 8)) {
                    ellipses->Draw(textXPos, y, 0x80808080, 0, 0);
                    return;
                }

                character->Draw(textXPos, y, 0x80808080, 0, 0);
                textXPos += charWidth;
            }
        }
    }
    else {
        for (int pos = 0; pos < strLen; ++pos) {
            if (pos > 0)
                prevchar = a4[pos - 1];
            else
                prevchar = 0;

            thischar = a4[pos];
            if (thischar == 0x85)
                continue;

            if (prevchar == 0x85 && (thischar == 0xDA || thischar == 0xC8)) {
                if (thischar == 0xDA)
                    character = (*pppsub)[95];
                else
                    character = (*pppsub)[96];
            }
            else {
                int v13 = thischar - 32;
                if (v13 < 0 || v13 > 0x8B)
                    continue;
                character = (*pppsub)[v13];
            }

            if (character) {
                int charWidth = character->field_22 - character->field_20 - 1;
                textWidth += charWidth;
                if ((textWidth + 8) >= availableWidth && (a4[pos + 1] || charWidth > 8)) {
                    ellipses->ExtraDraw(textXPos, y, 1.0, 1.0, a5, 0, 0, 0);
                    return;
                }

                character->ExtraDraw(textXPos, y, 1.0, 1.0, a5, 0, 0, 0);
                textXPos += charWidth;
            }
        }
    }
}

void FFXI::Text::TextRenderer::YkDrawStringScale(short a1, short a2, const char* a3, D3DCOLOR a4, float a5, float a6, bool a7)
{
    if (a3 == nullptr) return;
    
    int strLen = strlen(a3);
    if (strLen <= 0)
        return;

    short xpos = a1;
    short ypos = a2;

    if (a7 == true)
        xpos -= (double)TextRenderer::GetSomeValue(a3) * a5 * 0.5;

    _49SubList* character{ nullptr };
    _49SubList*** pppsub = Yk::YkManager::GetMenuRes(0);
    if (pppsub == nullptr)
        return;

    unsigned char thischar{ 0 }, prevchar{ 0 };
    if (a4 == 0x80808080 && a5 == 1.0 && a6 == 1.0) {
        for (int strPos = 0; strPos < strLen; ++strPos) {
            if (strPos > 0)
                prevchar = a3[strPos - 1];
            else
                prevchar = 0;

            thischar = a3[strPos];

            if (thischar == 0x85)
                continue;

            if (prevchar == 0x85 && (thischar == 0xDA || thischar == 0xC8)) {
                if (thischar == 0xDA)
                    character = (*pppsub)[95];
                else
                    character = (*pppsub)[96];
            }
            else {
                int v14 = thischar - 32;
                if (v14 < 0 || v14 > 0x8B)
                    continue;
                character = (*pppsub)[v14];
            }

            if (character) {
                character->Draw(xpos, ypos, 0x80808080, 0, 0);
                xpos += character->field_22 - character->field_20 - 1;
            }
        }
    }
    else {
        for (int strPos = 0; strPos < strLen; ++strPos) {
            if (strPos > 0)
                prevchar = a3[strPos - 1];
            else
                prevchar = 0;

            thischar = a3[strPos];

            if (thischar == 0x85)
                continue;

            if (prevchar == 0x85 && (thischar == 0xDA || thischar == 0xC8)) {
                if (thischar == 0xDA)
                    character = (*pppsub)[95];
                else
                    character = (*pppsub)[96];
            }
            else {
                int v14 = thischar - 32;
                if (v14 < 0 || v14 > 0x8B)
                    continue;
                character = (*pppsub)[v14];
            }

            if (character) {
                character->ExtraDraw(xpos, ypos, a5, a6, a4, a7, 0, 0);
                xpos += (character->field_22 - character->field_20 - 1)* a5;
            }
        }
    }
}

void FFXI::Text::TextRenderer::DrawScriptString(CTk::_49SubList*** a1, const char* a2, int a3, int a4)
{
    if (a2 == nullptr)
        return;

    int strLen = strlen(a2);
    if (strLen <= 0)
        return;

    int index{ 0 };
    for (int strPos = 0; strPos < strLen; ++strPos) {
        unsigned char thischar = a2[strPos];
        if (thischar >= '0' && thischar <= '9')
            index = 12 + thischar - '0';
        else if (thischar >= 'A' && thischar <= 'Z')
            index = 22 + thischar - 'A';
        else if (thischar >= 'a' && thischar <= 'z')
            index = 48 + thischar - 'a';
        else {
            unsigned char nextchar = a2[strPos + 1];
            if (thischar == 0x85 && nextchar >= 0x9F && nextchar <= 0xDF) {

                switch (nextchar) {
                case 0xBE:
                    index = 160;
                    break;
                case 0xC3:
                    index = 161;
                    break;
                case 0xC7:
                    index = 162;
                    break;
                case 0xCE:
                    index = 163;
                    break;
                case 0xD5:
                    index = 164;
                    break;
                case 0xC8:
                    index = 167;
                    break;
                default:
                    continue;
                }
            }
            else {
                a3 += 6;
                continue;
            }
        }

        CTk::_49SubList* list = (*a1)[index];
        list->Draw(a3, a4, 0x80808080, 0, 0);
        a3 += list->field_22 - list->field_20;
    }
}

MojiDraw* TextRenderer::g_MojiDraw{ nullptr };
const unsigned char TextRenderer::mojiwidth_ALL[] =
{
    7,   7,   7,  10,   8,  13,  11,   5,   8,   8,
    9,   9,   5,   8,   5,   7,   9,   9,   9,   9,
    9,   9,   9,   9,   9,   9,   5,   5,   9,   9,
    9,   9,  11,  11,  11,  10,  10,  10,   9,  11,
   10,   6,   9,  11,  10,  12,  10,  11,  10,  12,
   11,  10,  10,  10,  11,  14,  11,  11,  11,   8,
    8,   8,   9,   9,   7,   9,   9,   9,   9,   9,
    7,   9,   9,   5,   6,   9,   5,  12,   9,  10,
    9,   9,   6,   8,   7,   9,   9,  11,   9,   9,
    8,   7,   6,   7,  12,   7,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,   8,   7,   7,   7,
    9,   9,  16,  16,  14,  14,  14,  14,  14,  14,
   14,  14,  15,  14,  14,  14,  16,  16,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  16,  16,  16,  16,  14,  14,  14,  14,
    8,   8,  10,  15,  16,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  15,  15,  16,  16,  16,  16,
   16,  15,  15,  15,  15,  15,  15,  15,  14,  14,
   14,  14,  14,  15,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  15,  14,  14,
   14,  15,  14,  14,  14,  14,  16,  16,  16,  15,
   15,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  16,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
    8,   8,  11,  11,  11,  11,  11,  11,  10,   9,
    9,   9,   9,   9,   9,   9,   9,   9,  11,   9,
   10,  11,  11,  10,  10,  11,  11,   9,  10,   9,
    9,  11,   9,  10,   9,   9,   9,   9,   9,   9,
   12,   9,   9,   9,   9,   9,   7,   7,   7,   7,
    9,   9,  10,  10,  10,  10,  10,  10,  10,   9,
    9,   9,   9,  11,  10,  11,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  10,   8,   4,  10,   8,  10,
    8,   9,   8,  11,   9,   6,  11,   8,  10,   8,
    8,   5,   5,   8,   8,   7,  10,  11,   8,  10,
    8,   6,  11,   8,   9,   8,   7,   5,   8,   8,
   10,  11,   4,   8,   8,  10,   8,   9,  10,   8,
    8,  11,   5,  10,   8,   8,   5,   9,   8,   7,
    6,   5,   8,   9,  12,  12,  12,   9,   5,   5,
    5,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
    8,   8,   8,   8,   8,   8,   8,   5,   5,   7,
    7,   7,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
   14,  14,  14,  14,  14,  14,  14,  14,   7,   7,
    7,  10,   8,  13,  11,   5,   8,   8,   9,   9,
    5,   8,   5,   7,   9,   9,   9,   9,   9,   9,
    9,   9,   9,   9,   5,   5,   9,   9,   9,   9,
   11,  11,  11,  10,  10,  10,   9,  11,  10,   6,
    9,  11,  10,  12,  10,  11,  10,  12,  11,  10,
   10,  10,  11,  14,  11,  11,  11,   8,   8,   8,
    9,   9,   7,   9,   9,   9,   9,   9,   7,   9,
    9,   5,   6,   9,   5,  12,   9,  10,   9,   9,
    6,   8,   7,   9,   9,  11,   9,   9,   8,   7,
    6,   7,  12,   7,  11,  11,  11,  11,  11,  11,
   10,   9,   9,   9,   9,   8,   8,   8,   8,   8,
   11,  11,  11,  11,  11,  11,  11,  11,  11,  11,
   10,  10,  10,  10,  11,  11,  10,  10,  10,  10,
   10,  10,  10,   9,   9,   9,   9,   9,   6,   6,
    6,   6,  10,   9,  10,  10,  10,  10,  10,  10,
   10,   9,   9,   9,   9,  10,  10,  10,  11,  11,
   11,  11,  11,  11,  11,  11,  11,  11,  11,  11,
   11,  11,  11,  11,  11,  11,  11,  11,  11,  11,
   11,  11,  11,  11,  11,  11,  11,  11,  11,  11,
   10,   9,   6,  11,  10,  10,  10,   9,  12,  10,
    6,  12,   9,  10,   8,   8,   7,   7,   8,  10,
   10,   9,  11,   8,  10,   9,  10,  12,   9,   9,
   11,   8,   5,   9,   9,  10,   9,  11,   9,   9,
   11,   9,   9,  10,   9,   9,  12,   6,  11,   7,
    7,   5,   9,  10,  13,   7,   6,   9,  10,  12,
   12,  12,   9,  11,  11,  11,  11,  11,  11,  11,
   11,  11,  11,  11,  11,  11,  11,  11,  11,  11,
   11,  11,  11,  11,  11,  11,  11,  11,  11,  11,
   11,  11,  11,  11,  11,  11, 128, 128, 128, 128
};