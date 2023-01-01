#include "CYyFileLoadTask.h"
#include "Globals.h"
#include "CYyDb.h"
#include "CMoResourceMng.h"
#include "XIFileManager.h"
#include "XiFileStat.h"
#include "CYyResourceFile.h"

using namespace FFXI::CYy;

void FFXI::CYy::CYyFileLoadTask::VObj2(int* a2)
{
	if (*a2 == 2)
		this->AbortFlag = 1;
}

void FFXI::CYy::CYyFileLoadTask::OptionalMove()
{
	if (this->State == 0) {
		if (this->AbortFlag == 0) {
			if (CMoResourceMng::ActiveFileLoadTaskCount > 16)
				return;

			CMoResourceMng::ActiveFileLoadTaskCount += 1;
			this->FileStat = new FFXI::File::XiFileStat();
			if (this->FileStat && FFXI::File::XIFileManager::g_pXIFileManager->GetNumFileStat(this->VTableIndex, this->FileStat) >= 0) {
				this->State = 1;
				return;
			}
		}
	}
	else if (this->State == 1) {
		if (FFXI::File::XIFileManager::g_pXIFileManager->isGetStat(this->VTableIndex) == true)
			return;
		if (this->AbortFlag == 0) {
			if (this->FileStat->FileSize > 0) {
				this->FileDataBuffer = new char[this->FileStat->FileSize];
				if (this->FileDataBuffer != nullptr) {
					if (FFXI::File::XIFileManager::g_pXIFileManager->readFile(this->VTableIndex, this->FileDataBuffer, this->FileStat->FileSize, 0) >= 0) {
						this->State = 2;
						return;
					}
				}
			}
		}
	}
	else if (this->State == 2) {
		if (FFXI::File::XIFileManager::g_pXIFileManager->isReadFile(this->VTableIndex, 0, 0) == true)
			return;
		if (this->AbortFlag == 0) {
			if (this->FileDataBuffer != nullptr) {
				CYyDb::g_pCYyDb->pCMoResourceMng->SetNumFileIndex(this->VTableIndex);
				CYyResourceFile** file{ nullptr };
				CYyDb::g_pCYyDb->pCMoResourceMng->Parse(this->FileDataBuffer, this->FileStat->FileSize, &file, (CMoResource**)this->ResourceFile);
				if (file && *file) {
					(*file)->PreHeader.field_16 |= 1u;
					(*file)->FileIndex = this->VTableIndex;
				}
				delete this;
				return;
			}
		}
	}

	if (this->ResourceFile) 
		CYyDb::g_pCYyDb->pCMoResourceMng->RemoveRes((CMoResource**)this->ResourceFile);
	
	delete this;
}

FFXI::CYy::CYyFileLoadTask::~CYyFileLoadTask()
{
	if (this->FileDataBuffer) {
		delete[] this->FileDataBuffer;
		this->FileDataBuffer = nullptr;
	}

	if (this->FileStat) {
		delete this->FileStat;
		this->FileStat = nullptr;
	}

	CMoResourceMng::ActiveFileLoadTaskCount -= 1;
}

void FFXI::CYy::CYyFileLoadTask::Init(int a2)
{
	this->AbortFlag = 0;
	this->State = 0;
	this->VTableIndex = a2;
	this->Param1 = a2;
	this->Param2 = 'Mrt\0';
	this->TaskMngShift4(0);
	void** resourceslot = CYyDb::g_pCYyDb->pCMoResourceMng->ReserveSlot();
	this->ResourceFile = (CYyResourceFile**)resourceslot;
}
