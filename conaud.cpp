#include "Interfaces/BassInterface.h"
#include "Interfaces/MediaInfoInterface.h"

#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
	BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 2);

	if (!BASS_Init(-1,48000,0,0,NULL)){
		BASSI::Error(" Can't initialize device");
	}

	// Check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
		printf("\t Conaud : Error : An incorrect version of BASS was loaded.\n\tExpected Bass %d",BASSVERSION);
		return 0;
	}

	// Parse the arguments
	if(!argv[1]){
		printf("\n No arguments provided. Type \'conaud.exe -h\' for help.\n");
	} else if(!strcmp(argv[1],"-ld")){
		BASSI::ListDevices();
		return 0;
	} else if(!strcmp(argv[1],"-v")){
		BASSI::ListVersions();
		return 0;
	} else if(!strcmp(argv[1],"-h")){
		printf("\n Usage: conaud.exe argument <file> option\n Arguments:\n"
			   "\t-ld         =   list devices\n\t-v          =   version\n\t-h          =   print this message\n\t<file>      =   play the file"
			   "\n Options:\n\t--playlist  =   play files from a directory\n\t--online    =   play file from the web\n\t--info      =   Get the file's info\n");
		return 0;
	} else if(fs::exists(argv[1]) && fs::is_regular_file(argv[1]) && !(argc > 1 && (!strcmp(argv[1],"--info") || !strcmp(argv[1],"--online")))){
		BASSI::PlayFile(argv[1]);
	} else if(!fs::exists(argv[1]) && BASSI::SupportedFormat(argv[1])){
		printf("\n Error: The file cannot be found. Please give me the correct file path.\n You can type \'conaud.exe -h\' for help.");
	} else if(argv[2]) {
		if(!strcmp(argv[2],"--playlist")){
			BASSI::PlayFromDir(argv[1]);
		} else if(!strcmp(argv[2],"--online")){
			BASSI::PlayOnline(argv[1]);
		} else if(!strcmp(argv[2],"--info")){
			MII::MAINFO MAI(argv[1]);
			cout << "\n          Filename : " << argv[1] << "\n\tAudio Size : " << MAI.GetAudioSize() << " Bytes\n      Audio Format : " << MAI.GetAudioFormat() << "\n    Audio Channels : " << MAI.GetAudioChannels() << "\n    Audio Bit Rate : " << MAI.GetAudioBitRate() << "\n";
		}
	}

	BASS_Free();
	return 0;
}
