#include "KO_Path.h"
#include "CYyDb.h"
#include "BaseProcessor.h"

float RailData_GetDirection(float* a1, float* a2, D3DXVECTOR3* a3, D3DXVECTOR3* a4) {
	D3DXVECTOR3 v20{}, v21{}, v22{}, v23{};
	v20.x = a1[0] - a2[0];
	v20.y = a1[1] - a2[1];
	v20.z = a1[2] - a2[2];
	v21 = v20;
	v23 = v20;
	v22.x = a1[0] - a4->x;
	v22.y = a1[1] - a4->y;
	v22.z = a1[2] - a4->z;
	float v26 = FFXI::CYyDb::g_pCYyDb->pMoProcessor->Vec3Magnitude(&v23);
	FFXI::CYyDb::g_pCYyDb->pMoProcessor->VirtProcessor35(&v20);
	float v25 = -(v20.z * v22.z + v20.y * v22.y + v20.x * v22.x);

	if (v25 > 0.0) {
		v25 = 0.0;
	}
	if (v25 < -v26) {
		v25 = -v26;
	}

	v20.x = v20.x * v25 + a1[0];
	v20.y = v20.y * v25 + a1[1];
	v20.z = v20.z * v25 + a1[2];

	*a3 = v20 - *a4;

	float retval = FFXI::CYyDb::g_pCYyDb->pMoProcessor->Vec3Magnitude(a3);
	if (v26 > 0.0) {
		a3->x *= 1.0 / retval;
		a3->y *= 1.0 / retval;
		a3->z *= 1.0 / retval;
		float v19 = (1.0 - -(v25 / v26)) * a1[3] + -(v25 / v26) * a2[3];
		retval -= v19;
		if (retval < 0.0) {
			retval = 0.0;
		}
		a3->x *= retval;
		a3->y *= retval;
		a3->z *= retval;
	}

	return retval;
}

void FFXI::KO_Path::GetDirection(D3DXVECTOR3* a2, D3DXVECTOR3* a3)
{
	if (this->field_14[0] <= 0) {
		return;
	}

	float v14 = 1000000000.0;
	int* v5 = this->field_14 + 4;
	D3DXVECTOR3 v15{};
	int count = this->field_14[0];
	if (count <= 0) {
		return;
	}

	for (int i = 0; i < count; ++i) {

		float f = RailData_GetDirection(
			this->field_10 + 4 + 8 * (v5[0] & 0x00FFFFFF),
			this->field_10 + 4 + 8 * (v5[1] & 0x00FFFFFF),
			&v15,
			a3);

		if (v14 > f) {
			*a2 = v15;
			v14 = f;
		}
		v5 += 2;
	}
}
