#pragma once
#include "Enums.h"
namespace FFXI {
	namespace CYy { class MemoryHeader; class CYyObject; }
	class StorageLists {
	public:
		static StorageLists* instance;
		static bool Initialize();
		static void Uninitialize();
		virtual ~StorageLists();
		static void Clean123();
		static void Clean456();
		bool Init();
		char* Get(int, Constants::Enums::MEM_MODE);
		char* GetOrUpper(int, Constants::Enums::MEM_MODE);
		int FindLargestOpenSlot(Constants::Enums::MEM_MODE);
		void Delete(CYy::CYyObject*);
		void RunClean(int*);
		static CYy::MemoryHeader* g_pCYyDataHead;
		static CYy::MemoryHeader* g_pCYyDataTail;
		static CYy::MemoryHeader* g_pCYyElemHead;
		static CYy::MemoryHeader* g_pCYyElemTail;
		static CYy::MemoryHeader* g_pCYyLoadHead;
		static CYy::MemoryHeader* g_pCYyLoadTail;
		static CYy::MemoryHeader* g_pCYyMenuHead;
		static CYy::MemoryHeader* g_pCYyMenuTail;
		static CYy::MemoryHeader* g_pCYyExHead;
		static CYy::MemoryHeader* g_pCYyExTail;

		int field_4{ 0 };
		char* field_8{ nullptr };
		char* field_C{ nullptr };
	};
}