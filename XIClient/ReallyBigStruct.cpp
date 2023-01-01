#include "ReallyBigStruct.h"
#include "RidListNode.h"
#include "RidStruct.h"
#include "CTsZoneMapSubStruct1.h"
#include "Globals.h"
#include <string>
using namespace FFXI::CYy;

FFXI::CYy::ReallyBigStruct::ReallyBigStruct()
{
	memset(&this->field_8, 0, 0x6C);
	this->field_198FC = 0;
	this->field_1807C = 0;
	this->field_3413C = 0;
	this->field_34140 = 0;
	this->field_34144 = 0;
	this->field_34148 = 0;
	this->field_3414C = 0;
	this->field_34150 = 0;
	this->field_34158 = 0;
	this->field_34154 = 0;
	this->field_3415C = 0;

	//set some statics
	//sub //TODO

	this->field_34138 = 0;
	this->field_34139 = 0;
	this->field_3413A = 0;
	this->field_3413B = 0;
}

FFXI::CYy::ReallyBigStruct::~ReallyBigStruct()
{
	this->Clean();
}

void FFXI::CYy::ReallyBigStruct::PrepareData(CTsZoneMapSubStruct1* a2, char* a3, int MzbVersion, char a5)
{
	int v43{ 0 }, v6{ 0 };
	this->field_34138 = a5 & 1;
	this->field_34139 = (a5 & 4) != 0;
	this->field_3413A = (a5 & 8) != 0;
	this->field_3413B = (a5 & 16) != 0;

	this->field_1807C = 0;
	this->MzbVersion = MzbVersion;
	if (MzbVersion <= 0x19)
		this->field_70 = 128;
	else
		this->field_70 = 192;

	this->field_30 = a2->field_10;
	this->field_34 = a2->field_14;
	this->field_38 = a2->field_18;
	this->field_3C = a2->field_1C;

	float v9 = (double)this->field_30 * this->field_38;
	this->field_40 = v9 * 0.5;
	this->field_48 = v9 / 4;

	float v10 = (double)this->field_34 * this->field_3C;
	this->field_44 = v10 * 0.5;
	this->field_4C = v10 / 4;

	if (a2->field_C) {
		this->field_50 = a2->field_C[0];
		this->field_54 = (int*)((char*)a2->data + a2->field_C[1]);
		this->field_58 = a2->field_C[2];
		this->field_5C = (int*)((char*)a2->data + a2->field_C[3]);
		this->field_64 = a2->field_C[6];
		this->field_68 = (int*)((char*)a2->data + a2->field_C[5]);
		if (a2->field_0 == 1)
			this->field_60 = (int*)((char*)a2->data + a2->field_C[4]);

		if (this->field_50 > 0) {
			int* v12 = this->field_54;
			for (int i = 0; i < this->field_50; ++i) {
				v12[0] += (int)a2->data;
				v12[1] += (int)a2->data;
				v12[2] += (int)a2->data;

				short* v17 = (short*)(v12[2]);
				short someCount = Globals::Get<short>((char*)v12, 12);
				//it looks like this just sets some flags
				if (someCount > 0) {
					for (int j = 0; j < someCount; ++j) {
						float* v18 = (float*)(v12[1] + 12 * (v17[3] & 0x7FFF));
						int* nanCheck = (int*)v18;
						if (nanCheck[0] == 0xFFC00000) {
							v18[0] = 0.0;
							v43 = 1;
						}
						if (nanCheck[1] == 0xFFC00000) {
							v18[1] = 0.0;
							v43 = 1;
						}
						if (nanCheck[2] == 0xFFC00000) {
							v18[2] = 0.0;
							v43 = 1;
						}
						
						if (v18[0] == 0.0 && v18[1] == 0.0 && v18[2] == 0.0) {
							v18[1] = 0.000001;
							v43 = 1;
						}

						float* v19 = (float*)(v12[0] + 12 * (v17[0] & 0x7FFF));
						float* v20 = (float*)(v12[0] + 12 * (v17[1] & 0x3FFF));
						float* v21 = (float*)(v12[0] + 12 * (v17[2] & 0x3FFF));
					
						float va = v19[0] - v20[0];
						float vb = v19[1] - v20[1];
						float vc = v19[2] - v20[1];

						float v45 = v20[0] - v21[0];
						float v44 = v20[1] - v21[1];
						float v22 = v20[2] - v21[2];

						float v48 = v21[0] - v19[0];
						float v47 = v21[1] - v19[1];
						float v24 = v21[2] - v19[2];
						v17[1] &= 0xBFFF;

						if (va * va + vb * vb + vc * vc == 0.0
							|| v22 * v22 + v44 * v44 + v45 * v45 == 0.0
							|| v24 * v24 + v47 * v47 + v48 * v48 == 0.0) 
							v17[1] |= 0x40;
						
						v17 += 4;
					}
				}

				v12 = (int*)(v12[2] + 8 * someCount);
			}
		}

		int* v28 = this->field_5C;
		if (this->field_58 > 0) {
			for (int i = 0; i < this->field_58; ++i) {
				int* v29 = v28 + 1;
				int v30 = *v28 & 0x7FF;
				if (v30 > 0) {
					for (int j = 0; j < v30; ++j) {
						if (*v29)
							*v29 += (int)a2->data;
						else 
							v6 = 1;
						if (v29[1])
							v29[1] += (int)a2->data;
						else 
							v6 = 1;
						v29 += 2;
					}
				}
			}
		}

		int* v33 = this->field_68;
		unsigned char* v33c = (unsigned char*)v33;
		if (this->field_64 > 0) {
			if (v33[42])
				v33[42] += (int)a2->data;
			if (v33c[172]) {
				if ((v33[41] & 0x10000000) == 0) {
					v33[41] |= 0x10000000;
					v33c[172] = a3[v33c[172] - 1] + 1;
				}
			}
			
			if (v33c[173]) {
				if ((v33[41] & 0x20000000) == 0) {
					v33[41] |= 0x20000000;
					v33c[173] = a3[v33c[173] - 1] + 1;
				}
			}

			if (v33c[174]) {
				if ((v33[41] & 0x40000000) == 0) {
					v33[41] |= 0x40000000;
					v33c[174] = a3[v33c[174] - 1] + 1;
				}
			}

			if (v33c[175]) {
				if ((v33[41] & 0x80000000) == 0) {
					v33[41] |= 0x80000000;
					v33c[175] = a3[v33c[175] - 1] + 1;
				}
			}

			v33 = (int*)(v33c + this->field_70);
		}

		if (a2->field_0 == 1) {
			int* v39 = this->field_60;
			int v40 = this->field_4C * this->field_48;
			for (int j = 0; j < v40; ++j) {
				if (v39[j])
					v39[j] += (int)a2->data;
			}
		}

		this->field_8 = a2->data;
	}
}

void FFXI::CYy::ReallyBigStruct::Clean()
{
	if (this->field_14)
		throw "do this";

	if (this->field_1C)
		throw "do this";

	if (this->field_24)
		throw "do this";

	memset(&this->field_8, 0, 0x6C);
}

void FFXI::CYy::ReallyBigStruct::InitRid(RidListNode* a2)
{
	if (a2->field_10 <= 0) return;

	for (int i = 0; i < a2->field_10; ++i) {
		RidStruct* rs = a2->field_C + i;

		if (!rs->field_D8) continue;
		if (rs->field_D4 <= 0) continue;

		int* v5 = rs->field_D8;
		for (int j = 0; j < rs->field_D4; ++j) {
			int v6 = (__int64)((*((float*)v5 + 4) + this->field_40) * 0.25);
			long long v7 = (__int64)((*((float*)v5 + 5) + this->field_44) * 0.25);
			*v5 = ((int)v7 << 23) ^ *v5 & 0x17FF ^ ((v6 & 0x1FF) << 14) | 0x800;
			int** v8 = (int**)(this->field_60 + 4 * (v6 + v7 * this->field_48));
			for (int* k = *v8; *v8; k = *v8)
				v8 = (int**)&k[2 * (*k & 0x7FF) + 1];
			*v8 = v5;
			v5 += 124;
		}
	}
}
