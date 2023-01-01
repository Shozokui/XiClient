#include "FsFileThreadManager.h"
#include "XIFileBase.h"
#include <iostream>
using namespace FFXI::File;


FsFileThreadManager::~FsFileThreadManager() {
    if (this->FsThread) {
        FsFileEvent a2{};
        a2.Type = FsFileEvent::EventType::Exit;
        this->addFileEvent(&a2);
        while (this->ThreadSync->FileQueue->hasFileEvent())
        {
            this->ThreadSync->fileAwake();
            if (this->ThreadSync->mainSleep(1000) < 0) return;
        }
        delete this->FsThread;
        this->FsThread = nullptr;
    }
    if (this->ThreadSync) {
        delete this->ThreadSync;
        this->ThreadSync = nullptr;
    }
}

FsFileThreadManager::FsFileThreadManager() {
    this->FsThread = nullptr;
    this->ThreadSync = nullptr;
}

int FsFileThreadManager::Init(int p_eventCount) {
	this->ThreadSync = new FsFileThreadSync(p_eventCount);
	if (!this->ThreadSync) return -1;
    this->FsThread = new FFXI::File::FsThread(FsFileThreadManager::threadControlFunction, this->ThreadSync);
	if (!this->FsThread) return -1;
    int result = this->FsThread->Start();
    return 2 * (result >= 0) - 1;

}

int FFXI::File::FsFileThreadManager::DoEventNow(FsFileEvent* p_event)
{
    if (this->ThreadSync->Pause() < 0) return -1;
    FsFileEvent FsEvent(p_event);
    int v4 = this->HandleFileEvent(FsEvent);
    if (v4 >= 0) {
        if (this->ThreadSync->Resume() >= 0) return v4;
        return -1;
    }
    this->ThreadSync->Resume();
    return -1;
}

bool FFXI::File::FsFileThreadManager::checkFileEvent(FsFileEvent* a2)
{
    return this->ThreadSync->FileQueue->isInQueue(a2);
}

int FFXI::File::FsFileThreadManager::addFileEvent(FsFileEvent* p_event)
{
    int v3 = 0;
    int v4 = this->ThreadSync->FileQueue->addFileEvent(p_event);
    if (v4 < 0) {
        while (this->ThreadSync->fileAwake() >= 0) {
            if (this->ThreadSync->mainSleep(1000u) < 0) return -1;
            v3 += 1000;
            if (v3 >= 100000) return -1;
            v4 = this->ThreadSync->FileQueue->addFileEvent(p_event);
            if (v4 >= 0) break;
        }
    }
    if (this->ThreadSync->fileAwake() < 0) return -1;
    return v4;
}

int FFXI::File::FsFileThreadManager::HandleFileEvent(FsFileEvent p_event)
{
    FsFileEvent v8(&p_event);
    int OperationResult{ -1 };
    switch (p_event.Type) {
    case FsFileEvent::EventType::Read:
        OperationResult = FsFileThreadManager::DoRead(v8);
        if (OperationResult >= 0)
            return OperationResult;
        break;
    case FsFileEvent::EventType::GetFileSize:
        OperationResult = FsFileThreadManager::DoGetFileSize(v8);
        if (OperationResult >= 0) 
            return OperationResult;
        break;
    case FsFileEvent::EventType::GetStats:
        OperationResult = FsFileThreadManager::DoGetStat(v8);
        if (OperationResult >= 0)
            return OperationResult;
        break;
    case FsFileEvent::EventType::Exit:
        FsThread::ExitSelf();
        break;
    default:
        break;
    }
    return -1;
}

int FFXI::File::FsFileThreadManager::DoRead(FsFileEvent p_event)
{
    FILE* file{ nullptr };
    if (p_event.Type != FsFileEvent::EventType::Read) return -1;
    if (XIFileBase::Open(&file, p_event.FileName, XIFileBase::XI_OPENMODE::Read) < 0) return -1;
    if (p_event.BufferOffset > 0 && XIFileBase::Seek(file, p_event.BufferOffset, XIFileBase::XI_SEEKMODE::Set) < 0) {
        XIFileBase::Close(file);
        return -1;
    }
    if (p_event.BufferSize == 0x1400000) {
        off_t FileSize = XIFileBase::GetFileSize(p_event.FileName);
        if (FileSize < p_event.BufferSize)
            p_event.BufferSize = FileSize;
    }
    int v4 = XIFileBase::Read(file, (char*)p_event.Buffer, p_event.BufferSize);
    if (v4 < 0) {
        XIFileBase::Close(file);
        return -1;
    }
    else if (XIFileBase::Close(file) >= 0) {
        if (p_event.CallBack) p_event.CallBack((char*)p_event.Buffer, v4, p_event.FileNameJustRead);
        return v4;
    }
    return -1;
}

int FFXI::File::FsFileThreadManager::DoGetFileSize(FsFileEvent p_event)
{
    if (p_event.Type != FsFileEvent::EventType::GetFileSize) return -1;
    return XIFileBase::GetFileSize(p_event.FileName);
}

int FFXI::File::FsFileThreadManager::DoGetStat(FsFileEvent a1)
{
    return XIFileBase::GetFileState(a1.FileName, (XiFileStat*)a1.Buffer);
}

DWORD CALLBACK FsFileThreadManager::threadControlFunction(LPVOID param)
{
    FsFileThreadSync* ThreadSync = static_cast<FsFileThreadSync*>(param);
    FsFileEvent* v2{ nullptr };
    FsFileEvent a2{};
    //EventStore
    FsFileEvent::EventType Type{ FsFileEvent::EventType::Null };
    char* FileName{ nullptr };
    char* Buffer{ nullptr };
    char* FileNameJustRead{ nullptr };
    int BufferOffset{ 0 };
    int BufferSize{ 0 };
    int v10{ 0 };
    int SomeFunction{ 0 };
    void(__cdecl * CallBack)(char*, int, char*) { nullptr };
    //~EventStore
    int v1{ 0 }, pos{ 0 };
    char v15[256];
    memset(v15, 0, sizeof(v15));
    GetCurrentThread();
    while (true) {
        while (true) {
            while (!ThreadSync->FileQueue->hasFileEvent())
                ThreadSync->fileSleep(1000u);
            v2 = ThreadSync->FileQueue->startNextEvent(&a2);
            Type = v2->Type;
            FileName = v2->FileName;
            pos = 0;
            do {
                v15[pos] = v2->FileName[pos];
            } while (v2->FileName[pos++]);
            Buffer = (char*)v2->Buffer;
            CallBack = v2->CallBack;
            FileNameJustRead = v2->FileNameJustRead;
            BufferOffset = v2->BufferOffset;
            BufferSize = v2->BufferSize;
            v10 = v2->field_114;
            SomeFunction = v2->SomeFunction;
            if (v2->Type == FsFileEvent::EventType::Exit) {
                ThreadSync->FileQueue->endNextEvent();
                ThreadSync->End();
                FsThread::ExitSelf();
            }
            if (v2->Type != FsFileEvent::EventType::Wait)
                break;
            LABEL_16:
            if (v1 == -3)
                ThreadSync->FileQueue->cancelNextEvent();
            else
                ThreadSync->FileQueue->endNextEvent();
            ThreadSync->mainAwake();
            Sleep(0);
        }
        if (v2->Type != FsFileEvent::EventType::Dunno) {
            ThreadSync->ThreadSemaWait();
            FsFileEvent v13{};
            v13.Type = Type;
            v13.Buffer = Buffer;
            std::memcpy(v13.FileName, FileName, sizeof(v13.FileName));
            v13.BufferOffset = BufferOffset;
            v13.BufferSize = BufferSize;
            v13.field_114 = v10;
            v13.SomeFunction = SomeFunction;
            v13.CallBack = CallBack;
            v13.FileNameJustRead = FileNameJustRead;
            int v12 = FsFileThreadManager::HandleFileEvent(v13);
            v1 = v12;
            if (v12 < 0 && v12 == -2)
                ThreadSync->ErrorQueue->AddError(2003);
            ThreadSync->ThreadSemaSignal();
            goto LABEL_16;
        }
        if (v2->CallBack)
            v2->CallBack(nullptr, -1, v2->FileNameJustRead);
        v1 = 0;
        ThreadSync->FileQueue->endNextEvent();
        ThreadSync->mainAwake();
        Sleep(0);
    }
    return 1;
}
