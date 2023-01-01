#include <wtypes.h>
#include <string>
int SomeTable[] = {
	0, 0, 0x3C, 0, 0x73, -52, 0x62, -55, 0x7A, -60
};

void DecodeChannelSamples(int SampleCount, unsigned char* In, short* Out, int* Context, int gap) {
	
	char TableIndex = 2 * ((In[0] >> 4) & 0xF);
	char Shifter = In[0] & 0xF;

	if ((unsigned char)TableIndex >= 10) return;
	
	do {
		unsigned int Sample = In[1] & 0xF;
		int ScaledSample = (short)(Sample << 12) >> Shifter;
		int Feedback = Context[0] * SomeTable[TableIndex] + Context[1] * SomeTable[TableIndex + 1];
		Feedback >>= 6;
		Context[1] = Context[0];
		short DecodedSample = ScaledSample + Feedback;
		Out[0] = DecodedSample;
		Context[0] = DecodedSample;

		Sample = In[1] & 0xF0;
		ScaledSample = (short)(Sample << 8) >> Shifter;
		Feedback = Context[0] * SomeTable[TableIndex] + Context[1] * SomeTable[TableIndex + 1];
		Feedback >>= 6;
		Context[1] = Context[0];
		DecodedSample = ScaledSample + Feedback;
		Out[1] = DecodedSample;
		Context[0] = DecodedSample;

		In += 1;
		Out += gap;
		--SampleCount;
	} while (SampleCount);
}

void Type0Decode(unsigned char nChannels, unsigned char BytesPerSamplePerChannel, unsigned char* EncodedBuffer, short* DecodedBuffer, int* DecodeContext, unsigned int DecodedBufferSize) {
	if ((int)nChannels < 2) {
		int SampleCount = BytesPerSamplePerChannel / 2;
		int BytesPerPass = BytesPerSamplePerChannel * 2;
		do {
			DecodeChannelSamples(SampleCount, EncodedBuffer, DecodedBuffer, DecodeContext, 2);
			DecodedBuffer += 2 * SampleCount;
			EncodedBuffer += 1 + SampleCount;
			DecodedBufferSize -= BytesPerPass;
		} while (DecodedBufferSize > 0);
		return;
	}

	int SampleCount = BytesPerSamplePerChannel / 2;
	int BytesPerPass = BytesPerSamplePerChannel * 2 * nChannels;
	do {
		DecodeChannelSamples(SampleCount, EncodedBuffer, DecodedBuffer, DecodeContext, 4);
		DecodedBuffer += 2;
		EncodedBuffer += 1 + SampleCount;
		DecodeChannelSamples(SampleCount, EncodedBuffer, DecodedBuffer, DecodeContext + 2, 4);
		DecodedBuffer += 4 * SampleCount - 2;
		EncodedBuffer += 1 + SampleCount;
		DecodedBufferSize -= BytesPerPass;
	} while (DecodedBufferSize > 0);
}