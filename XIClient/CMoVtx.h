#pragma once
#include "CMoResource.h"
#include "CMoVtxList.h"
namespace FFXI {
	namespace CYy {
		class CMoVtx : public CMoResource {
		public:
			CMoVtx() = default;
			virtual ~CMoVtx() = default;
			virtual void Open() override final;
			CMoVtxList list;
			int field_3C;
			int field_40;
			int field_44;
			int field_48;
			int field_4C;
		};
	}
}