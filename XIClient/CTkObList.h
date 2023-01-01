#pragma once

namespace FFXI {
	namespace CTk {
		class CTkObject; class CTkNode;
		class CTkObList {
		public:
			CTkObList();
			~CTkObList();
			void AddTail(CTkObject*);
			void InsertAfter(CTkNode*, CTkObject*);
			void InsertBefore(CTkNode*, CTkObject*);
			void RemoveAt(CTkNode*);
			bool RemoveAtIfContains(CTkObject*);
			void RemoveAll();
			CTkNode* GetHeadPosition();
			CTkNode* GetTailPosition();
			CTkObject* GetNext(CTkNode**);
			CTkObject* GetPrev(CTkNode**);
			CTkNode* GetFreeNode();

			CTkNode* Head;
			CTkNode* Tail;
			CTkNode* field_8;
			CTkNode* field_C;
			int field_10;
			int field_14;
			int field_18;
		};
	}
}