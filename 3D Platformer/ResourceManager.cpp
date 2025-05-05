#include "ResourceManager.h"
#include "Engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Instantiate static variables
map<int, Shader> ResourceManager::shaders;

Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, int enum_)
{
    // Create shader object from source code
    shaders[enum_].InitializeShader(vShaderFile, fShaderFile);

    return shaders[enum_];
}

Shader ResourceManager::GetShader(int enum_)
{
    if (shaders.find(enum_) == shaders.end())
    {
        cout << "This mapped shader enum is not found: " << enum_ << endl;
    }

    return shaders[enum_];
}

void ResourceManager::Clear()
{
    // Delete all shaders properly	
    for (pair<int, Shader> iter : shaders)
    {
        iter.second.~Shader();
    }
}
