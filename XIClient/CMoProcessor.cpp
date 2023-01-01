#include "CMoProcessor.h"
using namespace FFXI::CYy;

const CYyClass CMoProcessor::CMoProcessorClass{
	"CMoProcessor", sizeof(CMoProcessor), &CYyObject::CYyObjectClass
};

const CYyClass* FFXI::CYy::CMoProcessor::GetRuntimeClass()
{
	return &CMoProcessorClass;
}
