#include "ZoneDropDown.h"
#include "imgui/imgui.h"

bool DATio::UI::Components::ZoneDropDown::is_dirty = true;
std::map<unsigned short, std::string> DATio::UI::Components::ZoneDropDown::zone_list{};

void DATio::UI::Components::ZoneDropDown::Refresh_Zone_List()
{
	DATio::UI::Components::ZoneDropDown::zone_list.clear();
	DATio::UI::Components::ZoneDropDown::zone_list.emplace(1, "Ronfaure");
	DATio::UI::Components::ZoneDropDown::zone_list.emplace(2, "Gustaberg");
}
void DATio::UI::Components::ZoneDropDown::Render()
{
	if (this->is_dirty == true) {
		this->selected_zone_id = -1;
		DATio::UI::Components::ZoneDropDown::Refresh_Zone_List();
		this->is_dirty = false;
	}

	char text[256];
	std::string combo_text;
	if (this->selected_zone_id == -1) {
		combo_text += "Select a zone";
	}
	else {
		sprintf_s(text, "[%0.3d] %s", this->selected_zone_id, this->zone_list[this->selected_zone_id].c_str());
		combo_text += text;
	}

	if (ImGui::BeginCombo("Zone", combo_text.c_str(), NULL) == true) {

		//Iterate the zone list
		for (std::pair<unsigned short, std::string> zone : DATio::UI::Components::ZoneDropDown::zone_list) {
			sprintf_s(text, "[%0.3d] %s", zone.first, zone.second.c_str());

			bool is_selected = this->selected_zone_id == zone.first;
			if (ImGui::Selectable(text, is_selected) == true) {
				//Check if they selected the already-selected option
				if (this->selected_zone_id != zone.first) {
					this->selected_zone_id = zone.first;
				}
			}

			if (is_selected == true) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}
