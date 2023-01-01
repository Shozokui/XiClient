#pragma once
#define WIN32_LEAN_AND_MEAN
#include "d3d8to9/d3d8types.hpp"

namespace FFXI {
	namespace CTk { class TKRECT; class _49SubList; }
	namespace Text {
		class MojiDraw;
		class TextRenderer {
		public:
			static MojiDraw* g_MojiDraw;
			static const unsigned char mojiwidth_ALL[];
			static void PrepareDeviceForText(int, int, int, int);
			static void PrepareDeviceForText2();
			static void RenderInButtonMenu(const char*, CTk::TKRECT*);
			static int GetSomeValue(const char*);
			static bool IsOneByteCode(unsigned char*);
			static bool IsTwoByteCode(unsigned int);
			static int GetCodeLen(unsigned char*);
			static short GetSqCode(unsigned char**);
			static int SJisGetLineWidth(const char*);
			static unsigned short SJis2Sq(unsigned char*);
			static int SJisFontWidthGet(unsigned short);
			static void SjisFontTextGetSizeHSpace(short*, short*, const char*, CTk::TKRECT*, int);
			//Calculates the widest text line in a multi-line string
			static void SjisFontTextSizeGetStrHSpace(const char*, short*, int);
			static int SjisFontWidthGetStr(const char*);
			static int SjisFontWidthGetStrMax0(const char*);
			static int SjisFontWidthGetStrMax(const char*, unsigned int);
			static int SjisFontTextHeightGetStrWidthHSpace(const char*, short, int);
			static void SjisFontDrawInRectHSpace(const char*, CTk::TKRECT*, int);
			static unsigned int SjisFontCharNumGetStrWidth(const char*, int, int);
			static unsigned int SjisFontCharNumGetStrWidth(const char*, int);
			static void SjisFontDrawWidthHSpace(short, short, const char*, int, int);
			static void mojishadow(short, short, const char*, int, D3DCOLOR);

			static void YkDrawString(short, short, short, const char*, D3DCOLOR);
			static void YkDrawStringScale(short, short, const char*, D3DCOLOR, float, float, bool);
			static void DrawScriptString(CTk::_49SubList***, const char*, int, int);
		};
	}
}