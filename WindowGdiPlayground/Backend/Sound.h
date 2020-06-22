#pragma once
#include <Windows.h>
#include "Lists/AssetsList.h"

#pragma comment(lib, "winmm.lib")

class Sound {
public:
	static void openMP3();
	static void playOnRepeatMP3();
	static void closeMP3();
};

//To play* .mp3:
 //   mciSendString(L"play mp3", NULL, 0, NULL);
    //To playand wait until the* .mp3 has finished playing :
//    mciSendString(L"play mp3 wait", NULL, 0, NULL);
    //To replay(play again from start) the* .mp3:
//    mciSendString(L"play mp3 from 0", NULL, 0, NULL);
    //To replayand wait until the* .mp3 has finished playing :
//    mciSendString(L"play mp3 from 0 wait", NULL, 0, NULL);
    //To play the* .mp3and replay it every time it ends like a loop :
//    mciSendString(L"play mp3 repeat", NULL, 0, NULL);
//To pause the* .mp3 in middle :
//    mciSendString(L"pause mp3", NULL, 0, NULL);
    //and to resume it :
//    mciSendString(L"resume mp3", NULL, 0, NULL);
    //To stop it in middle :
//    mciSendString(L"stop mp3", NULL, 0, NULL);
    //close file
//    mciSendString(L"close mp3", NULL, 0, NULL);