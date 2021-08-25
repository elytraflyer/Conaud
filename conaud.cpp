#include "BassInterface.h"

#include <cstdio>
#include <string>
#include <iostream>

string FilePath;

int main(int argc, char *argv[]){
	const int i = argc;
	int device = -1;

	BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST, 2);

	if (!BASS_Init(-1,48000,0,0,NULL)){
		Error(" Can't initialize device");
	}

	// Check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
		printf("\t Conaud : Error : An incorrect version of BASS was loaded.\n\tExpected Bass %d",BASSVERSION);
		return 0;
	}

	// Parse the arguments
	if(!argv[1]){
		printf("\n No arguments provided. Type \'conaud.exe -h\' for help.\n");
	} else if(strcmp(argv[1],"-ld") == 0){
		ListDevices();
		return 0;
	} else if(strcmp(argv[1],"-v") == 0){
		ListVersions();
		return 0;
	} else if(strcmp(argv[1],"-h") == 0){
		printf("\n Usage: conaud.exe argument <file> option\n Arguments:\n"
			   "\t-ld         =   list devices\n\t-v          =   version\n\t-h          =   print this message\n\t<file>      =   play the file"
			   "\n Options:\n\t--playlist  =   play files from a directory\n");
		return 0;
	} else if(fs::exists(argv[1]) && fs::is_regular_file(argv[1])){
		FilePath = argv[1];
		PlayFile(argv[1],device);
	} else if(!fs::exists(argv[1]) && SupportedFormat(argv[1])){
		printf("\n Error: The file cannot be found. Please give me the correct file path.\n You can type \'conaud.exe -h\' for help.");
	} else if(argv[2]) {
		if(strcmp(argv[2],"--playlist") == 0){
			FilePath = argv[1];
			PlayFromDir(argv[1]);
		}
	}

	BASS_Free();
	return 0;
}
