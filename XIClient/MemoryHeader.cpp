#include "MemoryHeader.h"

using namespace FFXI::CYy;

int MemoryHeader::GetSize() {
	MemoryHeader* NextEntry = this->NextEntry;
	if (NextEntry)
		return (int)NextEntry - (int)this - 0x20;
	else
		return 0;
}

int FFXI::CYy::MemoryHeader::GetFreeSize()
{
    int free = 0;
    MemoryHeader* tail = this;
    while (tail->PreviousEntry) {
        tail = tail->field_14;
        if (!tail->occupied)
            free += tail->GetSize();
    }
    return free;
}

MemoryHeader* FFXI::CYy::MemoryHeader::Delete()
{
    MemoryHeader* result; // eax
    MemoryHeader* PreviousEntry; // ecx
    MemoryHeader* NextEntry; // ecx
    MemoryHeader* v4; // edx
    MemoryHeader* v5; // ecx
    MemoryHeader* v6; // edx
    MemoryHeader* v7; // ecx

    result = this;
    if (!this->occupied)
        return result;
    PreviousEntry = this->PreviousEntry;
    this->occupied = 0;
    if (PreviousEntry && !PreviousEntry->occupied)
    {
        PreviousEntry->NextEntry = this->NextEntry;
        this->NextEntry->PreviousEntry = PreviousEntry;
        result = PreviousEntry;
    }
    NextEntry = result->NextEntry;
    if (!NextEntry->occupied)
    {
        result->NextEntry = NextEntry->NextEntry;
        NextEntry->NextEntry->PreviousEntry = result;
    }
    v4 = result->PreviousEntry;
    v5 = result;
    if (v4)
    {
        while (v4->occupied)
        {
            v5 = v4;
            v4 = v4->PreviousEntry;
            if (!v4)
                goto LABEL_12;
        }
        v5 = v5->PreviousEntry;
    }
LABEL_12:
    if (v5->PreviousEntry || result->PreviousEntry)
    {
        v5->field_10 = result;
        result->field_14 = v5;
    }
    v6 = result->NextEntry;
    v7 = result;
    if (v6)
    {
        while (v6->occupied)
        {
            v7 = v6;
            v6 = v6->NextEntry;
            if (!v6)
                goto LABEL_20;
        }
        v7 = v7->NextEntry;
    }
LABEL_20:
    if (!v7->NextEntry && !result->NextEntry)
        return result;
    v7->field_14 = result;
    result->field_10 = v7;
    return result;
}

