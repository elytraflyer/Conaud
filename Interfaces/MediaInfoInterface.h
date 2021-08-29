#pragma once

#include "MediaInfoDLL.h"
#define MediaInfoNameSpace MediaInfoDLL

#include <string>

using namespace MediaInfoNameSpace;
using namespace std;

namespace MII{
    class MAINFO{
        private:
        MediaInfo MI;

        public:
        MAINFO(string Path){
            MI.Open(__T(Path));
        }

        ~MAINFO(){
            MI.Close();
        }

        inline string GetAudioSize(){
            MI.Option(__T("Inform"),__T("General;%FileSize%"));
            return MI.Inform();
        }

        inline string GetAudioFormat(){
            MI.Option(__T("Inform"),__T("Audio;%Format%"));
            return MI.Inform();
        }

        inline string GetAudioChannels(){
            MI.Option(__T("Inform"),__T("Audio;%Channels%"));
            return MI.Inform();
        }

        inline string GetAudioBitRate(){
            MI.Option(__T("Inform"),__T("Audio;%BitRate/String%"));
            return MI.Inform();
        }
    };
}