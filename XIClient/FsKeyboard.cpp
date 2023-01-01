#include "FsKeyboard.h"

using namespace FFXI::Input;

void FFXI::Input::FsKeyboard::SetFlag1(unsigned char a1)
{
	this->KeyFlags[a1] |= 1u;
}

void FFXI::Input::FsKeyboard::SetFlag2(unsigned char a1)
{
	this->KeyFlags[a1] |= 2u;
}

void FFXI::Input::FsKeyboard::SetFlag4(unsigned char a1)
{
	this->KeyFlags[a1] |= 4u;
	this->field_100 = 1;
}

void FFXI::Input::FsKeyboard::SetFlag8(unsigned char a1)
{
	this->KeyFlags[a1] |= 8u;
}

void FFXI::Input::FsKeyboard::SetFlag10(unsigned char a1)
{
	this->KeyFlags[a1] |= 10u;
}

bool FFXI::Input::FsKeyboard::rept(unsigned char a1)
{
	return (this->KeyFlags[a1] & 1) != 0;
}

bool FFXI::Input::FsKeyboard::just(unsigned char a1)
{
	return (this->KeyFlags[a1] & 2) != 0;
}

bool FFXI::Input::FsKeyboard::CheckFlag4(unsigned char a1)
{
	return (this->KeyFlags[a1] & 4) != 0;
}

bool FFXI::Input::FsKeyboard::CheckFlag8(unsigned char a1)
{
	throw "NOT IMPLEMENTED";
}

bool FFXI::Input::FsKeyboard::CheckFlag10(unsigned char a1)
{
	throw "NOT IMPLEMENTED";
}

bool FFXI::Input::FsKeyboard::reptoff(unsigned char a1)
{
	if ((this->KeyFlags[a1] & 1) == 0)
		return false;

	this->KeyFlags[a1] &= ~1u;
	return true;
}

bool FFXI::Input::FsKeyboard::justoff(unsigned char a1)
{
	if ((this->KeyFlags[a1] & 2) == 0)
		return false;

	this->KeyFlags[a1] &= ~2u;
	return true;
}

void FFXI::Input::FsKeyboard::ClearFlags821(unsigned char a1)
{
	this->KeyFlags[a1] &= 0xF4u;
}

void FFXI::Input::FsKeyboard::ClearFlags(unsigned char a1)
{
	this->KeyFlags[a1] = 0;
}
