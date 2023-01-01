#include "FsFileEvent.h"
#include <algorithm>
using namespace FFXI::File;

FsFileEvent::FsFileEvent() {
	this->Type = FsFileEvent::EventType::Null;
	memset(this->FileName, 0, sizeof(this->FileName));
	this->BufferOffset = 0;
	this->Buffer = nullptr;
	this->BufferSize = 0;
	this->field_114 = 0;
	this->SomeFunction = 0;
	this->CallBack = 0;
	this->FileNameJustRead = nullptr;
}
FFXI::File::FsFileEvent::FsFileEvent(FsFileEvent* p_orig)
{
	this->Type = p_orig->Type;
	memcpy(this->FileName, p_orig->FileName, sizeof(this->FileName));
	this->BufferOffset = p_orig->BufferOffset;
	this->Buffer = p_orig->Buffer;
	this->BufferSize = p_orig->BufferSize;
	this->field_114 = p_orig->field_114;
	this->SomeFunction = p_orig->SomeFunction;
	this->CallBack = p_orig->CallBack;
	this->FileNameJustRead = p_orig->FileNameJustRead;
}