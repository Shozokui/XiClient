#pragma once
#include "CYyObject.h"
#include "SomeVertexThing.h"
namespace FFXI {
	namespace CYy {
		class SVTSStorage : public CYyObject {
		public:
			SVTSStorage();
			~SVTSStorage();
			bool TryDeposit(SomeVertexThingSub*);
			SomeVertexThingSub* TryWithdraw(SomeVertexThingSub*);
			unsigned int cur_index;
			char in_use[32];
			SomeVertexThingSub storage[32];
			unsigned int index[32];
		};
	}
}