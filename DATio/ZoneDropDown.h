#pragma once
#include <map>
#include <string>

namespace DATio {
	namespace UI {
		namespace Components {
			class ZoneDropDown {
			private:
				static std::map<unsigned short, std::string> zone_list;
				static void Refresh_Zone_List();
				int selected_zone_id{ -1 };
			public:
				static bool is_dirty;
				void Render();
			};
		}
	}
}