#include "CYyMusicLoadTask.h"
#include "Globals.h"
#include "SoundMng.h"
#include "CYyDb.h"
#include "MusicServer.h"
#include <wtypes.h>
using namespace FFXI::CYy;

const CYyClass CYyMusicLoadTask::CYyMusicLoadTaskClass{
	"CYyMusicLoadTask", sizeof(CYyMusicLoadTask), &CMoTask::CMoTaskClass
};
const CYyClass* FFXI::CYy::CYyMusicLoadTask::GetRuntimeClass()
{
	return &CYyMusicLoadTaskClass;
}

void FFXI::CYy::CYyMusicLoadTask::VObj2(int* a2)
{
	if (a2[0] != 2) {
		if (a2[0] != 5) {
			return;
		}
	}

	if (this)
		delete this;
}

char FFXI::CYy::CYyMusicLoadTask::OnMove()
{
	float v15{ 0.0 };
	int v8{ 0 };
	switch (this->field_38) {
	case 0:
		if (this->field_34 && FFXI::SoundMng::g_pSoundMng)
			SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::SetReplayPoint, 0, 0, 0, 0, 0, 0, 0);
		if (!SoundMng::SoundThreadRunning || !FFXI::SoundMng::g_pSoundMng) {
			SoundMng::sound_command_send = 1;
			++this->field_38;
			return false;
		}
		SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::MusicFadeOut, -1, this->field_4C, 0, 0, 0, 0, 0);
		SoundMng::sound_command_send = 1;
		++this->field_38;
		return 0;
	case 1:
	case 6:
		v15 = (float)this->field_4C - CYyDb::g_pCYyDb->CheckTick();
		this->field_4C = (int)v15;
		if (v15 >= 0)
			return false;
		if (SoundMng::SoundThreadRunning && FFXI::SoundMng::g_pSoundMng)
			SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::MusicFadeOut, -1, 0, 0, 0, 0, 0, 0);
		SoundMng::sound_command_send = 1;
		this->field_4C = 4;
		this->field_38 += 1;
		return 0;
	case 2:
		this->field_4C -= 1;
		if (this->field_4C >= 0)
			return false;
		if (FFXI::SoundMng::g_pSoundMng)
			SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::_6, -1, 0, 0, 0, 0, 0, 0);
		SoundMng::sound_command_send = 1;
		this->field_38 += 1;
		return false;
	case 3:
		//some dword
		if (!this->field_35) {
			if (SoundMng::SoundThreadRunning) {
				double value = (double)this->field_40 * SoundMng::SomeFloat2 * SoundMng::SomeFloat1;
				SoundMng::PlayMusicFromDisk(this->field_3C, (int)value, 0);
			}
				SoundMng::sound_command_send = 1;
		}
		if (this->field_40 == this->field_44) {
			this->field_58 = 0;
			this->field_38 += 1;
		}
		else {
			this->field_58 = 10;
			this->field_38 += 1;
		}
		return false;
	case 4:
		if (this->field_58) {
			this->field_58 -= 1;
			return true;
		}
		else {
			if (!this->field_35) {
				double value = (double)this->field_44 * SoundMng::SomeFloat2 * SoundMng::SomeFloat1;
				if (this->field_48) {
					if (SoundMng::SoundThreadRunning && FFXI::SoundMng::g_pSoundMng) {
						SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::VolumeTranslate, -1, ((int)value) & 0x7F, this->field_48, 0, 0, 0, 0);
					}
				}
				else if (SoundMng::SoundThreadRunning && FFXI::SoundMng::g_pSoundMng) {
					SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::VolumeTranslate, -1, ((int)value) & 0x7F, 0, 0, 0, 0, 0);
				}
			}
			v8 = SoundMng::MusicTable2[this->field_50].Two;
			if (!v8 || this->field_54 == 1) {
				delete this;
				return true;
			}
			else {
				this->field_4C = v8;
				this->field_38 += 1;
				return false;
			}
		}
	case 5:
		v15 = (float)this->field_4C - CYyDb::g_pCYyDb->CheckTick();
		this->field_4C = (int)v15;
		if (v15 >= 0)
			return false;
		this->field_4C = 360;
		if (!SoundMng::SoundThreadRunning || !FFXI::SoundMng::g_pSoundMng) {
			this->field_38 += 1;
			return false;
		}
		SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::MusicFadeOut, -1, 360, 0, 0, 0, 0, 0);
		this->field_38 += 1;
		return false;
	case 7:
		this->field_4C -= 1;
		if (this->field_4C >= 0)
			return false;
		v8 = SoundMng::MusicTable2[this->field_50].Three;
		this->field_3C = v8;
		MusicServer::last_request = v8;
		if (FFXI::SoundMng::g_pSoundMng)
			SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::_6, -1, 0, 0, 0, 0, 0, 0);
		SoundMng::sound_command_send = 1;
		this->field_38 += 1;
		return false;
	case 8:
		if (SoundMng::SoundThreadRunning) {
			double value = (double)this->field_44 * SoundMng::SomeFloat2 * SoundMng::SomeFloat1;
			SoundMng::PlayMusicFromDisk(this->field_3C, (int)value, 0);
		}
		SoundMng::sound_command_send = 1;
		//some dword
		if (FFXI::SoundMng::g_pSoundMng) {
			double value = (double)this->field_44 * SoundMng::SomeFloat2 * SoundMng::SomeFloat1;
			SoundMng::QueueNewSoundCommand(Constants::Enums::SoundCommand::VolumeTranslate, -1, ((int)value) & 0x7F, 0, 0, 0, 0, 0);
		}
		delete this;
		return true;
	default:
		return false;
	}
}
