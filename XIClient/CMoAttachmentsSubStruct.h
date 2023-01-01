#pragma once

namespace FFXI {
	namespace CYy {
		class CXiActor;
		class CMoAttachmentsSubStruct {
		public:
			CMoAttachmentsSubStruct();
			virtual ~CMoAttachmentsSubStruct();
			void SetValues(int, int);
			void DoSomething(CXiActor*);
			CXiActor* GetActor();
			CXiActor* GetSearchActor();
			CXiActor* TWOGetSearchActor();
			int field_4;
			int field_8;
		};
	}
}