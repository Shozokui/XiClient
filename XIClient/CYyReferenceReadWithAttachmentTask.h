#pragma once
#include "CYyReferenceReadTask.h"
#include "CMoAttachments.h"
namespace FFXI {
	namespace CYy {
		class CYyReferenceReadWithAttachmentTask : public CYyReferenceReadTask, public CMoAttachments  {
		public:
			~CYyReferenceReadWithAttachmentTask() = default;
		};
	}
}