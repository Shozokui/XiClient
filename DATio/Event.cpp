#include "Event.h"
#include "imgui/imgui.h"
#include "ZoneDropDown.h"

ImGuiWindowFlags window_flags = 0;
DATio::UI::Components::ZoneDropDown zone_drop_down{};

void DATio::UI::EventWindow::Render()
{
	if (ImGui::Begin("Event", &this->visible, window_flags) == true) {
		zone_drop_down.Render();
	}

	ImGui::End();
}
