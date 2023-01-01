#pragma once
namespace FFXI {
	namespace CYy {
		class CMoResource;
		class CMoResourceMng;
		class ResourceList {
		public:
			void Init();
			void Clear();
			void PrepareFromResource(CMoResource*, int, int, int);
			void PrepareFromLastCreated(CMoResourceMng*, int, int, int);
			bool Ready();
			CMoResource* GetNext(bool);
			CMoResource* field_0;
			CMoResourceMng* field_4;
			int field_8;
			int field_C;
			int field_10;
			CMoResource* field_14;
			int field_18;
		};
	}
}