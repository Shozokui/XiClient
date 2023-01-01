#pragma once
namespace FFXI {
	namespace Config {
		class CommandLineConfig {
		private:
			char* cl_args;
		public:
			static CommandLineConfig instance;
			static bool Init(const char*);
			static void Uninit();
			//Searches the command line args for a param
			char* ArgGetParam(const char*, char*, int, int);
			//Appends a string to the command line args with a space between
			void AppendLaunchArgs(const char*);
		};
	}
}