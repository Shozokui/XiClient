#pragma once
#include "CMoElem.h"
#include "Values.h"
namespace FFXI {
	namespace CYy {
		class CYySepRes;
		class CMoSphRes;
		class CYySoundElem : public CMoElem {
		public:
			static const CYyClass CYySoundElemClass;
			static CYySoundElem* head;
			static CYySepRes* zone_sound;
			static int zone_sound_id;
			static float system_volume;
			static float effect_volume;
			static float zone_volume;
			static float back_sound_rate;
			static float sound_near;
			static float sound_far;
			static float sound_width;
			static bool sound_standby;
			static bool check_flag;
			static int play_num;
			static int se_req_num;
			static int entry_num;
			static int report_buffer[FFXI::Constants::Values::INDEX_SOUND_REPORT_MAX];
			static CYy::CYySepRes** req_buffer[FFXI::Constants::Values::INDEX_SOUND_REQUEST_MAX];
			static void SysMove();
			static void SetZoneSound(CYySepRes*);
			static void AddEntry(CYySepRes**);
			static bool SendData(CYySepRes**);
			static bool EntryExist(CYySepRes**);
			static bool PlayExist(int);
			const CYyClass* GetRuntimeClass() override final;
			virtual char OnMove() override final;
			virtual bool VirtElem1(FFXI::Constants::Enums::ElemType) override final;
			virtual bool IsNever() override final;
			CYySoundElem();
			virtual ~CYySoundElem();
			void OnMove2();
			bool OnPlayUpdate();
			bool PlayExist();
			void Play(float*, int);
			void Stop();
			CYySoundElem* previous;
			CMoSphRes* field_198;
			int field_19C;
			D3DXVECTOR3 field_1A0;
			int field_1AC;
			float s_near;
			float s_far;
			float s_width;
			short field_1BC;
			short field_1BE;
			float field_1C0[4];
			float field_1D0[4];
			int field_1E0;
			float field_1E4;
			//ALIGNMENT char 1e8 is out of order
			int field_1E9;
			unsigned char field_1E8;
			char field_1ED;
			char field_1EE;
			char field_1EF;
		};
	}
}