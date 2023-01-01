#include "InputMng.h"
#include "CTkMouse.h"
#include "TkManager.h"

using namespace FFXI::Input;
using namespace FFXI::Constants;
int __cdecl IsLUp(Enums::TRGTYPE a1) {
	return 0;
}
int __cdecl IsMouseLButton(Enums::TRGTYPE a1) {
	FFXI::CTk::CTkMouse* mouse = FFXI::CTk::TkManager::g_pCTkMouse;

	if (!mouse) return false;

	int v2 = 0;
	switch (a1) {
	case 0:
		v2 = mouse->field_5C & 1;
		break;
	case 1:
		v2 = mouse->field_60 & 1;
		break;
	case 2:
		v2 = mouse->field_5C & 1;
		mouse->field_60 &= ~1u;
		mouse->field_64 &= ~1u;
		mouse->field_5C &= ~1u;
		break;
	case 3:
		v2 = mouse->field_60 & 1;
		mouse->field_60 &= ~1u;
		mouse->field_64 &= ~1u;
		mouse->field_5C &= ~1u;
		break;
	case 4:
		v2 = mouse->field_58 & 1;
		break;
	case 5:
		v2 = mouse->field_64 & 1;
		break;
	default:
		return false;
	}

	return v2;
}

int __cdecl IsMouseRButton(FFXI::Constants::Enums::TRGTYPE a1) {
	FFXI::CTk::CTkMouse* mouse = FFXI::CTk::TkManager::g_pCTkMouse;

	if (!mouse) return false;

	int v2 = 0;
	switch (a1) {
	case 0:
		v2 = mouse->field_5C & 2;
		break;
	case 1:
		v2 = mouse->field_60 & 2;
		break;
	case 2:
		v2 = mouse->field_5C & 2;
		mouse->field_60 &= ~2u;
		mouse->field_64 &= ~2u;
		mouse->field_5C &= ~2u;
		break;
	case 3:
		v2 = mouse->field_60 & 2;
		mouse->field_60 &= ~2u;
		mouse->field_64 &= ~2u;
		mouse->field_5C &= ~2u;
		break;
	case 4:
		v2 = (mouse->field_58 & 2) != 0;
		break;
	case 5:
		v2 = (mouse->field_64 & 2) != 0;
		break;
	default:
		return false;
	}

	return v2;
}

int __cdecl IsMouseMButton(FFXI::Constants::Enums::TRGTYPE a1) {
	FFXI::CTk::CTkMouse* mouse = FFXI::CTk::TkManager::g_pCTkMouse;
	if (!mouse) return false;

	unsigned int v2 = 0;
	switch (a1) {
	case 0:
		v2 = mouse->field_5C & 4;
		break;
	case 1:
		v2 = mouse->field_60 & 4;
		break;
	case 2:
		v2 = (mouse->field_5C & 4) != 0;
		mouse->field_60 &= ~4u;
		mouse->field_64 &= ~4u;
		mouse->field_5C &= ~4u;
		break;
	case 3:
		v2 = (mouse->field_64 & 4) != 0;
		mouse->field_60 &= ~4u;
		mouse->field_64 &= ~4u;
		mouse->field_5C &= ~4u;
		break;
	case 4:
		v2 = (mouse->field_58 & 4) != 0;
		break;
	case 5:
		v2 = (mouse->field_64 & 4) != 0;
		break;
	default:
		return false;
	}

	return v2;
}


int(__cdecl* InputMng::IsKeyTable[360])(FFXI::Constants::Enums::TRGTYPE) = {
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, IsLUp, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,//
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,//
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, IsMouseLButton, nullptr,
	IsMouseRButton, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	IsMouseMButton, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,//
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};