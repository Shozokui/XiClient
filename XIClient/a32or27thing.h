#pragma once
#include "CMoResource.h"
#include "TexHelper.h"
namespace FFXI {
	namespace CYy {
		class a32or27thing : public CMoResource {
		public:
			virtual void Open() override final;
			void Init(char*);
			TexHelper helper;
		};

	}
}