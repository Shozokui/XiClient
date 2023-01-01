#include "CommandLineConfig.h"
#include <string.h>

FFXI::Config::CommandLineConfig FFXI::Config::CommandLineConfig::instance{};

bool FFXI::Config::CommandLineConfig::Init(const char* args)
{
	FFXI::Config::CommandLineConfig::instance.cl_args = nullptr;
	if (args == nullptr) {
		return true;
	}

	int len = strlen(args) + 1;
	FFXI::Config::CommandLineConfig::instance.cl_args = new char[len];

	if (FFXI::Config::CommandLineConfig::instance.cl_args == nullptr) {
		return false;
	}

	strcpy_s(FFXI::Config::CommandLineConfig::instance.cl_args, len, args);
	return true;
}

void FFXI::Config::CommandLineConfig::Uninit()
{
	if (FFXI::Config::CommandLineConfig::instance.cl_args != nullptr) {
		delete[] FFXI::Config::CommandLineConfig::instance.cl_args;
		FFXI::Config::CommandLineConfig::instance.cl_args = nullptr;
	}
}

char* FFXI::Config::CommandLineConfig::ArgGetParam(const char* command, char* out, int out_size, int count)
{
	if (this->cl_args == nullptr) {
		return nullptr;
	}

	//Find the first substring match
	char* substring = strstr(this->cl_args, command);
	if (substring == nullptr) {
		return nullptr;
	}

	//Keep searching substrings until a non-partial match is found
	unsigned int substring_length = strlen(command);
	while (true) {
		char v5 = substring[substring_length];
		if (v5 == 0 || v5 == ' ' || v5 == '0' ||
			v5 == '1' || v5 == '2' || v5 == '3' ||
			v5 == '4' || v5 == '5' || v5 == '6' ||
			v5 == '7' || v5 == '8' || v5 == '9') {
			break;
		}
		substring = strstr(substring + 1, command);
		if (substring == nullptr) {
			return nullptr;
		}
	}

	char* result = &substring[substring_length];
	if (out == nullptr) {
		return result;
	}

	//Skip past spaces, tabs and nulls
	while (*result == ' ' || *result == '\t' || *result == 0) {
		result += 1;
	}

	//Different behavior depending on count's value
	int out_pos = 0;
	if (count != 0) {
		while (count > 0 && out_pos < out_size) {
			out[out_pos] = *result;
			out_pos += 1;
			result += 1;
			count -= 1;
		}
	}
	else {
		//Copy until a space, tab, or nullchar is found
		while (out_pos < out_size && *result != ' ' && *result != '\t' && *result != 0) {
			out[out_pos] = *result;
			out_pos += 1;
			result += 1;
		}
	}

	//Null terminate out
	if (out_pos >= out_size) {
		out_pos = out_size - 1;
	}
	out[out_pos] = 0;

	return result;
}

void FFXI::Config::CommandLineConfig::AppendLaunchArgs(const char* a1)
{
	if (a1 == nullptr) {
		return;
	}

	//Total length of the combined strings plus space and null char
	int newlen = strlen(this->cl_args) + strlen(a1) + 2;

	char* new_buffer = new char[newlen];
	if (new_buffer != nullptr) {
		strcpy_s(new_buffer, newlen, this->cl_args);
		int len = strlen(new_buffer);
		new_buffer[len] = ' ';
		new_buffer[len + 1] = '\0';
		strcat_s(new_buffer, newlen, a1);
		if (this->cl_args != nullptr) {
			delete[] this->cl_args;
		}
		this->cl_args = new_buffer;
	}
}
