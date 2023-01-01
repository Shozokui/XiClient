#include "CYyMotionMan.h"
#include "MotionManStaticStruct.h"
#include "CXiActor.h"
#include "KzFQuat.h"
#include "CYySkl.h"
#include "CMoMo2.h"
#include "Globals.h"

using namespace FFXI::CYy;

MotionManStaticStruct CYyMotionMan::g_mmss_arr[256]{};
MotionManStaticStruct local_mmss_arr[256]{};

void FFXI::CYy::CYyMotionMan::AppendSync(CXiActor* a2, CMoMo2** a3, float a4, float a5, int a6, float a7, float a8, int a9, float a10, float a11)
{
	if (a9 >= 0 && a9 < sizeof(this->motions) / sizeof(this->motions[0])) {
		if (a2->GetMotStop() == false || a2->IsMotionLock() == true) {
			this->motions[a9].AppendSync(a3, a4, a5, a6, a7, a8, a10, a11);
			if (a9 == 0 && a2 != nullptr)
				a2->SetMotionLock(false);
		}
	}
}

void FFXI::CYy::CYyMotionMan::Append(CXiActor* a2, CMoMo2** a3, float a4, float a5, int a6, float a7, float a8, int a9, float a10, float a11)
{
	if (a9 < 0 || a9 >= (sizeof(this->motions) / sizeof(this->motions[0])))
		return;

	if (a2->GetMotStop() == false
		|| a2->IsMotionLock() == true) {
		this->motions[a9].Append(a3, a4, a5, a6, a7, a8, a10, a11);
		if (a9 == 0) {
			if (a2 != nullptr) {
				a2->SetMotionLock(false);
			}
		}
	}
}

bool FFXI::CYy::CYyMotionMan::SetStatics()
{
	static FFXI::Math::KzFQuat quat{};

	for (int i = 0; i < CYySkl::g_count; ++i) {
		MotionManStaticStruct* ss = CYyMotionMan::g_mmss_arr + i;
		ss->field_0 = quat;
		ss->field_10 = { 0.0, 0.0, 0.0 };
		ss->field_1C = { 1.0, 1.0, 1.0 };
	}

	int sum = 0;
	for (int i = 4; i >= 0; --i) {
		sum += this->motions[i].GetNbQue();
	}

	if (sum == 0)
		return false;

	for (int i = 4; i >= 0; --i) {
		if (this->motions[i].Head == nullptr)
			continue;

		this->motions[i].UpdateFloats(i, CYyMotionMan::g_mmss_arr);
		for (int j = 0; j < CYySkl::g_count; ++j) {
			if ((CYySkl::g_obj[j] & 0x3F) != 0) {
				CYySkl::g_obj[j] |= 0x40;
			}
		}
	}

	for (int i = 5; i < sizeof(this->motions) / sizeof(this->motions[0]); ++i) {
		CYyMotionQueList* list = this->motions + i;
		if (list->Head == nullptr)
			continue;

		for (int j = 0; j < CYySkl::g_count; ++j) {
			CYySkl::g_obj[j] &= 0x80u;
		}

		list->UpdateFloats(i, local_mmss_arr);

		for (int j = 0; j < CYySkl::g_count; ++j) {
			if ((CYySkl::g_obj[j] & 0x80) != 0) {
				if ((CYySkl::g_obj[j] & 0x3F) != 0) {
					MotionManStaticStruct* mmss = CYyMotionMan::g_mmss_arr + j;
					FFXI::Math::KzFQuat v25{};
					mmss->field_0.lerp(&v25, &local_mmss_arr[j].field_0, list->field_C);

					Globals::Vec3Lerp(
						&CYyMotionMan::g_mmss_arr[j].field_10,
						&local_mmss_arr[j].field_10,
						&CYyMotionMan::g_mmss_arr[j].field_10,
						list->field_C);
					
					Globals::Vec3Lerp(
						&CYyMotionMan::g_mmss_arr[j].field_1C,
						&local_mmss_arr[j].field_1C,
						&CYyMotionMan::g_mmss_arr[j].field_1C,
						list->field_C);
				}
			}
		}
	}
	return true;
}
