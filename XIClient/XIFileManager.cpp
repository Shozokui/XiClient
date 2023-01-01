#include "XIFileManager.h"
#include <iostream>
using namespace FFXI::File;

XIFileManager* FFXI::File::XIFileManager::g_pXIFileManager{ nullptr };

XIFileManager::~XIFileManager() {
	if (this->FileImpl) {
		delete this->FileImpl;
		this->FileImpl = nullptr;
	}
}
XIFileManager::XIFileManager(int p_FsMode, int p_DiskMode) {
	this->FsMode = p_FsMode;
	this->DiskMode = p_DiskMode;
	this->FileImpl = nullptr;
}
int XIFileManager::fileSystemInit(LPCSTR p_dir, int p_dirLen) {
	//Clear the version string
	this->FFXIVersionString[0] = 0;

	this->FileImpl = new XIFileImpl(this->FsMode, this->DiskMode);
	if (this->FileImpl == nullptr) {
		return -1;
	}

	int v7 = this->FileImpl->Init(p_dir, p_dirLen);
	if (v7 != 1) {
		return v7;
	}
	
	char v9[292];
	if (this->FileImpl->ReadFilenameNow("patch.ver", v9, 288, 0) < 0) return -1;
	char v10[288];
	memcpy(v10, v9, sizeof(v10));
	//decrypt patch
	//TODO
	this->FFXIVersionString[0] = 0;
	return v7;
}

void XIFileManager::initffxiuserPath(LPCSTR p_string) {
	XIFileImpl* FileImpl = this->FileImpl;
	int buf_len = sizeof FileImpl->ffxiuserPath;
	strcpy_s(FileImpl->ffxiuserPath, p_string);
	FileImpl->ffxiuserPath[buf_len - 1] = 0;
	strncat_s(FileImpl->ffxiuserPath, buf_len, "\\ffxiuser", buf_len);
}

int FFXI::File::XIFileManager::ReadFilenameNow(char* p_filename, char* p_buffer, int p_buflen, int p_bufoffset)
{
	return this->FileImpl->ReadFilenameNow(p_filename, p_buffer, p_buflen, p_bufoffset);
}

int FFXI::File::XIFileManager::ReadNumfileNow(int index, char* buffer, int buflen, int bufoffset)
{
	int result = this->FileImpl->ReadNumfileNow(index, buffer, buflen, bufoffset);
	if (result >= 0) return result;
	//TODO //sub XiErr
	exit(2003);
}

int FFXI::File::XIFileManager::readUserFileNow(const char* a2, char* a3, int a4, int a5, int a6)
{
	return this->FileImpl->readUserFileNow(a2, a3, a4, a5, a6);
}

int FFXI::File::XIFileManager::readFile(int a2, char* a3, int a4, int a5)
{
	return this->FileImpl->readFile(a2, a3, a4, a5);
}

bool FFXI::File::XIFileManager::isReadFile(int a2, char* a3, int a4)
{
	return this->FileImpl->isReadFile(a2, a3, a4);
}

int FFXI::File::XIFileManager::GetNumFileStat(int a2, XiFileStat* a3)
{
	return this->FileImpl->GetNumFileStat(a2, a3);
}

bool FFXI::File::XIFileManager::isGetStat(int a2)
{
	return this->FileImpl->isGetStat(a2);
}

int FFXI::File::XIFileManager::GetFileSizeByNumfile(int p_index)
{
	int result = this->FileImpl->GetFileSizeByIndex(p_index);
	if (result >= 0) return result;
	//TODO //sub XiErr
	exit(2003);
}

bool FFXI::File::XIFileManager::fsInitFileSystem(int type, LPCSTR ss_path)
{
	switch (type) {
	case 1:
	case 2:
		try {
			FFXI::File::XIFileManager::g_pXIFileManager = new File::XIFileManager(1, 0);

			uint32_t CurrentDirectoryNameLen = GetCurrentDirectoryA(0, 0);
			char* CurrentDirectory = new char[CurrentDirectoryNameLen + 1];
			GetCurrentDirectoryA(CurrentDirectoryNameLen, CurrentDirectory);

			bool init_result = FFXI::File::XIFileManager::g_pXIFileManager->fileSystemInit(CurrentDirectory, 256) >= 0;
			if (CurrentDirectory != nullptr) {
				delete[] CurrentDirectory;
			}

			if (init_result == true) {
				FFXI::File::XIFileManager::g_pXIFileManager->initffxiuserPath(ss_path);
				return true;
			}
		}
		catch (...) {}
		break;
	default:
		break;
	}

	return false;
}

void FFXI::File::XIFileManager::fsUninitFileSystem()
{
	if (FFXI::File::XIFileManager::g_pXIFileManager != nullptr) {
		if (FFXI::File::XIFileManager::g_pXIFileManager->FileImpl) {
			delete FFXI::File::XIFileManager::g_pXIFileManager->FileImpl;
			FFXI::File::XIFileManager::g_pXIFileManager->FileImpl = nullptr;
		}

		delete FFXI::File::XIFileManager::g_pXIFileManager;
		FFXI::File::XIFileManager::g_pXIFileManager = nullptr;
	}
}

void XIFileManager::GetMem(char** p_buffer, uint32_t p_size)
{
	*p_buffer = new char[p_size];
	if (*p_buffer)
		memset(*p_buffer, 0, p_size);
}

char* XIFileManager::GetVersionString() {
	return this->FFXIVersionString[0] != '\0' ? this->FFXIVersionString : nullptr;
}