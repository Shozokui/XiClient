#include "FsFileQueue.h"
#include "FsFileEvent.h"
#include "FsSemaphore.h"
#include <new>
using namespace FFXI::File;

FsFileQueue::~FsFileQueue() {
	if (this->EventBuffer) delete[] this->EventBuffer;
	if (this->Semaphore) delete this->Semaphore;
}

FsFileQueue::FsFileQueue(int p_eventcount) {
	FsFileEvent* v5 = new FsFileEvent[p_eventcount];
	this->EventBuffer = v5;
	this->BufferSize = p_eventcount;
	this->Semaphore = new FsSemaphore(1, 1);
	this->BufferWritePos = 0;
	this->BufferReadPos = 0;
	this->FileCount = 0;
	this->field_18 = 0;
}

bool FFXI::File::FsFileQueue::hasFileEvent()
{
	this->Semaphore->Wait(INFINITE);
	if (this->FileCount <= 0) {
		this->Semaphore->Signal();
		return 0;
	}
	this->Semaphore->Signal();
	return 1;
}

bool FFXI::File::FsFileQueue::isInQueue(FsFileEvent* a2)
{
	if (this->FileCount == 0) return false;

	this->Semaphore->Wait(INFINITE);
	
	int v4 = this->BufferReadPos;

	while (v4 != this->BufferWritePos) {
		FsFileEvent* v7 = this->EventBuffer + v4;
		if (a2->Type == v7->Type) {
			if (strcmp(a2->FileName, v7->FileName) == 0) {
				if (a2->Buffer == nullptr
					|| a2->Buffer == v7->Buffer
					|| a2->BufferOffset == 0
					|| a2->BufferOffset == v7->BufferOffset) {
					break;
				}
			}
		}
		v4 += 1;
		if (v4 >= this->BufferSize)
			v4 = 0;
	}

	this->Semaphore->Signal();
	return v4 != this->BufferWritePos;
}

int FFXI::File::FsFileQueue::addFileEvent(FsFileEvent* p_event)
{
	this->Semaphore->Wait(INFINITE);
	if (this->FileCount + 1 >= this->BufferSize) {
		this->Semaphore->Signal();
		return -1;
	}
	FsFileEvent* v3 = &this->EventBuffer[this->BufferWritePos];
	*v3 = p_event;
	this->BufferWritePos += 1;
	if (this->BufferWritePos >= this->BufferSize)
		this->BufferWritePos = 0;
	this->FileCount += 1;
	this->Semaphore->Signal();
	return 0;
}

FsFileEvent* FFXI::File::FsFileQueue::startNextEvent(FsFileEvent* p_event)
{
	FsFileEvent v17{};
	this->Semaphore->Wait(INFINITE);
	if (this->FileCount > 0 && !this->field_18) {
		this->field_18 = 1;
		FsFileEvent* v7 = &this->EventBuffer[this->BufferReadPos];
		v17.Type = v7->Type;
		int pos = 0;
		do {
			v17.FileName[pos] = v7->FileName[pos];
		} while (v7->FileName[pos++]);
		v17.BufferOffset = v7->BufferOffset;
		v17.Buffer = v7->Buffer;
		v17.BufferSize = v7->BufferSize;
		v17.field_114 = v7->field_114;
		v17.SomeFunction = v7->SomeFunction;
		v17.FileNameJustRead = v7->FileNameJustRead;
	}
	this->Semaphore->Signal();
	p_event->Type = v17.Type;
	p_event->BufferOffset = v17.BufferOffset;
	memcpy(p_event->FileName, v17.FileName, sizeof(p_event->FileName));
	p_event->BufferSize = v17.BufferSize;
	p_event->Buffer = v17.Buffer;
	p_event->SomeFunction = v17.SomeFunction;
	p_event->field_114 = v17.field_114;
	p_event->CallBack = v17.CallBack;
	p_event->FileNameJustRead = v17.FileNameJustRead;
	return p_event;
}

int FFXI::File::FsFileQueue::endNextEvent()
{
	FsFileEvent* v3; // eax

	this->Semaphore->Wait(INFINITE);
	if (this->FileCount > 0 && this->field_18)
	{
		this->field_18 = 0;
		v3 = &this->EventBuffer[this->BufferReadPos];
		v3->Type = FsFileEvent::EventType::Null;
		v3->BufferOffset = 0;
		v3->Buffer = 0;
		v3->BufferSize = 0;
		v3->field_114 = 0;
		v3->SomeFunction = 0;
		v3->CallBack = 0;
		v3->FileNameJustRead = 0;
		this->BufferReadPos += 1;
		if (this->BufferReadPos >= this->BufferSize)
			this->BufferReadPos = 0;
		--this->FileCount;
		this->Semaphore->Signal();
		return 1;
	}
	else
	{
		this->Semaphore->Signal();
		return -1;
	}
	return 0;
}

int FFXI::File::FsFileQueue::cancelNextEvent()
{
	this->Semaphore->Wait(INFINITE);
	if (this->FileCount > 0 && this->field_18) {
		this->field_18 = 0;
		this->Semaphore->Signal();
		return 1;
	}
	this->Semaphore->Signal();
	return -1;
}
