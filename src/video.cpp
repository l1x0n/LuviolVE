#include <luviolVE/video.hpp>
#include <SDL3/SDL_messagebox.h>
#include <stdexcept>

namespace lvlve
{

Video::Video(const std::string& videoPath)
{
    if (avformat_open_input(&formatContext, videoPath.c_str() , nullptr, nullptr) < 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Error",
            "Video open falied", nullptr);
        throw std::runtime_error("Video open failed: " + videoPath);   
    }    
    if (avformat_find_stream_info(formatContext, nullptr) < 0)
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Error",
            "Find video info failed", nullptr);
    
    
    videoStream = formatContext->streams[av_find_best_stream(formatContext,
        AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0)];
    
    {
    const AVCodec* codec = avcodec_find_decoder(videoStream->codecpar->codec_id);
    codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext, videoStream->codecpar);
    codecContext->thread_count = 0;
    avcodec_open2(codecContext, codec, nullptr);
    }

    packet = av_packet_alloc();
    frame = av_frame_alloc();
}

Video::~Video()
{
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
}

AVFrame* Video::getNextFrame() 
{
    while (av_read_frame(formatContext, packet) >= 0)
    {
        if (packet->stream_index == videoStream->index)
        {
            if (avcodec_send_packet(codecContext, packet) >= 0)
            {
                if (avcodec_receive_frame(codecContext, frame) >= 0)
                {
                    av_packet_unref(packet);    
                    return frame; 
                }
                    
            } 
        }
        av_packet_unref(packet);
    }
    return nullptr;
}

}