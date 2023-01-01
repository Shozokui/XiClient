#pragma once
#include "CYyModelDt.h"
namespace FFXI {
	namespace CYy {
		class CYyBasicDt : public CYyModelDt {
		public:
			virtual void VirtModelDt1(CXiActor*, CYyModel*, CYyModelBase*, float, int*, int*) override final;
			virtual void VirtModelDt2() override final;
			virtual void VirtModelDt3() override final;
			virtual void VirtModelDt4(unsigned char*) override final;
			virtual CYyOs2VtxBuffer* VirtModelDt5() override final;
		};
	}
}