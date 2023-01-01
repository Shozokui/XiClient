#pragma once
namespace FFXI {
	namespace CYy { class CMoTask; }
	class OTStruct {
	public:
		static void(__cdecl* DefaultCallback)(CYy::CMoTask*);
		OTStruct();
		~OTStruct();
		float field_0;
		OTStruct* previous;
		void(__cdecl* Callback)(CYy::CMoTask*);
		CYy::CMoTask* field_C;
		OTStruct* next;
		int field_14;
		int field_18;
		int field_1C;
	};

}