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
        {
            if (target_ts != AV_NOPTS_VALUE)
            {           
                int64_t ts = frame->best_effort_timestamp;
                if (ts != AV_NOPTS_VALUE && ts < target_ts)
                    continue;
                target_ts = AV_NOPTS_VALUE;
                return frame;
            }
            return frame;
        }    
        if (ret != AVERROR(EAGAIN))
        {
            if (ret == AVERROR_EOF)
                return nullptr;
            if (ret < 0)
            {
                char errbuf[AV_ERROR_MAX_STRING_SIZE];
                av_strerror(ret, errbuf, sizeof(errbuf));
                // logging (future function)
                return nullptr;  
            }  
        }              
        ret = av_read_frame(formatContext, packet);
        if (ret < 0)
        {   
            if (ret == AVERROR_EOF)
                avcodec_send_packet(codecContext, nullptr);
            else
            {
                char errbuf[AV_ERROR_MAX_STRING_SIZE];
                av_strerror(ret, errbuf, sizeof(errbuf));
                // logging (future function)
            }
            continue;
        }
        if (packet->stream_index == videoStream->index)
        {
            ret = avcodec_send_packet(codecContext, packet);
            if (ret < 0)
            {
                char errbuf[AV_ERROR_MAX_STRING_SIZE];
                av_strerror(ret, errbuf, sizeof(errbuf));
                // logging (future function)
            } 
        } 
        av_packet_unref(packet);      
    }
}

void VideoDecoder::seek(double t)
{
    target_ts = av_rescale_q((static_cast<int64_t>(t * AV_TIME_BASE)), AV_TIME_BASE_Q, videoStream->time_base);
    int ret = avformat_seek_file(formatContext, videoStream->index, INT64_MIN, target_ts, target_ts, 0);
    if (ret < 0)
        return;
    avcodec_flush_buffers(codecContext);
}

}