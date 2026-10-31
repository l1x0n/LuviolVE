#include <luviolVE/videoRenderer.hpp>

extern "C"
{
#include <libavutil/frame.h>
}

namespace lvlve
{

VideoRenderer::VideoRenderer(int width, int height)
    : videoW(width), videoH(height)
{
    float vertices[] 
    {
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f 
    };

    unsigned short indices[]
    {
        0, 1, 3,
        1, 2, 3
    };

    glCreateVertexArrays(1, &VAO);

    glCreateBuffers(1, &VBO);
    glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5*sizeof(float));

    glEnableVertexArrayAttrib(VAO, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);

    glEnableVertexArrayAttrib(VAO, 1);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3*sizeof(float));
    glVertexArrayAttribBinding(VAO, 1, 0);

    glCreateBuffers(1, &EBO);
    glNamedBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(VAO, EBO);

    const char* vertexShaderSource = 
    R"glsl(
        #version 460 core
        layout (location = 0) in vec3 vertPos;
        layout (location = 1) in vec2 texCoord;
        out vec2 coord;

        void main()
        {
            gl_Position = vec4(vertPos, 1.0f);
            coord = texCoord;
        }
    )glsl";

    const char* fragmentShaderSource = 
    R"glsl(
        #version 460 core
        layout (binding = 0) uniform sampler2D texY;
        layout (binding = 1) uniform sampler2D texU;
        layout (binding = 2) uniform sampler2D texV;
        in vec2 coord;
        out vec4 fragColor;

        void main()
        {
            vec3 yuv = vec3(
                texture(texY, coord).r,
                texture(texU, coord).r - 0.5f,
                texture(texV, coord).r - 0.5f
            );

            mat3 yuvToRgb = mat3(
                1.0f,    1.0f,   1.0f,
                0.0f,   -0.344f, 1.772f,
                1.402f, -0.714f, 0.0f    
            );

            vec3 rgb = yuvToRgb * yuv;
            fragColor = vec4(clamp(rgb, 0.0f, 1.0f), 1.0f);
        }
    )glsl";

    {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &textures[0]);
    glTextureStorage2D(textures[0], 1, GL_R8, videoW, videoH);
    glTextureParameteri(textures[0], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(textures[0], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(textures[0], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(textures[0], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glCreateTextures(GL_TEXTURE_2D, 1, &textures[1]);
    glTextureStorage2D(textures[1], 1, GL_R8, videoW / 2, videoH / 2);
    glTextureParameteri(textures[1], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(textures[1], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(textures[1], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(textures[1], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glCreateTextures(GL_TEXTURE_2D, 1, &textures[2]);
    glTextureStorage2D(textures[2], 1, GL_R8, videoW / 2, videoH / 2);
    glTextureParameteri(textures[2], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(textures[2], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(textures[2], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(textures[2], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glCreateTextures(GL_TEXTURE_2D, 1, &outTexture);
    glTextureStorage2D(outTexture, 1, GL_RGBA8, videoW, videoH);
    glTextureParameteri(outTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(outTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(outTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(outTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glCreateFramebuffers(1, &FBO);
    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, outTexture, 0);
}

VideoRenderer::~VideoRenderer()
{
    glDeleteFramebuffers(1, &FBO);
    glDeleteTextures(1, &outTexture);
    glDeleteTextures(3, textures);
    glDeleteProgram(shaderProgram);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void VideoRenderer::render(AVFrame* frame)
{
    if (!frame) return;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, frame->linesize[0]);
    glTextureSubImage2D(textures[0], 0, 0, 0, videoW, videoH, GL_RED,
        GL_UNSIGNED_BYTE, frame->data[0]);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, frame->linesize[1]);
    glTextureSubImage2D(textures[1], 0, 0, 0, videoW / 2, videoH / 2, GL_RED,
        GL_UNSIGNED_BYTE, frame->data[1]);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, frame->linesize[2]);
    glTextureSubImage2D(textures[2], 0, 0, 0, videoW / 2, videoH / 2, GL_RED,
        GL_UNSIGNED_BYTE, frame->data[2]);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport(0, 0, videoW, videoH);

    glUseProgram(shaderProgram);
    glBindTextureUnit(0, textures[0]);
    glBindTextureUnit(1, textures[1]);
    glBindTextureUnit(2, textures[2]);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}