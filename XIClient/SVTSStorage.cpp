#include "SVTSStorage.h"
using namespace FFXI::CYy;

FFXI::CYy::SVTSStorage::SVTSStorage()
{
	memset(this->in_use, 0, sizeof(this->in_use));
}

FFXI::CYy::SVTSStorage::~SVTSStorage()
{
	for (int i = 0; i < sizeof(this->in_use) / sizeof(this->in_use[0]); ++i) {
		if (this->in_use[i] != 0) {
			this->storage[i].field_0->Release();
			this->storage[i].field_0 = nullptr;
		}
	}
}

bool FFXI::CYy::SVTSStorage::TryDeposit(SomeVertexThingSub* a2)
{
	for (int i = 0; i < sizeof(this->in_use) / sizeof(this->in_use[0]); ++i) {
		if (this->in_use[i] == 0) {
			this->storage[i] = *a2;
			this->cur_index += 1;
			this->index[i] = this->cur_index;
			this->in_use[i] = 1;
			return true;
		}
	}

	unsigned int v5 = 0xFFFFFFFF;
	int v4 = -1;

	for (int i = 0; i < sizeof(this->in_use) / sizeof(this->in_use[0]); ++i) {
		if (this->index[i] < v5) {
			v4 = i;
			v5 = this->index[i];
		}
	}

	if (v4 == -1) {
		return false;
	}

	this->storage[v4].field_0->Release();
	this->storage[v4] = *a2;
	this->cur_index += 1;
	this->index[v4] = this->cur_index;
	return true;
}

SomeVertexThingSub* FFXI::CYy::SVTSStorage::TryWithdraw(SomeVertexThingSub* a2)
{
	int v2 = -1;
	unsigned int v8 = 0xFFFFFFFF;
	for (int i = 0; i < sizeof(this->in_use) / sizeof(this->in_use[0]); ++i) {
		if (this->in_use[i] != 0) {
			if (this->storage[i].FVF == a2->FVF) {
				if (this->storage[i].field_18 == a2->field_18) {
					if (this->index[i] <= v8) {
						v2 = i;
						v8 = this->index[i];
					}
				}
			}
		}
	}

	if (v2 == -1) {
		return nullptr;
	}
	
	this->in_use[v2] = 0;
	this->storage[v2].field_4 = 0;
	return this->storage + v2;
}
