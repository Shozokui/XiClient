#include "CNtGame.h"
#include "Globals.h"
#include "CApp.h"
#include "NT_SYS.h"
using namespace FFXI::Network;

int CNtGame::ntGameGetCliMode()
{
	if (FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->pGame)
		return FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->pGame->CliMode;

	return 0;
}
