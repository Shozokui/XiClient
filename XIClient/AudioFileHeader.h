#pragma once

namespace FFXI {
	class AudioFileHeader {
	public:
		int field_0;
		int field_4;
		int field_8;
		int EncodingFormat;
		int FileSize;
		int FileID;
		int SampleCount;
		int field_1C;
		int SampleRateAddend1;
		int SampleRateAddend2;
		int FileHeaderSize;
		char field_2C;
		unsigned char wBitsPerSample;
		unsigned char nChannels;
		unsigned char field_2F;
	};
}