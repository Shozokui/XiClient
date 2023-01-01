#pragma once
namespace FFXI {
	namespace CYy {
		class CMoCamera;
		class CMoResource;
		class CMoPathObject;
		class AnotherSubStruct {
		public:
			static int camnum;
			AnotherSubStruct();
			~AnotherSubStruct();
			void Reset();
			void InitPath(CMoResource**);
			//field 0 is vtable
			CMoCamera** field_4;
			float field_8;
			float field_C;
			char field_10;
			char field_11;
			char field_12;
			char field_13;
			CMoPathObject* field_14;
		};
	}
}