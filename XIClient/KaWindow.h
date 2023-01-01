#pragma once

namespace FFXI {
	namespace CTk {
		class _49SubList;
		class KaWindow {
		public:
			void Create(short, short);
			void Draw(int, int, float, float, unsigned int, unsigned int, char);
			_49SubList* KaGraGetFrameShape(int);
			unsigned int field_0;
			unsigned int field_4;
			char field_8;
			char field_9;
			char field_A;
			char field_B;
		};
	}
}