#pragma once
namespace FFXI {
	namespace CYy {
		class CMoResource;
		class TexHelper {
		public:
			void Find(CYy::CMoResource*);
			void SetTexture(int);
			void DoSomething(int, float, float, float);
			void CaccSetup();
			unsigned short GetHeight();
			unsigned short GetWidth();
			char data[16];
		};
	}
}