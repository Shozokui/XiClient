#include "XIFileImpl.h"
#include "FsFileEvent.h"
#include "XIFileManager.h"
#include "FsFileThreadManager.h"
#include "Globals.h"
#include "CApp.h"
#include "NT_SYS.h"
#include "CYyDb.h"
#include "Values.h"
#include <algorithm>
#include <iostream>
using namespace FFXI::File;

XIFileImpl::~XIFileImpl() {
    if (this->ThreadManager) {
        delete this->ThreadManager;
        this->ThreadManager = nullptr;
    }

    for (int i = 0; i < this->MaxROMIndexLoaded; ++i) {
        if (SomeArray1[i]) {
            delete SomeArray1[i];
            SomeArray1[i] = nullptr;
        }
        SomeArray2[i] = 0;
    }
    this->field_44 = 0;
    this->MaxROMIndexLoaded = 0;
    if (this->VTableData) {
        delete[] this->VTableData;
        this->VTableData = nullptr;
        this->VTableSize = 0;
    }
    if (this->FTableData) {
        delete[] this->FTableData;
        this->FTableData = nullptr;
        this->FTableSize = 0;
        this->TableEntryCount = 0;
    }
}
XIFileImpl::XIFileImpl(int p_FsMode, int p_DiskMode) {
    if (!p_FsMode)
        goto LABEL_6;
    if (p_FsMode == 1)
    {
        this->FsMode = 1;
        goto LABEL_7;
    }
    if (p_FsMode != 2)
    {
    LABEL_6:
        this->FsMode = 0;
        goto LABEL_7;
    }
    this->FsMode = 2;
LABEL_7:
    this->DiskMode = 3;
    for (int i = 0; i < sizeof(this->SomeArray1) / sizeof(this->SomeArray1[0]); ++i) {
        this->SomeArray1[i] = nullptr;
        this->SomeArray2[i] = 0;
        this->SomeArray3[i] = 0;
    }
    this->field_44 = 0;
    this->MaxROMIndexLoaded = 0;
    this->VTableData = nullptr;
    this->VTableSize = 0;
    this->FTableData = nullptr;
    this->FTableSize = 0;
    this->TableEntryCount = 0;
    this->ThreadManager = 0;
    memset(this->UserPath, 0, sizeof(this->UserPath));
    memset(this->TempPath, 0, sizeof(this->TempPath));
    memset(this->SystemPath, 0, sizeof(this->SystemPath));
    memset(this->ffxiuserPath, 0, sizeof(this->ffxiuserPath));
}

int FFXI::File::XIFileImpl::Init(LPCSTR p_path, int p_len)
{
    this->ThreadManager = new FsFileThreadManager();
    if (this->ThreadManager == nullptr || this->ThreadManager->Init(p_len) < 0) {
        return -1;
    }

    if (this->FsMode == 1) {
        sprintf_s(this->CurrentDirectory, "%s", p_path);
        strcpy_s(&this->field_1D0, 6, "pfs1:\0");
        if (this->readNumFile() < 0) return -1;
        strcpy_s(this->UserPath, sizeof(this->UserPath), "USER\0");
        strcpy_s(this->TempPath, sizeof(this->TempPath), "TEMP\0");
        strcpy_s(this->SystemPath, sizeof(this->SystemPath), "SYS\0");
        return 1;
    }
    else {
        throw "Not supported";
    }
    return -1;
}

int FFXI::File::XIFileImpl::ReadFilenameNow(const char* p_filename, char* p_buffer, int p_buflen, int p_bufoffset)
{
    FsFileEvent FsEvent{};
    FsEvent.Type = FsFileEvent::EventType::Read;
    this->strcpy(FsEvent.FileName, sizeof(FsEvent.FileName), p_filename);
    FsEvent.BufferOffset = p_bufoffset;
    FsEvent.BufferSize = p_buflen;
    FsEvent.Buffer = p_buffer;
    return this->ThreadManager->DoEventNow(&FsEvent);
}

int FFXI::File::XIFileImpl::ReadNumfileNow(int p_index, char* p_buffer, int p_buflen, int p_bufoffset)
{
    char filename[256];
    if (this->GetFileName(p_index, filename, sizeof(filename)) < 0) return -1;
    FsFileEvent v8{};
    v8.Type = FsFileEvent::EventType::Read;
    this->GetFullName(v8.FileName, sizeof(v8.FileName), filename);
    v8.BufferOffset = p_bufoffset;
    v8.Buffer = p_buffer;
    v8.BufferSize = p_buflen;
    return this->ThreadManager->DoEventNow(&v8);
}

int FFXI::File::XIFileImpl::readUserFileNow(const char* a2, char* a3, int a4, int a5, int a6)
{
    char filename[0x100];
    if (this->getUserFileName(a2, filename, sizeof(filename), a6) < 0)
        return -1;

    FsFileEvent v2{};
    v2.Type = FsFileEvent::EventType::Read;
    this->GetFullName(v2.FileName, sizeof(v2.FileName), filename);
    v2.BufferOffset = a5;
    v2.BufferSize = a4;
    v2.Buffer = a3;
    return this->ThreadManager->DoEventNow(&v2);
}

int FFXI::File::XIFileImpl::getUserFileName(const char* a2, char* a3, int a3size, int a4)
{
    if (FFXI::Config::RegistryConfig::g_pOptions->Other.field_24 == 0
        || CYyDb::g_pCYyDb->field_9 == 0) {
        return -1;
    }
    if (a4 != 0) {
        sprintf_s(a3, a3size, "%s/%x/%s", this->UserPath, a4, a2);
        return 1;
    }
    if (FFXI::GlobalStruct::g_GlobalStruct.field_0 != 64
        && FFXI::GlobalStruct::g_GlobalStruct.field_0 != 0) {
        int id = FFXI::Network::NT_SYS::getFFXIID(FFXI::CYy::CApp::g_pCApp->g_pNT_SYS->field_80);
        sprintf_s(a3, a3size, "%s/%x/%s", this->UserPath, id, a2);
        return 1;
    }
    sprintf_s(a3, a3size, "%s/%s", this->UserPath, a2);
    return 1;
}

int FFXI::File::XIFileImpl::readFile(int a2, char* a3, int a4, int a5)
{
    char buff[256];
    if (this->GetFileName(a2, buff, sizeof(buff)) < 0)
        return -1;

    FsFileEvent v8{};
    this->GetFullName(v8.FileName, sizeof(v8.FileName), buff);
    
    v8.Type = FsFileEvent::EventType::Read;

    v8.Buffer = a3;
    v8.BufferSize = a4;
    v8.BufferOffset = a5;

    return 2 * (this->ThreadManager->addFileEvent(&v8) >= 0) - 1;
}

int FFXI::File::XIFileImpl::readNumFile()
{
    FsFileEvent FsEvent{};

    this->FTableSize = this->GetFileSizeByName("/FTABLE.DAT\0");
    if (this->FTableSize < 0) return -1;

    uint32_t v4 = (long)this->FTableSize + 1L;
    v4 &= 0xFFFFFFFE;

    char* memory{ nullptr };
    XIFileManager::GetMem(&memory, v4);
    this->FTableData = reinterpret_cast<uint16_t*>(memory);
    FsEvent.Type = FsFileEvent::EventType::Read;
    this->GetFullName(FsEvent.FileName, sizeof(FsEvent.FileName), "/FTABLE.DAT\0");
    FsEvent.Buffer = memory;
    FsEvent.BufferSize = this->FTableSize;
    if (this->ThreadManager->DoEventNow(&FsEvent) < 0) {
        delete[] this->FTableData;
        this->FTableData = nullptr;
        return -1;
    }

    this->TableEntryCount = this->FTableSize >> 1;
    this->VTableSize = this->GetFileSizeByName("/VTABLE.DAT\0");

    if (this->VTableSize >= 0) {
        XIFileManager::GetMem(&this->VTableData, this->VTableSize);
        this->GetFullName(FsEvent.FileName, sizeof(FsEvent.FileName), "/VTABLE.DAT\0");
        FsEvent.Buffer = this->VTableData;
        FsEvent.BufferSize = this->VTableSize;
        if (this->ThreadManager->DoEventNow(&FsEvent) < 0)
            memset(this->VTableData, 1, this->VTableSize);
    }
    else {
        this->VTableSize = this->TableEntryCount;
        XIFileManager::GetMem(&this->VTableData, this->VTableSize);
        memset(this->VTableData, 1, this->VTableSize);
    }

    char* VTableBuffer{ nullptr }, * FTableBuffer{ nullptr };
    XIFileManager::GetMem(&FTableBuffer, v4);
    XIFileManager::GetMem(&VTableBuffer, this->VTableSize);

    char a4[256] = { 0 };

    for (this->MaxROMIndexLoaded = 2; this->MaxROMIndexLoaded <= Constants::Values::INDEX_ROM_MAX; ++this->MaxROMIndexLoaded) {
        sprintf_s(a4, sizeof(a4), "/ROM%d/FTABLE%d.DAT\0", this->MaxROMIndexLoaded, this->MaxROMIndexLoaded);
        if (this->GetFileSizeByName(a4) >= 0) {

            if (this->MaxROMIndexLoaded < 9) {
                unsigned int v13 = this->MaxROMIndexLoaded - 6;
                if (v13 > 5 || (FFXI::GlobalStruct::g_GlobalStruct.ClientExpansions & 2) != 0) {
                    int shifter = (this->MaxROMIndexLoaded - 1) & 0x1F;
                    FFXI::GlobalStruct::g_GlobalStruct.ClientExpansions |= 1 << shifter;
                }
            }
            else {
                int shifter = (this->MaxROMIndexLoaded + 2) & 0x1F;
                FFXI::GlobalStruct::g_GlobalStruct.ClientExpansions |= 1 << shifter;
            }
            this->GetFullName(FsEvent.FileName, sizeof(FsEvent.FileName), a4);
            FsEvent.Buffer = FTableBuffer;
            FsEvent.BufferSize = this->FTableSize;
            int ftableresult = this->ThreadManager->DoEventNow(&FsEvent);

            sprintf_s(a4, sizeof(a4), "/ROM%d/VTABLE%d.DAT\0", this->MaxROMIndexLoaded, this->MaxROMIndexLoaded);
            this->GetFullName(FsEvent.FileName, sizeof(FsEvent.FileName), a4);
            FsEvent.Buffer = VTableBuffer;
            FsEvent.BufferSize = this->VTableSize;
            int vtableresult = this->ThreadManager->DoEventNow(&FsEvent);

            if (ftableresult < 0 || vtableresult != this->VTableSize) {
                delete[] this->FTableData;
                delete[] this->VTableData;
                this->FTableData = nullptr;
                this->VTableData = nullptr;
                delete[] FTableBuffer;
                delete[] VTableBuffer;
                return -1;
            }

            for (int i = 0; i < this->TableEntryCount; ++i) {
                if (VTableBuffer[i] == this->MaxROMIndexLoaded) {
                    this->VTableData[i] = this->MaxROMIndexLoaded;
                    this->FTableData[i] = ((unsigned short*)FTableBuffer)[i];
                }
            }
        }
    }

    delete[] FTableBuffer;
    delete[] VTableBuffer;
    int v26 = FFXI::GlobalStruct::g_GlobalStruct.ClientExpansions;
    if ((v26 & 2) != 0 && (v26 & 0x10) != 0)
        FFXI::GlobalStruct::g_GlobalStruct.ClientExpansions |= 0x700;
    return 1;
}

bool FFXI::File::XIFileImpl::isReadFile(int a2, char* a3, int a4)
{
    char buff[256];
    if (this->GetFileName(a2, buff, sizeof(buff)) < 0)
        return false;

    FsFileEvent v6{};
    this->GetFullName(v6.FileName, sizeof(v6.FileName), buff);

    v6.Type = FsFileEvent::EventType::Read;
    v6.Buffer = a3;
    v6.BufferOffset = a4;
    return this->ThreadManager->checkFileEvent(&v6);
}

int FFXI::File::XIFileImpl::GetFileSizeByIndex(int p_index)
{
    char filename[256];
    if (this->GetFileName(p_index, filename, sizeof(filename)) < 0) return -1;

    FsFileEvent a2{};
    a2.Type = FsFileEvent::EventType::GetFileSize;
    this->GetFullName(a2.FileName, sizeof(a2.FileName), filename);
    return this->ThreadManager->DoEventNow(&a2);
}

int FFXI::File::XIFileImpl::GetFileSizeByName(LPCSTR p_name)
{
    FsFileEvent FsEvent{};
    FsEvent.Type = FsFileEvent::EventType::GetFileSize;
    this->GetFullName(FsEvent.FileName, 256, p_name);
    return this->ThreadManager->DoEventNow(&FsEvent);
}

int FFXI::File::XIFileImpl::GetFileName(int p_index, char* p_buff, int p_len)
{
    if (this->FsMode == 1) {
        if (p_index >= this->TableEntryCount) return -1;
        int v8 = this->FTableData[p_index];
        int v9 = this->VTableData[p_index];
        if (!v9) return -1;
        if (v9 == 1)
            sprintf_s(p_buff, p_len, "/ROM/%d/%d.DAT", v8 >> 7, v8 & 0x7F);
        else
            sprintf_s(p_buff, p_len, "/ROM%d/%d/%d.DAT", v9, v8 >> 7, v8 & 0x7F);
        return 1;
    }
    else {
        throw "FsMode not handled XIFileImpl::GetFileName";
    }
    return 0;
}

int FFXI::File::XIFileImpl::GetFullName(char* p_buffer, int p_bufLen, LPCSTR p_shortName)
{
    if (this->DiskMode == 3)
    {
        if (strlen(p_shortName) + strlen(this->CurrentDirectory) >= p_bufLen)
            return -1;
        if (!*this->CurrentDirectory) {
            sprintf_s(p_buffer, p_bufLen, "%s", (const char*)p_shortName);
            return 1;
        }
        sprintf_s(p_buffer, p_bufLen, "%s/%s", this->CurrentDirectory, (const char*)p_shortName);
    }
    else if (this->DiskMode == 4)
    {
        sprintf_s(p_buffer, p_bufLen, "%s", (const char*)p_shortName);
        return 1;
    }
    return 1;
}

int FFXI::File::XIFileImpl::GetNumFileStat(int a2, XiFileStat* a3)
{
    FsFileEvent v5{};

    char buff[256];
    if (this->GetFileName(a2, buff, sizeof(buff)) < 0)
        return -1;

    v5.Type = FFXI::File::FsFileEvent::EventType::GetStats;
    this->GetFullName(v5.FileName, sizeof(v5.FileName), buff);
    v5.Buffer = a3;
    return 2 * (this->ThreadManager->addFileEvent(&v5) >= 0) - 1;
}

bool FFXI::File::XIFileImpl::isGetStat(int a2)
{
    FsFileEvent v4{};

    char buff[256];
    if (this->GetFileName(a2, buff, sizeof(buff)) < 0)
        return false;

    v4.Type = FFXI::File::FsFileEvent::EventType::GetStats;
    this->GetFullName(v4.FileName, sizeof(v4.FileName), buff);
    return this->ThreadManager->checkFileEvent(&v4);
}

int FFXI::File::XIFileImpl::strcpy(char* p_dst, int p_dstlen, const char* p_src)
{
    if (this->DiskMode == 3) {
        if (strlen(p_src) < p_dstlen) {
            sprintf_s(p_dst, p_dstlen, "%s", p_src);
            return 1;
        }
        return -1;
    }
    else if (this->DiskMode == 4) {
        sprintf_s(p_dst, p_dstlen, "%s", p_src);
    }
    return 1;
}
