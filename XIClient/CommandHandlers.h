#pragma once
namespace FFXI {
	extern const unsigned int CommandHandlerCount;
	void (__cdecl * CommandHandlers[])(unsigned int*);
}