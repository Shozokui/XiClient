#pragma once
namespace FFXI {
	namespace CYy {
		class RidListNode;
		class Zoneline;
		class Submodel;
		class RidManager {
		public:
			RidManager();
			virtual ~RidManager();
			static int ListSize;
			static RidListNode* ListHead;
			static RidListNode* ListTail;
			static RidListNode* GetHead();
			void Add(char*);
			void Clean();
			void Init();
			void InitZonelines();
			void InitSubModels();
			Submodel* Submodels;
			int SubmodelCount;
			Zoneline* Zonelines;
			int ZonelineCount;
		};
	}
}