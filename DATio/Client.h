#pragma once
namespace DATio {
	namespace UI {
		class ClientWindow {
			bool client_initialized{ false };
		public:
			bool visible{ false };
			void Render();
		};
	}
}