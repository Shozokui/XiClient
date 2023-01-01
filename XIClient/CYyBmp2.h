#pragma once
#include "CMoResource.h"
namespace FFXI {
	namespace CYy {
		class CYyTex;
		class CYyBmp2 : public CMoResource {
		public:
			virtual void VObj5(void*) override final;
			virtual void VObj6() override final;
			virtual void Open() override final;
			virtual void Close() override final;
			CYyBmp2() = default;
			virtual ~CYyBmp2() = default;
			void DoThing();
			void DoThing2(CYyTex*);
			char Data[0x3D];
		};
	}
}