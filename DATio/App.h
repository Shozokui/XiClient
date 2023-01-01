#pragma once
namespace DATio {
	namespace UI {
		static class App {
		protected:
			//Prevent instantiation
			App() {};
		public:
			static bool Initialize();
			static void Begin();
		};
	}
}