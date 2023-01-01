#include "CMoAttachmentsSubStruct.h"
#include "CXiActor.h"
#include "XiAtelBuff.h"
using namespace FFXI::CYy;

FFXI::CYy::CMoAttachmentsSubStruct::CMoAttachmentsSubStruct()
{
	this->field_4 = 0;
	this->field_8 = 0x04000000;
}

FFXI::CYy::CMoAttachmentsSubStruct::~CMoAttachmentsSubStruct()
{
}

void FFXI::CYy::CMoAttachmentsSubStruct::SetValues(int a2, int a3)
{
	this->field_4 = a2;
	this->field_8 = a3;
}

void FFXI::CYy::CMoAttachmentsSubStruct::DoSomething(CXiActor* a2)
{
	if (a2 && a2->AtelBuffer != nullptr
		&& (a2->AtelBuffer->field_120 & 0x200) != 0) {
		this->field_4 = a2->AtelBuffer->GlobalBufferIndex;
		this->field_8 = a2->AtelBuffer->field_78;
	}
	else {
		this->field_8 = 0x04000000;
	}
}

CXiActor* FFXI::CYy::CMoAttachmentsSubStruct::GetActor()
{
	XiAtelBuff* buffer = XiAtelBuff::AtelBuffers[this->field_4];
	if (buffer == nullptr)
		return nullptr;

	if ((buffer->field_120 & 0x200) != 0
		&& buffer->field_78 == this->field_8)
		return buffer->Actor;

	return nullptr;
}

CXiActor* FFXI::CYy::CMoAttachmentsSubStruct::GetSearchActor()
{
	XiAtelBuff* buffer = XiAtelBuff::AtelBuffers[this->field_4];
	if (buffer == nullptr)
		return nullptr;

	if ((buffer->field_120 & 0x200) != 0
		&& buffer->field_78 == this->field_8)
		return buffer->Actor;

	for (int i = 0; i < sizeof(XiAtelBuff::AtelBuffers) / sizeof(XiAtelBuff::AtelBuffers[0]); ++i) {
		buffer = XiAtelBuff::AtelBuffers[i];
		if (buffer == nullptr)
			continue;

		if ((buffer->field_120 & 0x200) != 0) {
			if (buffer->field_78 != 0x4000000 && buffer->field_78 == this->field_8) {
				this->field_4 = buffer->GlobalBufferIndex;
				this->field_8 = buffer->field_78;
				return XiAtelBuff::AtelBuffers[this->field_4]->Actor;
			}
		}
	}

	return nullptr;
}

CXiActor* FFXI::CYy::CMoAttachmentsSubStruct::TWOGetSearchActor()
{
	XiAtelBuff* buffer = XiAtelBuff::AtelBuffers[this->field_4];
	if (buffer == nullptr)
		return nullptr;

	if ((this->field_8 & 0xFF000000) == 0x04000000)
		return nullptr;

	if ((buffer->field_120 & 0x200) != 0
		&& buffer->field_78 == this->field_8)
		return buffer->Actor;

	for (int i = 0; i < sizeof(XiAtelBuff::AtelBuffers) / sizeof(XiAtelBuff::AtelBuffers[0]); ++i) {
		buffer = XiAtelBuff::AtelBuffers[i];
		if (buffer == nullptr)
			continue;

		if ((buffer->field_120 & 0x200) != 0) {
			if (buffer->field_78 != 0x4000000 && buffer->field_78 == this->field_8) {
				this->field_4 = buffer->GlobalBufferIndex;
				this->field_8 = buffer->field_78;
				return XiAtelBuff::AtelBuffers[this->field_4]->Actor;
			}
		}
	}

	return nullptr;
}