#pragma once

#include <luviolVE/videoDecoder.hpp>
#include <luviolVE/videoRenderer.hpp>
#include <luviolVE/playbackClock.hpp>
#include <memory>

namespace lvlve
{

struct PlayerState
{   
    GLuint texture = 0;
    int    videoW = 0;
    int    videoH = 0;
    bool   playing = false;
};

class Player
{
public:
    Player();

    void open(const std::string& path);
    void update();
    void togglePlay();
    void seek(double t);

    PlayerState getState() const 
    {   
        if (!decoder || !renderer)
            return {};

        return
        {
            renderer->getOutputTexture(),
            decoder->getVideoWidth(),
            decoder->getVideoHeight(),
            clock.isPlaying()
        };
    }
private:
    void play();
    void pause();
    std::unique_ptr<VideoDecoder> decoder;
    std::unique_ptr<VideoRenderer> renderer;
    PlaybackClock clock;
    AVFrame* currentFrame = nullptr;
    bool ended = false;
};

}
