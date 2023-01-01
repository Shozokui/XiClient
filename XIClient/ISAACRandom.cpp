#include "ISAACRandom.h"

const int arr[] = { 0, 0x9908B0DF };
void ISAACRandom::Seed(int a2)
{
	for (int i = 0; i < TABLESIZE; ++i) {
		randTable[i] = a2 & 0xFFFF0000;
		randTable[i] |= (unsigned int)(0x10DCD * a2 + 1) >> 16;
		a2 = 0x10DCD * (0x10DCD * a2 + 1) + 1;
	}
	randIndex = TABLESIZE;
}

unsigned int ISAACRandom::rand()
{
	if (randIndex >= TABLESIZE) {
		if (randIndex == (TABLESIZE + 1))
			Seed(0x1105);
		int i{ 0 };
		for (i = 0; i < 0xE3; ++i)
			randTable[i] = randTable[i + 0x18D] ^ ((randTable[i] ^ (randTable[i] ^ randTable[i + 1]) & 0x7FFFFFFFu) >> 1) ^ arr[randTable[i + 1] & 1];

		for (i; i < TABLESIZE - 1; ++i) {
			int v3 = randTable[i];
			int v4 = randTable[i - 0xE3];
			int v5 = (randTable[i] ^ randTable[i + 1]) & 0x7FFFFFFF;
			randTable[i] = ((v3 ^ (unsigned int)v5) >> 1) ^ v4 ^ arr[((unsigned char)v3 ^ (unsigned char)v5) & 1];
		}

		randIndex = 0;
		randTable[0x26F] = randTable[0x18C] ^ ((randTable[0x26F] ^ (randTable[0x26F] ^ randTable[0]) & 0x7FFFFFFFu) >> 1) ^ arr[randTable[0] & 1];
	}

	unsigned int v6 = randTable[randIndex];
	randIndex += 1;
	unsigned int v7 = (((((((v6 >> 11) ^ v6) & 0xFF3A58AD) << 7) ^ (v6 >> 11) ^ v6) & 0xFFFFDF8C) << 15) ^ ((((v6 >> 11) ^ v6) & 0xFF3A58AD) << 7) ^ (v6 >> 11) ^ v6;
	return v7 ^ (v7 >> 18);

}

unsigned int ISAACRandom::uirand(int a2)
{
	if (a2)
		return rand() % a2;

	return 0;
}

double ISAACRandom::ufrand(float a2)
{
	if (a2 == 0.0)
		return 0.0;

	return (double)(rand() % 0x186A0) * 0.0000099999997 * a2;
}

double ISAACRandom::frand(float a2)
{
	if (a2 == 0.0)
		return 0.0;

	return ((double)(rand() % 0x30D40u) * 0.0000099999997 - 1.0) * a2;
}
