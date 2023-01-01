#include "UnderscoreAtStruct.h"
using namespace FFXI::CYy;

FFXI::CYy::UnderscoreAtStruct::UnderscoreAtStruct()
{
	this->field_1A8 = 0;
}

FFXI::CYy::UnderscoreAtStruct::~UnderscoreAtStruct()
{
	//nullsub
}

void FFXI::CYy::UnderscoreAtStruct::InitMatrix(int subchunkindex)
{
	FFXI::Math::WMatrix a1{}, a3{};
	a1.Identity();

	//Scaling
	D3DXMatrixScaling(&a3,
		this->Scalings[subchunkindex].x,
		this->Scalings[subchunkindex].y,
		this->Scalings[subchunkindex].z);
	a1.MatrixMultiply(&a3);

	//Rotation
	D3DXMatrixRotationX(&a3, this->Rotations[subchunkindex].x);
	a1.MatrixMultiply(&a3);
	D3DXMatrixRotationY(&a3, this->Rotations[subchunkindex].y);
	a1.MatrixMultiply(&a3);
	D3DXMatrixRotationZ(&a3, this->Rotations[subchunkindex].z);
	a1.MatrixMultiply(&a3);

	//Translating
	D3DXMatrixTranslation(&a3, 
		this->Translations[subchunkindex].x,
		this->Translations[subchunkindex].y,
		this->Translations[subchunkindex].z);

	D3DXMatrixMultiply(this->field_98 + subchunkindex, &a1, &a3);
}
