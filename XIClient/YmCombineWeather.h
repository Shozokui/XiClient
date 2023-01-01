#pragma once
#include "StorageListObject.h"
#include "XiDateTime.h"
namespace FFXI {
	class XiWeather;
	namespace CYy {
		class XiArea;
		class CYyResourceFile;
		class CMoWeather;
		class CYySepRes;
		class YmCombineWeather : public StorageListObject {
		public:
			static const CYyClass YmCombineWeatherClass;
			static XiWeather* CreateCurrentWeather();
			virtual const CYyClass* GetRuntimeClass() override final;
			YmCombineWeather(XiArea*, int, int);
			~YmCombineWeather();
			void OnMoveAll();
			void OnMove();
			void MakeCurrentWeather();
			void ReleaseCurrentWeather();
			void FindSetPrevWeather();
			CMoWeather* FindPrevWeather(int);
			void SetPrevWeather(CMoWeather*);
			void FindSetNextWeather();
			CMoWeather* FindNextWeather(int);
			void SetNextWeather(CMoWeather*);
			void FindSetPrevSound();
			CYySepRes** FindPrevSound(int);
			void SetPrevSound(CYySepRes**);
			void FindSetNextSound();
			CYySepRes** FindNextSound(int);
			void SetNextSound(CYySepRes**);
			YmCombineWeather* GetParentWeather();
			void UpdateWeather();
			XiArea* Area;
			YmCombineWeather* AreasCombineWeather;
			CYyResourceFile* WeatherFile;
			int fourCC;
			XiDateTime field_14;
			XiDateTime field_18;
			float field_1C;
			XiWeather* field_20;
			CMoWeather* PrevWeather;
			CMoWeather* NextWeather;
			CYySepRes** PrevSound;
			CYySepRes** NextSound;
			char field_34;
			char field_35;
			int field_36;
			int field_3A;
			int field_3E;
			int field_42;
		};
	}
}