#include "MapChunk.h"
#include "MmbListNode.h"
using namespace FFXI::CYy;

void FFXI::CYy::MapChunk::DoTransNorms()
{
	MmbListNode* node = this->field_18;
	if (node == nullptr) return;


	this->field_40.Vec3TransNorm(this->field_F4 + 0, node->field_48[0], node->field_48[2], node->field_48[5]);
	this->field_40.Vec3TransNorm(this->field_F4 + 1, node->field_48[1], node->field_48[2], node->field_48[5]);
	this->field_40.Vec3TransNorm(this->field_F4 + 2, node->field_48[1], node->field_48[2], node->field_48[4]);
	this->field_40.Vec3TransNorm(this->field_F4 + 3, node->field_48[0], node->field_48[2], node->field_48[4]);
	this->field_40.Vec3TransNorm(this->field_F4 + 4, node->field_48[0], node->field_48[3], node->field_48[5]);
	this->field_40.Vec3TransNorm(this->field_F4 + 5, node->field_48[1], node->field_48[3], node->field_48[5]);
	this->field_40.Vec3TransNorm(this->field_F4 + 6, node->field_48[1], node->field_48[3], node->field_48[4]);
	this->field_40.Vec3TransNorm(this->field_F4 + 7, node->field_48[0], node->field_48[3], node->field_48[4]);
}
