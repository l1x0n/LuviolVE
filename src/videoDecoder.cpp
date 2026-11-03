#include <luviolVE/videoDecoder.hpp>
#include <stdexcept>
#include <SDL3/SDL_messagebox.h>

namespace lvlve
{

VideoDecoder::VideoDecoder(const std::string& videoPath)
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

VideoDecoder::~VideoDecoder()
{
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
}

AVFrame* VideoDecoder::getNextFrame() 
{
    while (true)
    {   
        int ret = avcodec_receive_frame(codecContext, frame);
        if (ret == 0)
            return frame;
        if (ret != AVERROR(EAGAIN))
            return nullptr;

        ret = av_read_frame(formatContext, packet);
        if (ret < 0)
        {
            avcodec_send_packet(codecContext, nullptr);
            continue;
        }
        if (packet->stream_index == videoStream->index)
            avcodec_send_packet(codecContext, packet);
        av_packet_unref(packet);      
    }
}

}