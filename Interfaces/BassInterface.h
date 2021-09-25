#pragma once

#include "../bass.h"

#include <cstdlib>
#include <string>
#include <filesystem>
#include <iostream>

#define CONAUD_VERSION 1.1

using namespace std;
namespace fs = std::filesystem;

namespace BASSI {
	string Formats[7] = {    // These are the supported file formats.
		".aif",
		".aiff",
		".mp1",
		".mp2",
		".mp3",
		".wav",
		".ogg"
	};

	void Error(const char *text){
		printf(" Error(%d): %s\n", BASS_ErrorGetCode(), text);
		BASS_Free();
		exit(0);
		return;
	}

	void ListDevices(){
		BASS_DEVICEINFO di;
		int a;
		printf(" ALL Devices : \n");
		for (a = 0; BASS_GetDeviceInfo(a, &di); a++) {
			printf("\tDev %d: %s\n\t Driver: %s\n", a, di.name, di.driver);
		}
		return;
	}

	void ListVersions(){
		printf(" Conaud Version : %2.2f\n",CONAUD_VERSION);
		return;
	}

	inline bool SupportedFormat(fs::path Path){
		for(int i = 0; i < 7; i++){
			if(Path.string().find(Formats[i]) != Path.string().npos){
				return true;
			}
		}
		return false;
	}

	void PlayFile(fs::path File,int device = -1){
		DWORD Channel, Active, Level;
		BOOL IsMOD = FALSE;
		QWORD Pos,Byte_Len;
		double Length = 0.0,Len = 0.0;
		int min,secs,_min,_secs;
		BASS_CHANNELINFO info;

		if(!SupportedFormat(File.c_str())){
			return;
		} else if(SupportedFormat(File.c_str())){
			Channel = BASS_StreamCreateFile(FALSE,File.c_str(),0,0,BASS_SAMPLE_LOOP);
			if (!Channel && BASS_ErrorGetCode() == BASS_ERROR_FILEFORM) { // For MOD music formats
				Channel = BASS_MusicLoad(FALSE,File.c_str(),0,0,BASS_MUSIC_SURROUND | BASS_UNICODE | BASS_MUSIC_PRESCAN, 1);
				IsMOD = TRUE;
			}
		}

		if(!Channel){
			Error(" Can't play the file.");
		}

		BASS_ChannelGetInfo(Channel, &info);
	
		cout << "\n Now Playing : " << File.string();
		printf("\n Channel Type : %x\n",info.ctype);

		if(!IsMOD){
			if(info.origres){
				printf(" Format : %u Hz, %d Channel, %d bit\n", info.freq, info.chans, LOWORD(info.origres));
			} else {
				printf(" Format : %u Hz, %d Channel\n", info.freq, info.chans);
			}
		}

		Byte_Len = BASS_ChannelGetLength(Channel, BASS_POS_BYTE);
		if(Byte_Len != -1){
			Length = BASS_ChannelBytes2Seconds(Channel, Byte_Len);
			min = (int)Length / 60;
			secs = (int)Length % 60;
			if (IsMOD){
				printf(" Length: %u:%02u (%llu samples), %u orders\n",(unsigned int)Length / 60,(unsigned int)Length % 60,(long long)(Length * info.freq),(DWORD)BASS_ChannelGetLength(Channel, BASS_POS_MUSIC_ORDER));
			} else {
				printf(" Length: %u:%02u (%llu samples)\n",(unsigned int)Length / 60,(unsigned int)Length % 60,(long long)(Length * info.freq));
			}
		} else if(IsMOD){
			printf(" Length: %u Orders\n", (DWORD)BASS_ChannelGetLength(Channel, BASS_POS_MUSIC_ORDER));
		}
		BASS_ChannelSetPosition(Channel,Byte_Len,BASS_POS_END);
		BASS_ChannelPlay(Channel,FALSE);

		Active = BASS_ChannelIsActive(Channel);
		while(Active == BASS_ACTIVE_PLAYING){
			Pos = BASS_ChannelGetPosition(Channel,BASS_POS_BYTE);
			Len = BASS_ChannelBytes2Seconds(Channel,Pos);
	
			_min = (int)Len / 60;
			_secs = (int)Len % 60;
			printf(" Progress: %u:%02u\r",_min,_secs);
			if(_min == min && _secs == secs){
				break;
			}
		}
	
		printf("\n");
		BASS_ChannelFree(Channel);
	}

	void PlayFromDir(fs::path Path){
		if(fs::is_directory(fs::status(Path)) && (fs::status_known(status(Path)) ? fs::exists(status(Path)) : fs::exists(Path))){
			for(auto it : fs::directory_iterator(Path)){
				if(SupportedFormat(it.path())){
					PlayFile(it.path().string().c_str(),-1);
				}
			}
			return;
		} else {
			printf("\n Didn't found the directory,or it isn't a directory.\n Type \'conaud.exe -h\' for help.\n");
			return;
		}
	}

	void PlayOnline(string Link,int device = -1){
		DWORD Channel;
		BOOL IsMOD = FALSE;
		QWORD Pos,Byte_Len;
		double Length = 0.0,Len = 0.0;
		int min,secs,_min,_secs;
		BASS_CHANNELINFO info;

		if(strstr(Link.c_str(), static_cast<const char*>("://"))){
			Channel = BASS_StreamCreateURL(Link.c_str(),0,BASS_SAMPLE_LOOP,0,0);
		} else {
			printf("\n Online file not found.\n");
		}

		if (!Channel){
			Error(" Can't play the file.\n");
		}

		BASS_ChannelGetInfo(Channel, &info);
	
		cout << "\n Now Playing : " << Link;
		printf("\n Channel Type: %x\n",info.ctype);

		if (!IsMOD) {
			if (info.origres){
				printf(" format: %u Hz, %d Channel, %d bit\n", info.freq, info.chans, LOWORD(info.origres));
			} else {
				printf(" format: %u Hz, %d Channel\n", info.freq, info.chans);
			}
		}

		Byte_Len = BASS_ChannelGetLength(Channel, BASS_POS_BYTE);
		if (Byte_Len != -1) {
			Length = BASS_ChannelBytes2Seconds(Channel, Byte_Len);
			min = (int)Length / 60;
			secs = (int)Length % 60;
			if (IsMOD){
				printf(" Length: %u:%02u (%llu Samples), %u Orders\n",(unsigned int)Length / 60,(unsigned int)Length % 60,(long long)(Length * info.freq),(DWORD)BASS_ChannelGetLength(Channel, BASS_POS_MUSIC_ORDER));
			} else {
				printf(" Length: %u:%02u (%llu Samples)\n",(unsigned int)Length / 60,(unsigned int)Length % 60,(long long)(Length * info.freq));
			}
		} else if (IsMOD){
			printf(" Length: %u Orders\n", (DWORD)BASS_ChannelGetLength(Channel, BASS_POS_MUSIC_ORDER));
		}
		BASS_ChannelSetPosition(Channel,Byte_Len,BASS_POS_END);
		BASS_ChannelPlay(Channel,FALSE);

		DWORD Active = BASS_ChannelIsActive(Channel);
		while(Active == BASS_ACTIVE_PLAYING){
			Pos = BASS_ChannelGetPosition(Channel,BASS_POS_BYTE);
			Len = BASS_ChannelBytes2Seconds(Channel,Pos);

			_min = (int)Len / 60;
			_secs = (int)Len % 60;
			printf(" Progress: %u:%02u\r",_min,_secs);
			if(_min == min && _secs == secs){
				break;
			}
		}
	
		printf("\n");
		BASS_ChannelFree(Channel);
	}
}
