#include "DxGamma.h"
#include "Globals.h"
#include "RegistryConfig.h"
#include "CDx.h"
#include <iostream>
#include "RuntimeConfig.h"
using namespace FFXI::CYy;

DxGamma::~DxGamma() {


}

DxGamma::DxGamma()
{
	this->field_604 = 1.5;
	this->field_608 = 1.5;
	this->field_60C = 1.5;
	this->RedOffset = 1.0;
	this->GreenOffset = 1.0;
	this->BlueOffset = 1.0;
	this->Init();
}

int FillGammaRamp(float p_float, WORD* p_addr) {
	double v2; // st7
	int v3; // esi
	int64_t result; // rax
	int v5; // [esp+Ch] [ebp+4h]

	v2 = FFXI::Config::MainRegistryConfig::GammaBase + p_float;
	if (v2 < 0.0099999998)
		v2 = 0.0099999998;
	v3 = 0;
	v5 = 0;
	do
	{
		result = (int64_t)(pow((double)v5 * 0.0039215689, 1.0 / v2) * 65535.0);
		p_addr[v3++] = result;
		v5 = v3;
	} while (v3 < 256);
	return result;
}
int FFXI::CYy::DxGamma::Init()
{
	FillGammaRamp(this->RedOffset, this->GammaRamp.red);
	FillGammaRamp(this->GreenOffset, this->GammaRamp.green);
	return FillGammaRamp(this->BlueOffset, this->GammaRamp.blue);
}

void FFXI::CYy::DxGamma::SetGamma()
{
	char v2 = 0;
	if (this->RedOffset != this->field_604)
	{
		v2 = 1;
		this->RedOffset = this->field_604;
	}
	if (this->GreenOffset != this->field_608)
	{
		this->GreenOffset = this->field_608;
		v2 = 1;
	}
	if (this->BlueOffset == this->field_60C)
	{
		if (v2 != 1)
			return;
	}
	else
	{
		this->BlueOffset = this->field_60C;
	}
	this->Init();
	if (FFXI::CYy::CDx::instance && FFXI::CYy::CDx::instance->DXDevice) {
		if (FFXI::Config::RuntimeConfig::instance.window_mode == FFXI::Config::RuntimeConfig::WindowMode::Fullscreen)
			FFXI::CYy::CDx::instance->DXDevice->SetGammaRamp(D3DSGR_NO_CALIBRATION, &this->GammaRamp);
	}
}

void FFXI::CYy::DxGamma::GetTriplet(float* a2, float* a3, float* a4)
{
	*a2 = this->field_604;
	*a3 = this->field_608;
	*a4 = this->field_60C;

}

void FFXI::CYy::DxGamma::SetTriplet(float a2, float a3, float a4)
{
	this->field_604 = a2;
	this->field_608 = a3;
	this->field_60C = a4;
}

void FFXI::CYy::DxGamma::GetLightMultipliers(float* a2, float* a3, float* a4)
{
	*a2 = (double)this->GammaRamp.red[120] * 0.000035095749;
	*a3 = (double)this->GammaRamp.green[116] * 0.000035095749;
	*a4 = (double)this->GammaRamp.blue[128] * 0.000035095749;
}
