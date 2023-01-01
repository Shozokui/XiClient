#include "CMoD3a.h"
#include "TexHelper.h"
#include "SomeVertexThing.h"
#include "FVF152Vertex.h"
using namespace FFXI::CYy;

void FFXI::CYy::CMoD3a::Open()
{
	unsigned char* data = (unsigned char*)(this + 1);
	TexHelper* th = (TexHelper*)(data + 0x08);
	char namebackup[16];
	memcpy_s(namebackup, sizeof(namebackup), th->data, sizeof(th->data));

	th->Find(this);

	if (data[0x04] != 0) {
		*(void**)(data + 0x14) = CYyObject::Wrap(sizeof(th->data), FFXI::Constants::Enums::MEM_MODE::Ex);
		memcpy_s(*(void**)(data + 0x14), sizeof(namebackup), namebackup, sizeof(namebackup));
		*(SomeVertexThing**)(data + 0x0C) = nullptr;
	}
	else {
		unsigned short count = *(unsigned short*)(data + 0x02);
		unsigned short* pos = (unsigned short*)CYyObject::Wrap(2 * 2 * count, FFXI::Constants::Enums::MEM_MODE::Ex);
		*(void**)(data + 0x14) = pos;

		unsigned char* values = data + 0x18;
		int v9{ 0 };
		for (unsigned short i = 0; i < count; ++i) {
			pos[2 * i] = values[2];
			pos[2 * i + 1] = v9;
			v9 += 6 * values[2];
			values += 144 * values[2] + 4;
		}

		float width{ 1.0 }, height{ 1.0 };
		if (data[0x07] == 0) {
			if (*(unsigned short*)(data + 0x00) >= 1) {
				width /= (float)th->GetWidth();
				height /= (float)th->GetHeight();
			}
		}

		SomeVertexThing* svt = new SomeVertexThing();
		svt->Init(v9,  FVF152Vertex::FVF);
		svt->UpdateSubValues(v9);

		FVF152Vertex* buf = (FVF152Vertex*)svt->LockBuffer();
		if (buf == nullptr) {
			svt->UnlockBuffer();
			delete svt;
			*(SomeVertexThing**)(data + 0x0C) = nullptr;
		}
		else {
			values = data + 0x18;
			for (unsigned short i = 0; i < count; ++i) {
				unsigned int subcount = 6 * values[2];
				float* fvals = (float*)(values + 4);
				for (unsigned int j = 0; j < subcount; ++j) {
					buf->X = fvals[0];
					buf->Y = fvals[1];
					buf->Z = fvals[2];
					buf->N_X = 0.0;
					buf->N_Y = 0.0;
					buf->N_Z = 1.0;
					buf->Diffuse = *(D3DCOLOR*)(fvals + 3);
					buf->TexVertX = width * fvals[4];
					buf->TexVertY = height * fvals[5];
					buf += 1;
					fvals += 6;
				}
				values += 24 * subcount + 4;
			}
			svt->UnlockBuffer();
			*(SomeVertexThing**)(data + 0x0C) = svt;
		}
	}
}

void FFXI::CYy::CMoD3a::Close()
{
	char* data = (char*)(this + 1);
	SomeVertexThing* svt = *(SomeVertexThing**)(data + 0x0C);

	if (svt != nullptr) {
		delete svt;
	}
	void* wrapped = *(void**)(data + 0x14);
	if (wrapped != nullptr) {
		CYyObject::Unwrap(wrapped);
	}
}
