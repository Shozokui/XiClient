#pragma once
#include "CMoResource.h"
#include "WMatrix.h"
namespace FFXI {
	namespace CYy {
		class CXiActor;
		class CYyModel;
		class CYyModelBase;
		class CMoOs2 : public CMoResource {
		public:
			static Math::WMatrix* mat_matrix;
			static bool DrawBasicFlagOne;
			static Math::WMatrix DrawBasicMatrixOne;
			static bool DrawBasicFlagTwo;
			static Math::WMatrix DrawBasicMatrixTwo;
			CMoOs2() = default;
			virtual ~CMoOs2() = default;
			virtual void Open() override final;
			virtual void Close() override final;
			void InitTypeZero();
			void InitTypeOne();
			void DrawBasic(CXiActor*, CYyModel*, CYyModelBase*, float, int*, int*);
			void FillVertexBuffers();
			unsigned char Data[0x50];
		};
	}
}