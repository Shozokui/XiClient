#include "CYyIcon.h"
#include "Enums.h"
#include "CYyTex.h"
#include "CYyDb.h"
#include "CYyTexMng.h"
#include "Globals.h"
using namespace FFXI::CYy;

const CYyClass CYyIcon::CYyIconClass{
	"CYyIcon", sizeof(CYyIcon), &CYyObject::CYyObjectClass
};
const CYyClass* FFXI::CYy::CYyIcon::GetRuntimeClass()
{
	return &CYyIconClass;
}

FFXI::CYy::CYyIcon::CYyIcon(int a1, unsigned char* a2) {
	this->field_4 = a1;
	this->Vertices[0].RHW = 1.0;
	this->Vertices[1].RHW = 1.0;
	this->Vertices[2].RHW = 1.0;
	this->Vertices[3].RHW = 1.0;
	this->Vertices[0].Z = 0.0;
	this->Vertices[1].Z = 0.0;
	this->Vertices[2].Z = 0.0;
	this->Vertices[3].Z = 0.0;
	this->Vertices[0].DiffuseColor = 0x80808080;
	this->Vertices[1].DiffuseColor = 0x80808080;
	this->Vertices[2].DiffuseColor = 0x80808080;
	this->Vertices[3].DiffuseColor = 0x80808080;
	this->Vertices[0].TexVertX = 0.0;
	this->Vertices[1].TexVertX = 1.0;
	this->Vertices[2].TexVertX = 0.0;
	this->Vertices[3].TexVertX = 1.0;
	this->Vertices[0].TexVertY = 0.0;
	this->Vertices[1].TexVertY = 0.0;
	this->Vertices[2].TexVertY = 1.0;
	this->Vertices[3].TexVertY = 1.0;

	char* mem = StorageLists::instance->Get(sizeof(CYyTex), Constants::Enums::MEM_MODE::Ex);
	if (mem)
		this->field_8 = new (mem) CYyTex();
	else
		return;

	this->field_8->DoThing((char*)a2 + 1, *a2 >> 7, (*a2 >> 4) & 7);
	CYyDb::g_pCYyDb->pCYyTexMng->AssignTex(this->field_8);
	++this->field_8->ReferenceCount;
}

FFXI::CYy::CYyIcon::~CYyIcon()
{
	if (this->field_8) {
		delete this->field_8;
		this->field_8 = nullptr;
	}
}
