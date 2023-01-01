#pragma once
#include "CMoResource.h"
#include "KO_Path.h"
namespace FFXI {
	namespace CYy {
		class CMoSphRes : public CMoResource {
		public:
			static const CYyClass CMoSphResClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			CMoSphRes() = default;
			virtual ~CMoSphRes() = default;
			virtual void Open() override final;
			virtual void Close() override final;
			virtual void VObj6() override final;
			FFXI::KO_Path path;
		};
	}
}