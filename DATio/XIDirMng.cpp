#include "XIDirMng.h"
#include <fstream>

DATio::IO::XIDirMng::result_code DATio::IO::XIDirMng::BuildIndexTables(std::filesystem::path head)
{
	DATio::IO::XIDirMng::result_code vtable_result = this->BuildVTable(head);
	if (vtable_result != DATio::IO::XIDirMng::result_code::OK) {
		return vtable_result;
	}

	DATio::IO::XIDirMng::result_code ftable_result = this->BuildFTable(head);
	if (ftable_result != DATio::IO::XIDirMng::result_code::OK) {
		return ftable_result;
	}

	return DATio::IO::XIDirMng::result_code::OK;
}

DATio::IO::XIDirMng::result_code DATio::IO::XIDirMng::BuildVTable(std::filesystem::path head)
{
	std::filesystem::path base_vtable = head / "VTABLE.DAT";
	//the base vtable is required
	if (std::filesystem::exists(base_vtable) == false) {
		return DATio::IO::XIDirMng::result_code::BASE_VTABLE_MISSING;
	}

	uintmax_t filesize = std::filesystem::file_size(base_vtable);
	if (filesize == 0) {
		return DATio::IO::XIDirMng::result_code::BASE_VTABLE_BLANK;;
	}

	this->vtable.resize(filesize);
	
	std::ifstream ifs(base_vtable, std::ios::binary | std::ios::beg);
	if (!ifs) {
		return DATio::IO::XIDirMng::result_code::FILESYSTEM_FAILURE;
	}
	
	ifs.read((char*)this->vtable.data(), filesize);
	ifs.close();

	char rom_folder[] = "ROM0";
	char vtable_file[] = "VTABLE0.DAT";
	std::vector<unsigned char> file_buffer(filesize);
	for (char rom_index = 2; ; ++rom_index) {

		rom_folder[3] = '0' + rom_index;
		vtable_file[6] = rom_folder[3];

		std::filesystem::path vtable_expansion_path = head / rom_folder / vtable_file;
		if (std::filesystem::exists(vtable_expansion_path) == false) {
			break;
		}

		if (std::filesystem::file_size(vtable_expansion_path) != filesize) {
			return DATio::IO::XIDirMng::result_code::EXPANSION_VTABLE_SIZE_MISMATCH;
		}

		std::ifstream eifs(vtable_expansion_path, std::ios::binary | std::ios::beg);
		if (!eifs) {
			return DATio::IO::XIDirMng::result_code::FILESYSTEM_FAILURE;
		}

		eifs.read((char*)file_buffer.data(), filesize);
		eifs.close();

		for (uintmax_t pos = 0; pos < filesize; ++pos) {
			if (file_buffer[pos] == rom_index) {
				this->vtable[pos] = rom_index;
			}
		}
	}

	return DATio::IO::XIDirMng::result_code::OK;
}

DATio::IO::XIDirMng::result_code DATio::IO::XIDirMng::BuildFTable(std::filesystem::path head)
{
	std::filesystem::path base_ftable = head / "FTABLE.DAT";
	//the base ftable is required
	if (std::filesystem::exists(base_ftable) == false) {
		return DATio::IO::XIDirMng::result_code::BASE_FTABLE_MISSING;
	}

	uintmax_t filesize = std::filesystem::file_size(base_ftable);
	if (filesize == 0) {
		return DATio::IO::XIDirMng::result_code::BASE_FTABLE_BLANK;
	}

	this->ftable.resize(filesize / 2);
	if (this->ftable.size() != this->vtable.size()) {
		return DATio::IO::XIDirMng::result_code::BASE_VTABLE_FTABLE_SIZE_MISMATCH;
	}

	std::ifstream ifs(base_ftable, std::ios::binary | std::ios::beg);
	if (!ifs) {
		return DATio::IO::XIDirMng::result_code::FILESYSTEM_FAILURE;
	}

	ifs.read((char*)this->ftable.data(), filesize);
	ifs.close();

	char rom_folder[] = "ROM0";
	char ftable_file[] = "FTABLE0.DAT";
	std::vector<unsigned short> file_buffer(this->ftable.size());
	for (char rom_index = 2; ; ++rom_index) {

		rom_folder[3] = '0' + rom_index;
		ftable_file[6] = rom_folder[3];

		std::filesystem::path ftable_expansion_path = head / rom_folder / ftable_file;
		if (std::filesystem::exists(ftable_expansion_path) == false) {
			break;
		}

		if (std::filesystem::file_size(ftable_expansion_path) != filesize) {
			return DATio::IO::XIDirMng::result_code::EXPANSION_FTABLE_SIZE_MISMATCH;
		}

		std::ifstream eifs(ftable_expansion_path, std::ios::binary | std::ios::beg);
		if (!eifs) {
			return DATio::IO::XIDirMng::result_code::FILESYSTEM_FAILURE;
		}

		eifs.read((char*)file_buffer.data(), file_buffer.size());
		eifs.close();

		for (uintmax_t pos = 0; pos < this->vtable.size(); ++pos) {
			if (this->vtable[pos] == rom_index) {
				this->ftable[pos] = file_buffer[pos];
			}
		}
	}

	return DATio::IO::XIDirMng::result_code::OK;
}

DATio::IO::XIDirMng::result_code DATio::IO::XIDirMng::TrySetHead(std::filesystem::path path)
{
	this->vtable.clear();
	this->ftable.clear();
	this->head = "-";

	DATio::IO::XIDirMng::result_code tables_result = this->BuildIndexTables(path);
	if (tables_result != DATio::IO::XIDirMng::result_code::OK) {
		return tables_result;
	}

	this->head = path;
	return DATio::IO::XIDirMng::result_code::OK;
}

std::filesystem::path DATio::IO::XIDirMng::GetHead()
{
	return this->head;
}
