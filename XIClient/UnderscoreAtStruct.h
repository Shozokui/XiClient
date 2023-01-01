#pragma once
#include "StorageListObject.h"
#include "WMatrix.h"
namespace FFXI {
	namespace CYy {
		class MapChunk;
		class UnderscoreAtStruct : public StorageListObject {
		public:
			UnderscoreAtStruct();
			~UnderscoreAtStruct();
			void InitMatrix(int);
			int FourCC;
			D3DXVECTOR3 Translations[4];
			D3DXVECTOR3 Rotations[4];
			D3DXVECTOR3 Scalings[4];
			FFXI::Math::WMatrix field_98[4];
			MapChunk* Subchunks[4];
			char field_1A8;
			char SubchunkCount;
			char field_1AA;
			char field_1AB;
		};
	}
}