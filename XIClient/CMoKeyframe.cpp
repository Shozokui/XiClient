#include "CMoKeyframe.h"
using namespace FFXI::CYy;

void FFXI::CYy::CMoKeyframe::Open()
{
	//nullsub
}

double FFXI::CYy::CMoKeyframe::GetFrameValue(float a2)
{
	//Clip a2 between 0.0-1.0
	if (a2 > 1.0)
		a2 = 1.0;
	else if (a2 < 0.0)
		a2 = 0.0;

	float* v6 = &this->field_30;
	float* v7 = &this->field_38;
	while (v7[0] < a2) {
		v6 = v7;
		if (v7[0] >= 1.0)
			v7 = nullptr; //did they do this to catch mistakes in their data?
		else
			v7 += 2;
	}
	if (v6[0] == v7[0])
		return v7[1];

	return (a2 - *v6) / (*v7 - *v6) * (v7[1] - v6[1]) + v6[1];
}

double FFXI::CYy::CMoKeyframe::GetFrameValue(float a2, float a3)
{
	float backup = this->field_34;
	this->field_34 = a3;
	float result = this->GetFrameValue(a2);
	this->field_34 = backup;
	return result;
}

double FFXI::CYy::CMoKeyframe::GetSplineFrameValue(float a2)
{
	float* v5 = &this->field_30;
	float* v3 = &this->field_38;

	while (v3[0] < (double)a2) {
		v5 = v3;
		if (v3[0] >= 1.0) {
			v3 = nullptr;
		}
		else {
			v3 += 2;
		}
	}

	double v6 = (a2 - *v5) / (*v3 - *v5);
	if (v6 < 0.5)
		return (1.0 - (v6 * v6 + v6 * v6)) * v5[1] + v3[1] * (v6 * v6 + v6 * v6);
	double v8 = (1.0 - v6) * (1.0 - v6) + (1.0 - v6) * (1.0 - v6);
	return (1.0 - v8) * v3[1] + v5[1] * v8;
}

double FFXI::CYy::CMoKeyframe::GetSplineFrameValue(float a2, float a3)
{
	float backup = this->field_34;
	this->field_34 = a3;
	float result = this->GetSplineFrameValue(a2);
	this->field_34 = backup;
	return result;
}

