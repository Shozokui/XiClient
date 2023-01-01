#include "CMoD3bElem.h"
#include "CYyGenerator.h"
#include "CYyDb.h"
#include "BaseProcessor.h"
#include "CMoD3b.h"
#include <iostream>
FFXI::CYy::CMoD3bElem::CMoD3bElem()
{
	std::cout << "D3B CREATED. " << std::endl;
	this->field_176 = sizeof(CMoD3bElem);

	for (int i = 0; i < 5; ++i) {
		this->field_194[i] = 0.0;
		this->field_1A8[i] = 0.0;
	}

	this->field_1BC = nullptr;
}

FFXI::CYy::CMoD3bElem::~CMoD3bElem()
{
	if (this->field_1BC != nullptr) {
		CYyObject::Unwrap(this->field_1BC);
	}
}

bool FFXI::CYy::CMoD3bElem::VirtElem1(FFXI::Constants::Enums::ElemType a2)
{
	return a2 == FFXI::Constants::Enums::ElemType::D3b;
}

void FFXI::CYy::CMoD3bElem::OnDraw()
{
	CYyGenerator* gen{ nullptr };
	if (this->field_FC != nullptr) {
		gen = *this->field_FC;
	}

	if (gen->field_DE >= 0 || CYyGenerator::GetSomeGeneratorScalar() != 0.0) {
		if (this->res != nullptr) {
			CMoD3b* d3b = (CMoD3b*)*this->res;
			d3b->Draw(this, &CMoElem::SomeColor, this->field_194, this->field_1A8, this->field_1BC, 0);
			this->field_1BC = nullptr;
			memcpy(this->field_1A8, this->field_194, sizeof(this->field_1A8));
		}
	}
}

void FFXI::CYy::CMoD3bElem::VirtElem9()
{
	if (this->res == nullptr) {
		return;
	}

	CMoD3b* d3b = (CMoD3b*) *this->res;
	if (d3b == nullptr) {
		return;
	}

	this->field_1BC = d3b->MakeObj(this->field_194, this->field_1A8);
	this->SomeMatrixCalc();
	CYyDb::g_pCYyDb->pMoProcessor->VirtProcessor31(CMoElem::Floats, this->field_1BC, 36, d3b->field_34, &this->field_A0);
	CYyObject::Unwrap(this->field_1BC);
	this->field_1BC = nullptr;
}
