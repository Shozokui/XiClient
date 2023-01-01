#pragma once
#include "CMoResource.h"
namespace FFXI {
	namespace CYy {
		class CMoKeyframe : public CMoResource {
		public:
			virtual void Open() override final;
			double GetFrameValue(float);
			double GetFrameValue(float, float);
			double GetSplineFrameValue(float);
			double GetSplineFrameValue(float, float);
			CMoKeyframe() = default;
			virtual ~CMoKeyframe() = default;
			float field_30;
			float field_34;
			float field_38;
			float field_3C;
		};
	}
}