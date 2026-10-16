#pragma once

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/avutil.h>
}

namespace lvlve
{

class VideoReader
{
public:
    VideoReader();
    ~VideoReader();

private:
    AVFormatContext* formatContext = nullptr;
};

}