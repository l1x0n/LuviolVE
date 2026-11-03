#pragma once

#include <SDL3/SDL_timer.h>

namespace lvlve
{

class PlaybackClock 
{
public:
    void play() 
    { 
        if (!playing) 
        {
            anchorWall = now();
            playing = true;
        }
    }
    
    void pause()
    {
        if (playing)
        {
            anchorMedia = time();
            playing = false; 
        }
    }

    void seek(double t)
    {
        anchorMedia = t;
        anchorWall = now();
    }

    double time() const
    {
        if (playing)
            return anchorMedia + (now() - anchorWall);
        else
            return anchorMedia;
    }

    bool isPlaying() const { return playing ;}
private:
    double now() const { return SDL_GetTicksNS() / 1e9; }
    double anchorMedia = 0.0;
    double anchorWall = 0.0;
    bool playing = false;    
};

}