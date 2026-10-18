// audio.h
#pragma once
#include <iostream>
#include <string>
#include "miniaudio.h"
using namespace std;
// ... Audio class
class Audio{
    ma_engine engine;
    ma_sound sound;
    bool engineOk=false, soundLoaded=false;
public:
    Audio()
    {
        engineOk=(ma_engine_init(NULL,&engine)==MA_SUCCESS);
        if(!engineOk) cout<<"Audio engine failed\n";
    }
    void stop()
    {
        if(soundLoaded){ ma_sound_uninit(&sound); soundLoaded=false; }
    }
    void pause()
    {
        ma_sound_stop(&sound);
    }
    void resume()
    {
        ma_sound_start(&sound);
    }
    void playFile(const string& path)
    {
        if(!engineOk) return;
        stop();                                   // stop the old song first
        if(path.empty()){ cout<<"(no file for this song)\n"; return; }
        if(ma_sound_init_from_file(&engine,path.c_str(),0,NULL,NULL,&sound)!=MA_SUCCESS)
        { cout<<"Could not open "<<path<<"\n"; return; }
        soundLoaded=true;
        ma_sound_start(&sound);
    }
    ~Audio(){ stop(); if(engineOk) ma_engine_uninit(&engine); }
};
