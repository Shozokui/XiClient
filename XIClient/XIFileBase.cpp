#include "Globals.h"
#include "XIFileBase.h"
#include "XiFileStat.h"
int FFXI::File::XIFileBase::Open(FILE** p_file, char* p_path, XI_OPENMODE p_mode)
{
	FILE* v4{ nullptr };
	if (!p_path) return -1;
	if (p_mode == XIFileBase::XI_OPENMODE::Read) {
		fopen_s(&v4, p_path, "rb\0");
	}
	else if (p_mode == XIFileBase::XI_OPENMODE::Write) {
		fopen_s(&v4, p_path, "wb\0");
		*p_file = v4;
		if (v4) return 1;
		XIFileBase::CreatePath(p_path);
		fopen_s(&v4, p_path, "wb\0");
		*p_file = v4;
		if (v4) return 1;
		return -1;
	}
	else if (p_mode == XIFileBase::XI_OPENMODE::Append) {
		fopen_s(&v4, p_path, "ab\0");
	}
	else {
		return -1;
	}
	*p_file = v4;
	if (v4) return 1;
	return -1;
}

int FFXI::File::XIFileBase::Read(FILE* p_file, char* p_buffer, int p_buflen)
{
	int BytesRead{ 0 };
	unsigned int ChunkCount = p_buflen / 0x40000;
	int LeftoverChunk = p_buflen % 0x40000;
	while (ChunkCount != 0) {
		BytesRead += std::fread(p_buffer, 1u, 0x40000, p_file);
		if (std::ferror(p_file)) return -1;
		p_buffer += 0x40000;
		Sleep(0);
		--ChunkCount;
	}

	if (LeftoverChunk == 0) 
		return BytesRead;

	BytesRead += fread(p_buffer, 1u, LeftoverChunk, p_file);
	if (std::ferror(p_file)) return -1;
	return BytesRead;
}

int FFXI::File::XIFileBase::Seek(FILE* p_file, int p_offset, XI_SEEKMODE p_mode)
{
	int mode = SEEK_SET;
	if ((int)p_mode) {
		if (p_mode == XIFileBase::XI_SEEKMODE::Cur) {
			mode = SEEK_CUR;
		}
		else {
			if (p_mode != XIFileBase::XI_SEEKMODE::End) return -1;
			mode = SEEK_END;
		}
	}
	//Return 1 or -1
	int v5 = -(fseek(p_file, p_offset, mode) != 0);
	v5 &= 0xFFFFFFFE;
	return v5 + 1;
}

int FFXI::File::XIFileBase::Close(FILE* p_file)
{
	return fclose(p_file);
}

off_t FFXI::File::XIFileBase::GetFileSize(char* p_filename)
{
	struct _stat stats;
	if (_stat(p_filename, &stats) != S_OK) return -1;
	return stats.st_size;
}

int FFXI::File::XIFileBase::GetFileState(char* a2, XiFileStat* a3)
{
	struct _stat stats;
	if (_stat(a2, &stats) != S_OK) {
		memset(a3, 0, sizeof(*a3)); 
		return -1;
	}

	a3->setSize(stats.st_size);
	a3->setCreation(stats.st_ctime);
	a3->setLastAccess(stats.st_atime);
	a3->setLastModify(stats.st_mtime);
	return 1;
}

void FFXI::File::XIFileBase::CreatePath(char* p_path)
{
	int v1; // edi
	int v2; // ebx
	char* v3; // esi
	char PathName[256]; // [esp+10h] [ebp-100h] BYREF

	v1 = 0;
	do
	{
		v3 = &PathName[v1];
		if (p_path[v1] == '/')
		{
			PathName[v1] = '\0';
			CreateDirectoryA(PathName, NULL);
		}
		++v1;
		PathName[v1] = p_path[v1];
	} while (v1 < sizeof(PathName));
}
