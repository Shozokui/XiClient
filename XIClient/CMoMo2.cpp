#include "CMoMo2.h"
#include "CMoResourceMng.h"
#include "MotionManStaticStruct.h"
#include "CYySkl.h"
#include "Globals.h"
using namespace FFXI::CYy;

FFXI::CYy::CMoMo2::CMoMo2()
{
	//nullsub
}

void FFXI::CYy::CMoMo2::Open()
{
	if (this->Data[0] != 0) {
		//version error
	}

	if (this->Data[1] != 0) 
		return;

	this->Data[1] = 1;
	if (*(short*)(this->Data + 4) != 0)
		*(short*)(this->Data + 4) -= 1;

	int resfileindex = CMoResourceMng::GetNumFileIndex();
	if (*(short*)(this->Data + 2) != 0) {
		if (resfileindex == 0xC122 || resfileindex == 0xC40D || resfileindex == 0xC129) {
			//throw "not implemented";
			exit(0x10018900);
		}
	}

	if (resfileindex >= CMoResourceMng::SomeFileMappingFunc(0, 8) && resfileindex <= CMoResourceMng::SomeFileMappingFunc(0, 15)
		|| resfileindex >= CMoResourceMng::SomeFileMappingFunc(0, 436) && resfileindex <= CMoResourceMng::SomeFileMappingFunc(0, 443)
		|| resfileindex >= CMoResourceMng::SomeFileMappingFunc(1, 8) && resfileindex <= CMoResourceMng::SomeFileMappingFunc(1, 15)) {
		int fourcc = this->Header.FourCC & 0xFFFF0000;
		if (fourcc == 0x316C0000 || fourcc == 0x316D0000 || fourcc == 0x31700000) {
			exit(0x10018901);
		}
	}
}

void FFXI::CYy::CMoMo2::Close()
{
	//nullsub
}

void FFXI::CYy::CMoMo2::CalcMotionData(int a2, float a3, float a4, MotionManStaticStruct* a5)
{
	if (a4 < 0.000099999997)
		return;

	unsigned short count = *(unsigned short*)(this->Data + 2);
	for (int i = 0; i < count; ++i) {
		unsigned int index = *(unsigned int*)(this->Data + 10 + i * 84);
		unsigned char flags = CYySkl::g_obj[index];
		if ((flags & 0x80u) != 0
			&& (flags & 0x40) != 0x40) {
			if ((flags & 0x3F) == 0) {
				if (this->BlendZero(a2, i, index, a3, a5)) {
					CYySkl::g_obj[index] |= 1u;
				}
			}
			else if ((flags & 0x3F) == 1) {
				this->BlendOne(a2, i, index, a3, a4, a5);
			}
		}
	}
}

bool FFXI::CYy::CMoMo2::BlendZero(int a2, int a3, unsigned int a4, float a5, MotionManStaticStruct* a6)
{
	FFXI::Math::KzFQuat quatlerp{}, quatsetup{};
	int* intchunk = (int*)(this->Data + 10 + 84 * a3);
	float* floatchunk = (float*)intchunk;

	for (int i = 0; i < 4; ++i) {
		if (intchunk[1 + i] == 0) {
			*(&a6[a4].field_0.x + i) = floatchunk[5 + i];
			*(&quatsetup.x + i) = floatchunk[5 + i];
		}
		else if (intchunk[1 + i] < 0 && a2 != 0) {
			if (a2 < 5) {
				FFXI::Math::KzFQuat quat{};
				a6[a4].field_0 = quat;
				a6[a4].field_10 = { 0.0, 0.0, 0.0 };
				a6[a4].field_1C = { 1.0, 1.0, 1.0 };
			}

			return false;
		} 
		else {
			int v11 = intchunk[1 + i] & 0x7FFFFFFF;
			if (v11 != 0) {
				*(&a6[a4].field_0.x + i) = floatchunk[(int)a5 + v11];
				*(&quatsetup.x + i) = floatchunk[(int)a5 + v11 + 1];
			}
			else {
				*(&a6[a4].field_0.x + i) = floatchunk[5 + i];
				*(&quatsetup.x + i) = floatchunk[5 + i];
			}
		}
	}

	float v44 = a5 - (int)a5;
	a6[a4].field_0.lerp(&quatlerp, &quatsetup, v44);
	a6[a4].field_0 = quatlerp;


	bool retval{ false };
	D3DXVECTOR3 vecsetup{};
	if (intchunk[9] < 0 && a2 != 0) {
		for (int i = 0; i < 3; ++i) {
			*(&a6[a4].field_10.x + i) = 0.0;
			int offset = intchunk[15 + i];
			if (offset == 0) {
				*(&a6[a4].field_1C.x + i) = floatchunk[18 + i];
				*(&vecsetup.x + i) = 0.0;
			}
			else {
				*(&a6[a4].field_1C.x + i) = floatchunk[offset + (int)a5];
				*(&vecsetup.x + i) = floatchunk[1 + offset + (int)a5];
			}
		}

		a6[a4].field_1C.x += v44 * vecsetup.x;
		a6[a4].field_1C.y += v44 * vecsetup.y;
		a6[a4].field_1C.z += v44 * vecsetup.z;
	}
	else {
		for (int i = 0; i < 3; ++i) {
			int index = intchunk[9 + i] & 0x7FFFFFFF;
			if (index == 0) {
				*(&quatlerp.x + i) = 0.0;
				*(&a6[a4].field_10.x + i) = floatchunk[12 + i];
			}
			else {
				float* pvalue = floatchunk + index + (int)a5;
				*(&a6[a4].field_10.x + i) = pvalue[0];
				*(&quatlerp.x + i) = pvalue[1] - pvalue[0];
			}

			int offset = intchunk[15 + i];
			if (offset == 0) {
				*(&vecsetup.x + i) = 0.0;
				*(&a6[a4].field_1C.x + i) = floatchunk[18 + i];
			}
			else {
				float* pvalue = floatchunk + offset + (int)a5;
				*(&a6[a4].field_1C.x + i) = pvalue[0];
				*(&vecsetup.x + i) = pvalue[1] - pvalue[0];
			}
		}

		a6[a4].field_10.x += v44 * quatlerp.x;
		a6[a4].field_10.y += v44 * quatlerp.y;
		a6[a4].field_10.z += v44 * quatlerp.z;

		a6[a4].field_1C.x += v44 * vecsetup.x;
		a6[a4].field_1C.y += v44 * vecsetup.y;
		a6[a4].field_1C.z += v44 * vecsetup.z;

		retval = true;
	}

	if (a4 == 0) {
		a6->field_1C.x = 1.0;
		a6->field_1C.y = 1.0;
		a6->field_1C.z = 1.0;
	}
	
	return retval;
}

bool FFXI::CYy::CMoMo2::BlendOne(int a2, int a3, unsigned int a4, float a5, float a6, MotionManStaticStruct* a7)
{
	//sub //TODO
	exit(0x100190B0);
	return false;
}
