#pragma once
#include <filesystem>

namespace DATio {
	namespace IO {
		class XIDirMng {
		public:
			enum class result_code {
				OK,
				BASE_VTABLE_MISSING,
				BASE_VTABLE_BLANK,
				EXPANSION_VTABLE_SIZE_MISMATCH,
				BASE_FTABLE_MISSING,
				BASE_FTABLE_BLANK,
				EXPANSION_FTABLE_SIZE_MISMATCH,
				BASE_VTABLE_FTABLE_SIZE_MISMATCH,
				FILESYSTEM_FAILURE
			};
			result_code TrySetHead(std::filesystem::path);
			std::filesystem::path GetHead();
		private:
			std::filesystem::path head = "-";
			std::vector<unsigned char> vtable;
			std::vector<unsigned short> ftable;
			result_code BuildIndexTables(std::filesystem::path);
			result_code BuildVTable(std::filesystem::path);
			result_code BuildFTable(std::filesystem::path);
		};
	}
}