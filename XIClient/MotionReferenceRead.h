#pragma once
#include "ReferenceReadBase.h"
namespace FFXI {
	namespace CYy { class CXiActor;	class CYyResourceFile; }
	namespace File {
		class MotionReferenceRead : public ReferenceReadBase {
		public:
			static void ReadCallback(CYy::CYyResourceFile**, ReferenceReadBase*);
			int field_10;
		};
	}
}