#pragma once
#include "StorageListObject.h"
#include "CMoResource.h"
#include "Values.h"
#include "ISAACRandom.h"
namespace FFXI {
	namespace CTk { class _49SubList; }
	namespace CYy {
		class CYyResourceFile;
		class CYyScheduler;
		class CYyOs2VtxBuffer;
		class CXiActor;
		class CMoResourceMng : public StorageListObject {
		public:
			const static CYyClass CMoResourceMngClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			~CMoResourceMng();
			CMoResourceMng();			
			static int SomeFileMappingFunc(int, int);
			static bool __fastcall CheckResourceThing(CMoResource***);
			static CMoResource*** FindNextUnder(CMoResource***, CMoResource*, int, int);
			static void InitSomeStatics();
			static char SomeByte;
			static CMoResource** LastResourceCreatedByType[Constants::Values::COUNT_RESOURCE_TYPES];
			static CMoResource** LastResourceCreated;
			static int ActiveFileLoadTaskCount;
			static int NumFileIndex;
			static ISAACRandom rng;
			static CYyResourceFile** file27;
			static CYyScheduler** damv;
			static CYyScheduler** misv;
			static CYyScheduler** curv;
			static CYyOs2VtxBuffer* os2vtxbuffers[16];
			void RemoveRes(CMoResource**);
			void Unlink(CMoResource**);
			//The only difference between these two is the check on the readfile retval
			CYyResourceFile*** LoadNumFile(CYyResourceFile***, int);
			CYyResourceFile*** LoadNumFile2(CYyResourceFile***, int);
			CYyResourceFile*** Parse(char*, int, CYyResourceFile***, CMoResource**);
			CYyResourceFile** GetOrLoadDatByIndex(int);
			CTk::_49SubList*** LoadNumFileGet49(int, CYyResourceFile***);
			CMoResource** Link(CMoResource*);
			bool InList(CMoResource**);
			static void SetNumFileIndex(int);
			static int GetNumFileIndex();
			void LoadStartScheduler(int, int, CXiActor*, CXiActor*, unsigned int);
			void InitUnk3(int);
			int GetMod(int);
			void** ReserveSlot();
			void* NextFreeSlot;
			void* Resources[0x10000];
			CYyResourceFile** Unknown3;
			int MaybeResourceCount;
		};
	}
}