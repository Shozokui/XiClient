#pragma once
#include "CMoResource.h"
namespace FFXI {
	namespace CYy {
		class CMoSk2 : public CMoResource {
		public:
			CMoSk2();
			virtual ~CMoSk2() = default;
			virtual void Open() override final;
			virtual void Close() override final;
			char Data[0x24];
		};
	}
}