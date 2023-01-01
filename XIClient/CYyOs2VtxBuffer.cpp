#include "CYyOs2VtxBuffer.h"
#include "CMoOs2.h"
#include "Globals.h"
#include "CDx.h"
#include "CYyVbMng.h"
#include "CYyDb.h"
#include "CXiActor.h"
#include "XiAtelBuff.h"
#include "CMoResourceMng.h"
using namespace FFXI::CYy;

unsigned short* CYyOs2VtxBuffer::DataPointer1{ nullptr };
unsigned short* CYyOs2VtxBuffer::DataPointer2{ nullptr };

const CYyClass CYyOs2VtxBuffer::CYyOs2VtxBufferClass{
	"CYyOs2VtxBuffer", sizeof(CYyOs2VtxBuffer), &CMoTask::CMoTaskClass
};

const DWORD ShaderDecl[] = {
	0x20000000, 0x40020000, 0x20000001, 0x40020003, 0x20000002, 0x40010007, 0x0FFFFFFFF
};

const float FloatScalers[] = {
	1.0, 1.0, 1.0, 0.0, -1.0, 1.0, 1.0, 0.0, 1.0, -1.0, 1.0, 0.0, 1.0, 1.0, -1.0, 0.0
};

const CYyClass* FFXI::CYy::CYyOs2VtxBuffer::GetRuntimeClass()
{
	return &CYyOs2VtxBuffer::CYyOs2VtxBufferClass;
}

char FFXI::CYy::CYyOs2VtxBuffer::OnMove()
{
	bool v2 = false;

	CXiActor* actor = CXiActor::tempAttachStruct.GetSearchActor();
	if (actor != nullptr) {
		if (actor->AtelBuffer != nullptr) {
			if ((actor->AtelBuffer->field_120 & 0x80) != 0
				|| (actor->AtelBuffer->field_120 & 0x100) != 0) {
				v2 = true;
			}
		}
	}

	if (this->Os2Resource == nullptr)
		return 0;

	CMoOs2* os2 = *this->Os2Resource;
	if (os2 == nullptr)
		return 0;


	switch (this->State) {
	case 0:
		if (v2 == true) {
			CMoResourceMng::os2vtxbuffers[0] = this;
			this->State = 1;
			this->Substate = 0;
		}
		else {
			for (int i = 0; i < sizeof(CMoResourceMng::os2vtxbuffers) / sizeof(CMoResourceMng::os2vtxbuffers[0]); ++i) {
				if (CMoResourceMng::os2vtxbuffers[i] == nullptr) {
					CMoResourceMng::os2vtxbuffers[i] = this;
					this->State = 1;
					this->Substate = 0;
					break;
				}
			}

			return 0;
		}
		//FALLTHROUGH
	case 1:
		this->Substate = 0;
		if (this->CheckOs2Data(os2->Data + 6, &this->SomeCount))
			this->State = 2;
		else
			this->State = 7;
		if (v2 == false)
			return 0;
		//FALLTHROUGH
	case 2:
		if (this->CreateBuffers(os2->Data + 6) == false)
			return 0;
		this->State = 3;
		this->Substate = 0;
		if (v2 == false)
			return 0;
		//FALLTHROUGH
	case 3:
		if (this->FillBuffers(os2->Data + 6) == false)
			return 0;
		this->State = 4;
		this->Substate = 0;
		if (v2 == false)
			return 0;
		//FALLTHROUGH
	case 4:
		if (this->Os2Resource != nullptr) {
			this->field_6E = *(int*)((*this->Os2Resource)->Data + 30);
		}
		else {
			this->field_6E = 0x4E;
		}

		this->State = 5;
		this->Substate = 0;
		this->StateCounter = 0;
		if (v2 == false)
			return 0;
		//FALLTHROUGH
	case 5:
		if ((os2->Data[4] & 1) == 0 || this->ParseFloats()) {
			this->State = 6;
			this->Substate = 0;
			this->StateCounter = 0;
			if (v2 == false)
				return 0;
		} else
			return 0;
		break;
	case 6:
		if (v2 == false) {
			this->StateCounter += 1;
			if (this->StateCounter >= 3) {
				this->Substate = 0;
				this->State = 7;
			}
			return 0;
		}
		break;
	case 7:
		break;
	default:
		return 0;
	}

	os2->DoSomething3()->PreHeader.field_18 -= 1;
	this->field_68 = 1;
	this->State = 8;
	this->Substate = 0;

	for (int i = 0; i < sizeof(CMoResourceMng::os2vtxbuffers) / sizeof(CMoResourceMng::os2vtxbuffers[0]); ++i) {
		if (CMoResourceMng::os2vtxbuffers[i] == this) {
			CMoResourceMng::os2vtxbuffers[i] = nullptr;
			break;
		}
	}

	return 0;
}

FFXI::CYy::CYyOs2VtxBuffer::CYyOs2VtxBuffer(CMoOs2** a2)
{
	this->State = 0;
	this->Os2Resource = a2;
	
	CMoOs2* os2{ nullptr };
	if (a2)
		os2 = *a2;

	//os2 can be nullptr as per client
	os2->DoSomething3();
	os2->PreHeader.field_18 += 1;

	this->field_5C = 0;
	this->field_60 = 0;
	this->IndexBuffer = nullptr;
	this->field_44 = 0;
	this->field_48 = 0;
	this->VertexBuffer = nullptr;
	this->field_50 = 0;
	this->field_54 = 0;
	this->SomeCount = 0;
	this->field_68 = 0;
	this->field_69 = 0;
	this->field_6A = 0;
	this->field_6E = 0;
	this->field_72 = 0;
	this->field_76 = 0;
	this->field_7A = 0;

	FFXI::CYy::CDx::instance->DXDevice->CreateVertexShader(ShaderDecl, NULL, &this->ShaderHandle, NULL);
}

FFXI::CYy::CYyOs2VtxBuffer::~CYyOs2VtxBuffer()
{
	if (this->field_5C != nullptr) {
		CYyObject::Unwrap(this->field_5C);
		this->field_5C = nullptr;
	}

	this->field_60 = 0;
	if (this->IndexBuffer != nullptr) {
		this->IndexBuffer->Release();
		this->IndexBuffer = nullptr;
	}

	if (this->VertexBuffer != nullptr) {
		CYyDb::g_pCYyDb->pCYyVbMng->DoSomething(&this->VertexBuffer);
		this->VertexBuffer = nullptr;
	}

	this->field_44 = 0;
	this->field_48 = 0;
	this->field_50 = 0;
	this->field_54 = 0;
	this->SomeCount = 0;

	if (this->ShaderHandle != 0) {
		FFXI::CYy::CDx::instance->DXDevice->DeleteVertexShader(this->ShaderHandle);
		this->ShaderHandle = 0;
	}

	this->field_68 = 0;
	this->field_69 = 0;

	if (this->field_6A != nullptr) {
		CYyObject::Unwrap(this->field_6A);
		this->field_6A = nullptr;
	}

	this->field_6E = 0;
	if (this->field_72 != nullptr) {
		CYyObject::Unwrap(this->field_72);
		this->field_72 = nullptr;
	}

	this->field_76 = 0;
	for (int i = 0; i < sizeof(CMoResourceMng::os2vtxbuffers) / sizeof(CMoResourceMng::os2vtxbuffers[0]); ++i) {
		if (CMoResourceMng::os2vtxbuffers[i] == this) {
			CMoResourceMng::os2vtxbuffers[i] = nullptr;
			break;
		}
	}

	if (this->field_7A != nullptr) {
		CYyObject::Unwrap(this->field_7A);
		this->field_7A = nullptr;
	}
}

bool FFXI::CYy::CYyOs2VtxBuffer::CheckOs2Data(unsigned char* a2, int* a3)
{
	unsigned short* shortdata = *(reinterpret_cast<unsigned short**>(a2));
	while (*shortdata != 0xFFFF) {
		switch (*shortdata) {
			case 0x8000u:
				shortdata += 9;
				break;
			case 0x8010u:
			{
				unsigned int v13[11];
				memcpy(v13, shortdata + 1, sizeof(v13));
				int v7 = (v13[3] >> 8) & 0xF;
				if ((v13[7] & 0xFFFF) != 0 && v7 == 4) {
					shortdata[7] &= 0xF0FF;
					shortdata[7] |= 0x0800;
					this->field_69 = 1;
				}
				else if (v7 == 1 || v7 == 2 || v7 == 3 || v7 == 4) {
					this->field_69 = 1;
				}
				shortdata += 23;
			}
				break;
			case 0x5453u:
				*a3 += 2 + shortdata[1];
				shortdata += 5 * shortdata[1] + 12;
				break;
			case 0x4353u:
				shortdata += shortdata[1] + 6;
				break;
			case 0x43u:
				shortdata += 5 * shortdata[1] + 2;
				break;
			case 0x54u:
				*a3 += 3 * shortdata[1];
				shortdata += 15 * shortdata[1] + 2;
				break;
		}
	}

	return *a3 != 0;
}

bool FFXI::CYy::CYyOs2VtxBuffer::CreateBuffers(unsigned char* a2)
{
	if (this->SomeCount == 0)
		return true;

	switch (this->Substate) {
	case 0:
		this->field_5C = CYyObject::Wrap(2 * this->SomeCount, FFXI::Constants::Enums::MEM_MODE::Ex);
		if (this->field_5C != nullptr) {
			for (int i = 0; i < this->SomeCount; ++i) {
				((short*)this->field_5C)[i] = -1;
			}
			this->Substate = 1;
			return false;
		}
		break;
	case 1:
		if (FFXI::CYy::CDx::instance->DXDevice->CreateIndexBuffer(
			2 * this->SomeCount,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			&this->IndexBuffer) >= D3D_OK) {
			this->Substate = 2;
			return false;
		}
		break;
	case 2:
		this->Substate = 3;
		return false;
	case 3:
		this->Substate = 4;
		return false;
	case 4:
		if (CYyDb::g_pCYyDb->pCYyVbMng->InitBuffer(
			8 * this->SomeCount + 4, 
			520, 
			NULL, 
			D3DPOOL_DEFAULT, 
			&this->VertexBuffer, 
			0) >= D3D_OK) {
			this->Substate = 5;
			return false;
		}
		break;
	case 5:
		this->Substate = 6;
		return false;
	case 6:
		this->Substate = 7;
		return false;
	default:
		return true;
	}

	//Some failure occurred to get here
	if (this->field_5C != nullptr) {
		CYyObject::Unwrap(this->field_5C);
		this->field_5C = nullptr;
	}

	this->field_60 = 0;
	if (this->IndexBuffer != nullptr) {
		this->IndexBuffer->Release();
		this->IndexBuffer = 0;
	}
	if (this->VertexBuffer != nullptr) {
		CYyDb::g_pCYyDb->pCYyVbMng->DoSomething(&this->VertexBuffer);
		this->VertexBuffer = nullptr;
	}
	this->field_44 = 0;
	this->field_48 = 0;
	this->field_50 = 0;
	this->field_54 = 0;
	
	return false;
}

bool FFXI::CYy::CYyOs2VtxBuffer::FillBuffers(unsigned char* a2)
{
	if (this->IndexBuffer == nullptr)
		return true;

	CMoOs2* os2{ nullptr };
	if (this->Os2Resource != nullptr)
		os2 = *this->Os2Resource;
	//os2 can be nullptr as per client

	if (this->Substate == 0) {
		this->field_7A = CYyObject::Wrap(8 * this->SomeCount, FFXI::Constants::Enums::MEM_MODE::Ex);
		if (this->SomeCount != 0) {
			int* intdata = (int*)this->field_7A;
			for (int i = 0; i < this->SomeCount; ++i) {
				intdata[i] = -1;
				intdata[this->SomeCount + i] = -1;
			}
		}
		this->field_82 = 0;

		this->field_7E = *reinterpret_cast<unsigned char**>(a2);
		this->field_60 = 0;
		this->Substate = 1;

		return false;
	}
	else if (this->Substate == 1) {
		this->StateCounter = 0;

		BYTE* indexbuf{ nullptr }, * vertexbuf{ nullptr };
		while (this->IndexBuffer->Lock(0, 2 * this->SomeCount, &indexbuf, NULL) != D3D_OK)
			Sleep(0);
		while (this->VertexBuffer->Lock(0, 8 * this->SomeCount, &vertexbuf, NULL) != D3D_OK)
			Sleep(0);

		unsigned short* shortindexbuff = (unsigned short*)indexbuf;
		int* intvertexbuff = (int*)vertexbuf;

		//os2 can be nullptr as per client
		unsigned short* short18 = *(unsigned short**)(os2->Data + 18);
		int* int7A = (int*)this->field_7A;
		unsigned short* short7E = (unsigned short*)this->field_7E;
		while (*short7E != 0xFFFFu) {
			switch (*short7E) {
				case 0x8000u:
					this->field_7E += 18;
					break;
				case 0x8010u:
					this->field_7E += 46;
					break;
				case 0x5453u:
					for (int i = 0; i < 3; ++i) {
						unsigned short* v21 = short7E + 2 + i;
						int* v13 = (int*)(short7E + 5 + 4 * i);
						int v24 = *v21;
						int* v16 = int7A + 2 * v24;
						if ((v16[0] != -1 || v16[1] != -1)
							&& (v16[0] != v13[0] || v16[1] != v13[1])) {
							if (this->field_60 < this->SomeCount) {
								if (this->field_5C != nullptr) {
									*((unsigned short*)this->field_5C + this->field_60) = v24;
									v24 = this->field_60 + short18[0] + short18[1];
									this->field_60 += 1;
								}
							}
							else {
								this->field_60 = 0;
							}
						}

						shortindexbuff[this->field_82] = v24;
						this->field_82 += 1;

						intvertexbuff[2 * v24] = v13[0];
						intvertexbuff[2 * v24 + 1] = v13[1];
						int7A[2 * v24] = v13[0];
						int7A[2 * v24 + 1] = v13[1];
					}

					for (int i = 0; i < (short7E[1] - 1); ++i) {
						unsigned short* v27 = short7E + 17 + 5 * i;
						int* v13 = (int*)(v27 + 1);
						int v31 = *v27;
						int* v16 = int7A + 2 * v31;
						if ((v16[0] != -1 || v16[1] != -1)
							&& (v16[0] != v13[0] || v16[1] != v13[1])) {
							if (this->field_60 < this->SomeCount) {
								if (this->field_5C != nullptr) {
									*((unsigned short*)this->field_5C + this->field_60) = v31;
									v31 = this->field_60 + short18[0] + short18[1];
									this->field_60 += 1;
								}
							}
							else {
								this->field_60 = 0;
							}
						}

						shortindexbuff[this->field_82] = v31;
						this->field_82 += 1;

						intvertexbuff[2 * v31] = v13[0];
						intvertexbuff[2 * v31 + 1] = v13[1];
						int7A[2 * v31] = v13[0];
						int7A[2 * v31 + 1] = v13[1];
					}
					this->field_7E += 24 + 10 * short7E[1];
					break;
				case 0x4353u:
					this->field_7E += 12 + 2 * short7E[1];
					break;
				case 0x43:
					this->field_7E += 4 + 10 * short7E[1];
					break;
				case 0x54:
					for (int i = 0; i < short7E[1]; ++i) {
						unsigned short* v11 = short7E + 2 + 15 * i;
						for (int j = 0; j < 3; ++j) {
							unsigned int* v13 = (unsigned int*)(v11 + 3) + 2 * j;
							int v24 = v11[j];
							int* v16 = int7A + 2 * v24;
							if ((v16[0] != -1 || v16[1] != -1)
								&& (v16[0] != v13[0] || v16[1] != v13[1])) {
								if (this->field_60 < this->SomeCount) {
									if (this->field_5C != nullptr) {
										*((unsigned short*)this->field_5C + this->field_60) = v24;
										v24 = this->field_60 + short18[0] + short18[1];
										this->field_60 += 1;
									}
								}
								else {
									this->field_60 = 0;
								}
							}

							shortindexbuff[this->field_82] = v24;
							this->field_82 += 1;
							intvertexbuff[2 * v24] = v13[0];
							intvertexbuff[2 * v24 + 1] = v13[1];
							int7A[2 * v24] = v13[0];
							int7A[2 * v24 + 1] = v13[1];
						}
					}
					this->field_7E += 4 + 30 * short7E[1];
					break;
			}
			
			this->StateCounter += 1;
			if (this->StateCounter > 1000) {
				while (this->IndexBuffer->Unlock() != D3D_OK)
					Sleep(0);
				while (this->VertexBuffer->Unlock() != D3D_OK)
					Sleep(0);

				return false;
			}

			short7E = (unsigned short*)this->field_7E;
		}

		if (this->field_7A) {
			CYyObject::Unwrap(this->field_7A);
			this->field_7A = nullptr;
		}

		while (this->IndexBuffer->Unlock() != D3D_OK)
			Sleep(0);
		while (this->VertexBuffer->Unlock() != D3D_OK)
			Sleep(0);

		return true;
	}
	else
		return true;
}

bool FFXI::CYy::CYyOs2VtxBuffer::ParseFloats()
{
	CMoOs2* os2{ nullptr };
	if (this->Os2Resource != nullptr)
		os2 = *this->Os2Resource;

	//os2 can be nullptr as per the client
	unsigned short* short18 = *(unsigned short**)(os2->Data + 18);

	if (this->Substate == 0) {
		int size = 64 + 24 * short18[0] + 56 * short18[1];
		this->field_72 = CYyObject::Wrap(size, FFXI::Constants::Enums::MEM_MODE::Ex);
		if (this->field_72 != nullptr) {
			int ptr = (int)this->field_72 + 63;
			ptr &= 0xFFFFFFC0;
			this->field_76 = ptr;
			this->Substate = 1;
			this->StateCounter = 0;
			this->field_86 = 0;
		}

		return false;
	}
	else if (this->Substate == 1) {
		this->StateCounter = 0;
		CYyOs2VtxBuffer::DataPointer1 = *(unsigned short**)(os2->Data + 24);
		while (this->field_86 < short18[0]) {
			int offset = 24 * this->field_86;
			float* setter = (float*)(this->field_76 + offset);
			float* getter = (float*)(this->field_6E + offset);
			unsigned int index = CYyOs2VtxBuffer::DataPointer1[2 * this->field_86];
			index = (index >> 12) & 0xC;
			setter[0] = getter[0] * FloatScalers[index + 0];
			setter[1] = getter[1] * FloatScalers[index + 1];
			setter[2] = getter[2] * FloatScalers[index + 2];
			setter[3] = getter[3] * FloatScalers[index + 0];
			setter[4] = getter[4] * FloatScalers[index + 1];
			setter[5] = getter[5] * FloatScalers[index + 2];

			this->StateCounter += 1;
			if (this->StateCounter > 1000)
				return false;

			this->field_86 += 1;
		}

		this->Substate = 2;
		this->StateCounter = 0;
		this->field_86 = 0;
		return false;
	}
	else if (this->Substate == 2) {
		this->StateCounter = 0;
		CYyOs2VtxBuffer::DataPointer1 = *(unsigned short**)(os2->Data + 24);
		CYyOs2VtxBuffer::DataPointer2 = DataPointer1 + 2 * short18[0];
		
		while (this->field_86 < short18[1]) {
			int offset = 24 * short18[0] + 56 * this->field_86;
			float* setter = (float*)(this->field_76 + offset);
			float* getter = (float*)(this->field_6E + offset);

			int index1 = DataPointer2[2 * this->field_86];
			int index2 = DataPointer2[2 * this->field_86 + 1];
			index1 = (index1 >> 12) & 0xC;
			index2 = (index2 >> 12) & 0xC;
			setter[0] = getter[0] * FloatScalers[index1 + 0];
			setter[1] = getter[1] * FloatScalers[index2 + 0];
			setter[2] = getter[2] * FloatScalers[index1 + 1];
			setter[3] = getter[3] * FloatScalers[index2 + 1];
			setter[4] = getter[4] * FloatScalers[index1 + 2];
			setter[5] = getter[5] * FloatScalers[index2 + 2];
			setter[6] = getter[6];
			setter[7] = getter[7];
			setter[8] = getter[8] * FloatScalers[index1 + 0];
			setter[9] = getter[9] * FloatScalers[index2 + 0];
			setter[10] = getter[10] * FloatScalers[index1 + 1];
			setter[11] = getter[11] * FloatScalers[index2 + 1];
			setter[12] = getter[12] * FloatScalers[index1 + 2];
			setter[13] = getter[13] * FloatScalers[index2 + 2];

			this->StateCounter += 1;
			if (this->StateCounter > 1000)
				return false;

			this->field_86 += 1;
		}

		return true;
	}
	else
		return true;
}
