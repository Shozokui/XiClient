#pragma once
#include "CYyObject.h"
#include "StorageLists.h"
#include <cstdlib>
namespace FFXI {
	namespace CYy {
		class StorageListObject : public CYyObject {
		public:
			void operator delete(void*);
			void operator delete[](void*);
		};
	}
}