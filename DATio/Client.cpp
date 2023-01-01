#include "Client.h"
#include "imgui/imgui.h"
#include "../XIClient/XIClient.h"

void DATio::UI::ClientWindow::Render()
{
	if (this->client_initialized == false) {
		XIClient::StartMain(NULL, NULL, NULL, NULL);
		this->client_initialized = true;
	}
	if (ImGui::Begin("Client", &this->visible, 0) == true) {

	}

	ImGui::End();
}