#include "XiFileStat.h"
#include <string>
#include <winerror.h>
using namespace FFXI::File;

FFXI::File::XiFileStat::XiFileStat()
{
	this->FileSize = 0;
	memset(&this->LastAccess, 0, sizeof(this->LastAccess));
	memset(&this->LastModify, 0, sizeof(this->LastModify));
	memset(&this->CreationTime, 0, sizeof(this->CreationTime));
}

void FFXI::File::XiFileStat::setSize(off_t a2)
{
	this->FileSize = a2;
}

void FFXI::File::XiFileStat::setCreation(time_t a2)
{
	struct tm result {};
	if (localtime_s(&result, &a2) != S_OK) {
		this->CreationTime.Second = 0;
		this->CreationTime.Minute = 0;
		this->CreationTime.Hour = 0;
		this->CreationTime.Day = 1;
		this->CreationTime.Month = 1;
		this->CreationTime.Year = 1900;
	}
	else {
		this->CreationTime.Second = result.tm_sec;
		this->CreationTime.Minute = result.tm_min;
		this->CreationTime.Hour   = result.tm_hour;
		this->CreationTime.Day    = result.tm_mday;
		this->CreationTime.Month  = (result.tm_mon & 0xFF) + 1;
		this->CreationTime.Year   = (result.tm_year & 0xFFFF) + 1900;
	}
}

void FFXI::File::XiFileStat::setLastAccess(time_t a2)
{
	struct tm result {};
	if (localtime_s(&result, &a2) != S_OK) {
		this->LastAccess.Second = 0;
		this->LastAccess.Minute = 0;
		this->LastAccess.Hour = 0;
		this->LastAccess.Day = 1;
		this->LastAccess.Month = 1;
		this->LastAccess.Year = 1900;
	}
	else {
		this->LastAccess.Second = result.tm_sec;
		this->LastAccess.Minute = result.tm_min;
		this->LastAccess.Hour   = result.tm_hour;
		this->LastAccess.Day    = result.tm_mday;
		this->LastAccess.Month  = (result.tm_mon & 0xFF) + 1;
		this->LastAccess.Year   = (result.tm_year & 0xFFFF) + 1900;
	}
}

void FFXI::File::XiFileStat::setLastModify(time_t a2)
{
	struct tm result {};
	if (localtime_s(&result, &a2) != S_OK) {
		this->LastModify.Second = 0;
		this->LastModify.Minute = 0;
		this->LastModify.Hour = 0;
		this->LastModify.Day = 1;
		this->LastModify.Month = 1;
		this->LastModify.Year = 1900;
	}
	else {
		this->LastModify.Second = result.tm_sec;
		this->LastModify.Minute = result.tm_min;
		this->LastModify.Hour   = result.tm_hour;
		this->LastModify.Day    = result.tm_mday;
		this->LastModify.Month  = (result.tm_mon & 0xFF) + 1;
		this->LastModify.Year   = (result.tm_year & 0xFFFF) + 1900;
	}
}
