#pragma once
#include "StorageListObject.h"
#include "ResourcePreHeader.h"
#include "ResourceHeader.h"
namespace FFXI {
	class XiDateTime;
	namespace Math { class WMatrix; }
	namespace CYy {
		class CMoResource : public StorageListObject {
		public:
			const static CYyClass CMoResourceClass;
			virtual const CYyClass* GetRuntimeClass() override;
			virtual ~CMoResource();
			virtual void VObj2(int*) override;
			virtual void VObj5(void*) override;
			virtual void ConstructFromData(char*);
			virtual void Open();
			virtual void Close();
			virtual FFXI::Math::WMatrix* VRes4();
			static CMoResource* OpenResource(char*, CMoResource*);
			static CMoResource*** SomeAssign(CMoResource***, CMoResource***, CMoResource**);
			static void __fastcall DoTheThing(CMoResource*);
			static void UnlinkFromManager(CMoResource***);
			static unsigned int SomeCounter;
			CMoResource();
			CMoResource* GetHead();
			void DoSomething2();
			CMoResource* DoSomething3();
			void DoSomething4(char);
			int GetSomething();
			void SomeClean2();
			void SomeClean3();
			void SomeClean2Helper();
			CMoResource*** FindResourceUnder(CMoResource***, int, int);
			void FindResourceNextUnder(CMoResource***, int, int, int);
			CMoResource*** AnotherResourceSearcher(CMoResource***, int, int);
			bool FindResourceWithCallback(int, int, bool(__cdecl*)(CMoResource**, void*), void*);
			void GetActivateTime(XiDateTime*);
			void CheckGenerators();

			ResourcePreHeader PreHeader;
			ResourceHeader Header;
		};
	}
}