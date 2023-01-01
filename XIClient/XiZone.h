#pragma once
#include "XiArea.h"
#include "XiDateTime.h"
namespace FFXI {
	namespace CYy {
		class CXiActor;
		class XiZone : public XiArea {
		public:
			const static CYyClass XiZoneClass;
			XiZone();
			virtual ~XiZone();
			virtual const CYyClass* GetRuntimeClass() override final;
			virtual void VObj2(int*) override final;
			virtual void SetAction(int, CXiActor*, CXiActor*);
			static XiZone* zone;
			static float SomeFloat;
			static void SysInit();
			static void Open(int);
			static void SetFile(int);
			void Close();
			void InitWeather(CYyResourceFile*);
			void SetWeatherTable(int);
			void SetPlaceCode(int);
			void SetCurrentArea(XiArea*);
			float GetDrawDistance(bool);
			void SysMove();
			CYyResourceFile* field_1AC;
			int field_1B0;
			int ZoneID;
			int field_1B8;
			int PlaceCode;
			XiArea* CurrentArea;
			XiArea* field_1C4;
			XiDateTime field_1C8;
			short field_1CC;
			char field_1CE;
			char field_1CF;
			float MinimumDrawDistance;
			float field_1D4;
			const int* WeatherTable;
		};
	}
}