#include "KaWindow.h"
#include "_49SubList.h"
#include "TkManager.h"
#include "CTkMenuMng.h"

using namespace FFXI::CTk;

void FFXI::CTk::KaWindow::Create(short a2, short a3)
{
	this->field_0 = a2;
	this->field_4 = a3;
	this->field_8 = 0;
}

void FFXI::CTk::KaWindow::Draw(int a2, int a3, float a4, float a5, unsigned int a6, unsigned int a7, char a8)
{
	double alterv9 = (double)(this->field_0 >> 1) * (a4 - 1.0);
	double alterv10 = (double)(this->field_4 >> 1) * (a5 - 1.0);

	int v9 = (double)a2 - alterv9;
	int v10 = (double)a3 - alterv10;
	_49SubList* v11 = KaWindow::KaGraGetFrameShape(this->field_8 != 0 ? 9 : 0);
	v11->DrawTile(v9, v10, 0, 0, this->field_0, this->field_4, a4, a5, a6, a7, 0);
	if (!a8) {
		v11 = KaWindow::KaGraGetFrameShape(2);
		v11->DrawTile(v9, v10, 24, 0, this->field_0 - 48, 5, a4, a5, a6, 0, 0);
		
		v11 = KaWindow::KaGraGetFrameShape(7);
		v11->DrawTile(v9, v10, 24, this->field_4 - 5, this->field_0 - 48, 5, a4, a5, a6, 0, 0);
		
		if (this->field_4 > 0x30) {
			v11 = KaWindow::KaGraGetFrameShape(4);
			v11->DrawTile(v9, v10, 0, 24, 5, this->field_4 - 48, a4, a5, a6, 0, 0);

			v11 = KaWindow::KaGraGetFrameShape(5);
			v11->DrawTile(v9, v10, this->field_0 - 5, 24, 5, this->field_4 - 48, a4, a5, a6, 0, 0);
		}

		v11 = KaWindow::KaGraGetFrameShape(1);
		v11->DrawTile(v9, v10, 0, 0, 24, 24, a4, a5, a6, 0, 0);
		
		v11 = KaWindow::KaGraGetFrameShape(3);
		v11->DrawTile(v9, v10, this->field_0 - 24, 0, 24, 24, a4, a5, a6, 0, 0);

		v11 = KaWindow::KaGraGetFrameShape(6);
		v11->DrawTile(v9, v10, 0, this->field_4 - 24, 24, 24, a4, a5, a6, 0, 0);

		v11 = KaWindow::KaGraGetFrameShape(8);
		v11->DrawTile(v9, v10, this->field_0 - 24, this->field_4 - 24, 24, 24, a4, a5, a6, 0, 0);
		
	
		
	}

}

_49SubList* FFXI::CTk::KaWindow::KaGraGetFrameShape(int a1)
{
	if (TkManager::g_CTkMenuMng.field_BC)
		return (*TkManager::g_CTkMenuMng.field_BC)[a1];

	return nullptr;
}
