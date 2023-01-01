#pragma once
#define WIN32_LEAN_AND_MEAN
#include "d3dx9math.h"
namespace FFXI {
	namespace CYy {
		class MmbListNode; 
		class CYyResourceFile;
		class MmbManager {
		private:
			MmbListNode* Create();
		public:
			MmbManager();
			~MmbManager();
			MmbListNode* GetByName(const char*, bool);
			MmbListNode* AddFromData(CYyResourceFile**, int*, int, CYyResourceFile*, int, D3DXVECTOR4*, int);
			int GetSomething();
			void Clean(MmbListNode*);
			MmbListNode* ListHead;
			MmbListNode* ListTail;
			int ListCount;
		};
	}
}