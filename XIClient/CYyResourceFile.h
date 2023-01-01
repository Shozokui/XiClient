#pragma once
#include "CMoResource.h"

namespace FFXI {
	namespace File { class ReferenceReadBase; }
	namespace CYy {
		class CXiActor;
		class CYyResourceFile : public CMoResource {
		public:
			CYyResourceFile();
			virtual ~CYyResourceFile();
			static void ReferenceRead(CYyResourceFile***, void(__cdecl*)(CYyResourceFile**, File::ReferenceReadBase*), File::ReferenceReadBase*, CXiActor*, CXiActor*);
			void StopMovers();
			int field_30;
			int FileIndex;
			int field_38;
			CMoResource** TerminateNode;
		};
	}
}