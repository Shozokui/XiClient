#pragma once
#include "CMoTask.h"
#include "d3d8to9/d3d8.hpp"
namespace FFXI {
	namespace CYy {
		class CMoOs2;
		class CYyOs2VtxBuffer : public CMoTask {
		public:
			const static CYyClass CYyOs2VtxBufferClass;
			static unsigned short* DataPointer1;
			static unsigned short* DataPointer2;
			virtual const CYyClass* GetRuntimeClass() override final;
			virtual char OnMove() override final;
			CYyOs2VtxBuffer(CMoOs2**);
			~CYyOs2VtxBuffer();
			bool CheckOs2Data(unsigned char*, int*);
			bool CreateBuffers(unsigned char*);
			bool FillBuffers(unsigned char*);
			bool ParseFloats();
			CMoOs2** Os2Resource;
			int State;
			int Substate;
			IDirect3DIndexBuffer8* IndexBuffer;
			IDirect3DVertexBuffer8* field_44;
			IDirect3DVertexBuffer8* field_48;
			IDirect3DVertexBuffer8* VertexBuffer;
			IDirect3DVertexBuffer8* field_50;
			IDirect3DVertexBuffer8* field_54;
			int SomeCount;
			void* field_5C;
			int field_60;
			DWORD ShaderHandle;
			char field_68;
			char field_69;
			void* field_6A;
			int field_6E;
			void* field_72;
			int field_76;
			void* field_7A;
			unsigned char* field_7E;
			int field_82;
			int field_86;
			int StateCounter;
			int field_8E;
			int field_92;
			int field_96;
			int field_9A;
			char field_9E;
			char field_9F;
		};
	}
}