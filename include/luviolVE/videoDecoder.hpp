#pragma once

#include <string>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
}

namespace lvlve
{

class VideoDecoder
{
public:
    VideoDecoder(const std::string& videoPath);
    ~VideoDecoder();
    VideoDecoder(const VideoDecoder&) = delete;
    VideoDecoder& operator=(const VideoDecoder&) = delete;

    int getVideoWidth() const { return codecContext->width; }
    int getVideoHeight() const { return codecContext->height; }
    AVRational getTimeBase() const { return videoStream->time_base; }
    AVFrame* getNextFrame();

private:
    AVFormatContext* formatContext = nullptr;
    AVStream* videoStream = nullptr;
    AVCodecContext* codecContext = nullptr;
    AVPacket* packet = nullptr;
    AVFrame* frame = nullptr;
    
};

}