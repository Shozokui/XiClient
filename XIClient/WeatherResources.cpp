#include "WeatherResources.h"
#include "CYyResourceFile.h"
#include "XiZone.h"
#include "CMoKeyframe.h"
using namespace FFXI::CYy;

void FFXI::CYy::WeatherResources::LoadAll(XiZone* a2)
{
	this->Index = 0;
	memset(this->Resources, 0, sizeof(this->Resources));
	this->Load('enif', a2);
	this->Load('ynus', a2);
	this->Load('dolc', a2);
	this->Load('tsim', a2);
	this->Load('wyrd', a2);
	this->Load('taeh', a2);
	this->Load('niar', a2);
	this->Load('laqs', a2);
	this->Load('tsud', a2);
	this->Load('dnas', a2);
	this->Load('dniw', a2);
	this->Load('mots', a2);
	this->Load('wons', a2);
	this->Load('zilb', a2);
	this->Load('rdht', a2);
	this->Load('tlob', a2);
	this->Load('arua', a2);
	this->Load('tgil', a2);
	this->Load('dgof', a2);
	this->Load('krad', a2);
}

void FFXI::CYy::WeatherResources::Load(int a2, XiZone* a3)
{
	if (!a3) return;
	if (!a3->field_1AC) return;
	if (this->Index >= sizeof(this->Resources) / sizeof(this->Resources[0])) return;

	WeatherResource* res = this->Resources + this->Index;
	memset(res, 0, sizeof(*res));
	CYyResourceFile** file{ nullptr };
	a3->field_1AC->FindResourceUnder((CMoResource***)&file, FFXI::Constants::Enums::ResourceType::Rmp, 'taew');
	if (file) {
		if (*file) {
			(*file)->FindResourceUnder((CMoResource***)&file, FFXI::Constants::Enums::ResourceType::Rmp, a2);
			if (file && *file) {
				(*file)->AnotherResourceSearcher((CMoResource***)&file, FFXI::Constants::Enums::ResourceType::Rmp, 'mrfk');
				if (file)
					res->field_4 = *file;
				else
					res->field_4 = nullptr;
			}
		}
	}

	if (!res->field_4) {
		a3->field_1AC->FindResourceUnder((CMoResource***)&file, FFXI::Constants::Enums::ResourceType::Rmp, 'edom');
		if (!file) return;
		(*file)->AnotherResourceSearcher((CMoResource***)&file, FFXI::Constants::Enums::ResourceType::Rmp, 'mrfk');
		if (file)
			res->field_4 = *file;
		else
			res->field_4 = nullptr;
	}

	CMoResource* v11 = res->field_4;
	while (v11){
		CMoKeyframe** keyframe{ nullptr };
		v11->AnotherResourceSearcher((CMoResource***)&keyframe, FFXI::Constants::Enums::ResourceType::Keyframe, 0);
		if (!keyframe || !*keyframe)
			break;
		res->Keyframes[res->KeyframeCount] = (CMoKeyframe**)(*keyframe)->PreHeader.PointerToThisInRPL;
		res->KeyframeCount += 1;
		if (res->KeyframeCount >= sizeof(res->Keyframes) / sizeof(res->Keyframes[0]))
			break;
		if ((*keyframe)->PreHeader.NextRPL)
			v11 = *(*keyframe)->PreHeader.NextRPL;
		else
			v11 = nullptr;
	}
	
	if (res->KeyframeCount) {
		res->FourCC = a2;
		++this->Index;
	}
}

CMoKeyframe** FFXI::CYy::WeatherResources::GetRandKeyframe(int a2)
{
	if (this->Index <= 0) return nullptr;
	for (int i = 0; i < this->Index; ++i) {
		WeatherResource* wr = this->Resources + i;
		if (wr->FourCC == a2) {
			if (wr->KeyframeCount) {
				int v7 = rand() % wr->KeyframeCount;
				return wr->Keyframes[v7];
			}
			else
				return nullptr;
		}
	}
	return nullptr;
}
