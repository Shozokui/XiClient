#pragma once
#include <filesystem>
namespace FFXI {
	namespace Config {
		class RuntimeConfig {
		public:
			static RuntimeConfig instance;
			static int Initialize(std::filesystem::path);

			//types
			struct Resolution {
				unsigned short X;
				unsigned short Y;
			};
			enum class WindowMode : int {
				Fullscreen = 0,
				Windowed = 1,
				BorderlessWindowed = 2,
				BorderlessFullscreen = 3
			};

			//install
			std::filesystem::path ffxi_install_path;
			//process
			unsigned int process_priority;
			//sound
			bool sound_enable;
			//video
			WindowMode window_mode;
			Resolution resolution_main;
			Resolution resolution_ui;
			Resolution resolution_bg;
		};
	}
}