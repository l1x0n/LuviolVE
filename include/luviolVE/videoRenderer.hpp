#pragma once

#include <glad/gl.h>

extern "C"
{
struct AVFrame;
}

namespace lvlve
{

class VideoRenderer
{
public:
    VideoRenderer(int width, int height);
    ~VideoRenderer();
    VideoRenderer(const VideoRenderer&) = delete;
    VideoRenderer& operator=(const VideoRenderer&) = delete;

    void render(AVFrame* frame);
    GLuint getOutputTexture() const { return outTexture; }
private:
    int videoW {0};
    int videoH {0};
    GLuint VAO {0}, VBO {0}, EBO {0}, FBO {0},
           textures[3] {0, 0, 0},
           outTexture {0},
           shaderProgram {0};     
};

}