#pragma once
#include "RuntimeClass.h"
namespace FFXI {
	namespace Constants { namespace Enums { enum class MEM_MODE; } }
	namespace CYy {
		class CYyObject {
		public:
			const static CYyClass CYyObjectClass;
			static void* Wrap(int, Constants::Enums::MEM_MODE);
			static void Unwrap(void*);

			static void* WrapPlus12(int, Constants::Enums::MEM_MODE);
			static void UnwrapPlus12(void*);

			virtual const CYyClass* GetRuntimeClass();
			virtual void VObj2(int*);
			virtual unsigned int VObj3(char);
			virtual int VObj4();
			virtual void VObj5(void*);
			virtual void VObj6();
			virtual ~CYyObject() = default;
			bool IsKindOf(const CYyClass*);
		};
	}
}