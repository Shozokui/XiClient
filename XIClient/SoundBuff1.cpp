#include "SoundBuff1.h"
#include "Enums.h"
using namespace FFXI;
extern void Type0Decode(unsigned char a1, unsigned char a2, unsigned char* a3, short* a4, int* a5, unsigned int a6);

void FFXI::SoundBuff1::PrepareSoundData(char* a2, char* a3, int a4)
{
	int v9{}, v11{}, v12{}, v13{};
	unsigned int v6{}, v8{};
	unsigned char* byteData = (unsigned char*)this->FileHeader;
	int* intData = (int*)this->FileHeader;

	int EncodingType = intData[3];
	if (EncodingType == Constants::Enums::AudioEncoding::Encoding0) {
		v6 = ((byteData[44] >> 1) + 1) * ((a4 >> 1) / byteData[44]);
	}
	else if (EncodingType == Constants::Enums::AudioEncoding::ATRAC3) {
		v6 = this->field_AC * ((((a4 >> 1) / byteData[42]) >> 10) + 4);
	}
	else {
		v6 = a4;
	}

	memset(a2, 0, v6);
	int v57 = v6;
	if (this->FileData) {
		if (EncodingType == Constants::Enums::AudioEncoding::Encoding0) {
			v8 = (byteData[44] >> 1) + 1;
			v12 = byteData[42] * v8;
			v13 = intData[9];
			v9 = v13 + v12 * intData[5];
			v11 = v13 + v12 * this->field_F0;
		}
		else if (EncodingType == Constants::Enums::AudioEncoding::ATRAC3) {
			throw "NOT IMPLEMENTED";
		}
		else {
			v8 = byteData[43] >> 3;
			v12 = byteData[42] * v8;
			v13 = intData[9];
			v9 = v13 + v12 * intData[5];
			v11 = v13 + v12 * this->field_F0;
		}

		int v15 = intData[6];
		if (v15 >= 0 || v11 < v9) {
			int v58 = v6;
			char* a1a = a2;
			while (true) {
				unsigned int v14 = v58;
				if (v9 < v58 + v11)
					v14 = v9 - v11;
				memcpy(a1a, this->FileData + v11, v14);
				v58 -= v14;
				if (v58 <= 0 || v15 < 0)
					break;

				char* v16 = a1a + v14;
				if (EncodingType == Constants::Enums::AudioEncoding::Encoding0) {
					int v18 = (byteData[44] >> 1) + 1;
					v11 = intData[9] + v15 * byteData[42] * v18;
				}
				else if (EncodingType == Constants::Enums::AudioEncoding::ATRAC3) {
					throw "NOT IMPLEMENTED";
				}
				else {
					int v18 = byteData[43] >> 3;
					v11 = intData[9] + v15 * byteData[42] * v18;
				}
			}
		}

		if (EncodingType == Constants::Enums::AudioEncoding::Encoding0) {
			Type0Decode(byteData[42], byteData[44], (unsigned char*)a2, (short*)a3, this->Type0DecodeContext, a4);
		}
		else if (EncodingType == Constants::Enums::AudioEncoding::ATRAC3) {
			throw "NOT IMPLEMENTED";
		}
		else {
			memcpy(a3, a2, a4);
			if (EncodingType == Constants::Enums::AudioEncoding::Encoding2) {
				if ((byteData[43] & 0xF8) == 16) {
					char* v45 = a3;
					do {
						char v46 = v45[1] ^ *v45;
						*v45 = v46;
						char v48 = v45[1];
						v45 += 2;
						char v49 = v46 ^ v48;
						*(v45 - 1) = v49;
						*(v45 - 2) ^= v49;
					} while (v45 < a3 + (2 * (a4 >> 1)));
				}
			}
		}

		int v51{}, v52{}, v53{}, v54{};
		if (EncodingType == Constants::Enums::AudioEncoding::Encoding0) {
			v51 = (byteData[44] >> 1) + 1;
			v52 = v57;
		}
		else if (EncodingType == Constants::Enums::AudioEncoding::ATRAC3) {
			v52 = a4;
			v51 = byteData[43] >> 3;
		}
		else {
			v51 = byteData[43] >> 3;
			v52 = v57;
		}

		v53 = intData[6];
		v54 = v52 / (byteData[42] * v51) + this->field_F0;
		if (v53 >= 0) {
			int v55 = intData[5];
			if (v54 >= v55) {
				do {
					v54 += v53 - v55;
				} while (v54 >= v55);
			}
		}
		this->field_F0 = v54;
	}
}
