#pragma once
#include "CYyObject.h"
#include "FVF144Vertex.h"

namespace FFXI {
	namespace CYy {
		class CYyTex;
		class CYyIcon : public CYyObject {
		public:
			const static CYyClass CYyIconClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			CYyIcon(int, unsigned char*);
			~CYyIcon();
			int field_4;
			CYyTex* field_8;
			FVF144Vertex Vertices[4];
		};
	}
}