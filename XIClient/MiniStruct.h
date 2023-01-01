#pragma once
namespace FFXI {
	namespace CYy {
		class MapChunk;
		class MiniStruct {
		public:
			MapChunk* chunk;
			void* data;
			int field_8;
		};
	}
}