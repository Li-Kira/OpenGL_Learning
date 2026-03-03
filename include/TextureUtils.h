#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <string>
#include <iostream>
#include <unordered_map>
#include "Logger.h"

class TextureUtils
{
private:
    static std::unordered_map<std::string, unsigned int> textureCache;

    static std::string generateCacheKey(const std::string& path, bool gammaCorrection) {
        return path + (gammaCorrection ? "_gamma" : "_nogamma");
    }
public:
    static unsigned int loadTexture(const std::string& path, 
                                   bool flipVertically = true, 
                                   GLenum wrapS = GL_REPEAT, 
                                   GLenum wrapT = GL_REPEAT,
                                   bool gammaCorrection = false) 
    {
        // 生成包含gamma校正信息的缓存键
        std::string cacheKey = generateCacheKey(path, gammaCorrection);
        
        // 检查缓存
        if (textureCache.find(cacheKey) != textureCache.end()) {
            Logger::log(Logger::Level::Info, "Texture loaded from cache: ", path, 
                       gammaCorrection ? " (gamma corrected)" : " (no gamma correction)");
            return textureCache[cacheKey];
        }

        stbi_set_flip_vertically_on_load(flipVertically);
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        
        if (!data) {
            Logger::log(Logger::Level::Error, "Failed to load texture: ", path);
            return 0;
        }

        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // 设置纹理参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 确定纹理格式，考虑gamma校正
        GLenum internalFormat;
        GLenum dataFormat;
        
        if (nrChannels == 1) {
            internalFormat = dataFormat = GL_RED;
        }
        else if (nrChannels == 3) {
            if (gammaCorrection) {
                internalFormat = GL_SRGB;  // gamma校正的RGB格式
            } else {
                internalFormat = GL_RGB;   // 线性RGB格式
            }
            dataFormat = GL_RGB;
        }
        else if (nrChannels == 4) {
            if (gammaCorrection) {
                internalFormat = GL_SRGB_ALPHA;  // gamma校正的RGBA格式
            } else {
                internalFormat = GL_RGBA;        // 线性RGBA格式
            }
            dataFormat = GL_RGBA;
        } else {
            // 不支持其他通道数
            Logger::log(Logger::Level::Error, "Unsupported number of channels: ", nrChannels, " in texture: ", path);
            stbi_image_free(data);
            glDeleteTextures(1, &textureID);
            return 0;
        }

        // 上传纹理数据
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        
        Logger::log(Logger::Level::Info, "Successfully loaded texture: ", path, 
                   " (", width, "x", height, ", channels: ", nrChannels, 
                   gammaCorrection ? ", gamma corrected)" : ", linear)");
        
        // 添加到缓存
        textureCache[cacheKey] = textureID;
        return textureID;
    }

    static unsigned int loadTextureHDR(const std::string& path, 
                                      bool flipVertically, 
                                      GLenum wrapS, 
                                      GLenum wrapT) 
    {
        stbi_set_flip_vertically_on_load(flipVertically);
        int width, height, nrComponents;
        float* data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
        
        if (!data) {
            Logger::log(Logger::Level::Error, "Failed to load HDR texture: ", path);
            return 0;
        }

        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // HDR纹理的特殊参数设置
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 确定HDR纹理格式
        GLenum internalFormat;
        GLenum dataFormat;
        
        if (nrComponents == 1) {
            internalFormat = GL_R16F;
            dataFormat = GL_RED;
        }
        else if (nrComponents == 3) {
            internalFormat = GL_RGB16F;
            dataFormat = GL_RGB;
        }
        else if (nrComponents == 4) {
            internalFormat = GL_RGBA16F;
            dataFormat = GL_RGBA;
        } else {
            Logger::log(Logger::Level::Error, "Unsupported number of components: ", nrComponents, " in HDR texture: ", path);
            stbi_image_free(data);
            glDeleteTextures(1, &textureID);
            return 0;
        }

        // 上传HDR纹理数据（使用GL_FLOAT）
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_FLOAT, data);
        
        // HDR纹理通常不生成mipmap，除非需要
        // glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        
        Logger::log(Logger::Level::Info, "Successfully loaded HDR texture: ", path, 
                   " (", width, "x", height, ", components: ", nrComponents, ", 16-bit float)");
        
        return textureID;
    }

    static void deleteTexture(unsigned int textureID)
    {
        glDeleteTextures(1, &textureID);

        for (auto it = textureCache.begin(); it != textureCache.end(); )
        {
            if (it->second == textureID)
            {
                it = textureCache.erase(it);
            }
            else
            {
                ++it;
            }
        }        

    }

    static void clearCache()
    {
        for(auto& pair : textureCache)
        {
            glDeleteTextures(1, &pair.second);
        }
        textureCache.clear();
         Logger::log(Logger::Level::Info, "Texture cache cleared");
    }

    static void bindTexture(unsigned int textureID, GLenum textureUnit = GL_TEXTURE0)
    {
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    // 获取纹理缓存信息
    static size_t getCacheSize() {
        return textureCache.size();
    }
    
    // 打印缓存信息
    static void printCacheInfo() {
        Logger::log(Logger::Level::Info, "Texture cache contains ", textureCache.size(), " textures:");
        for (const auto& pair : textureCache) {
            Logger::log(Logger::Level::Info, "  ", pair.first);
        }
    }
};

// 初始化静态成员
std::unordered_map<std::string, unsigned int> TextureUtils::textureCache;
