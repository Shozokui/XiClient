#pragma once
class ISAACRandom {
	//Don't change this
	static const unsigned int TABLESIZE = 0x270;
	unsigned int randIndex{ TABLESIZE + 1 };
	unsigned int randTable[TABLESIZE];
public:
	void Seed(int);
	unsigned int rand();
	unsigned int uirand(int);
	double frand(float);
	double ufrand(float);
};