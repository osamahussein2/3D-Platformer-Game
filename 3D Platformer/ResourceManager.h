#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <iostream>
#include <sstream>
#include <fstream>

#include <glad/glad.h>

#include "Shader.h"

class ResourceManager
{
public:

    // resource storage
    static map<int, Shader> shaders;

    // Loads (and generates) a shader program from file loading vertex and fragment shader's source code
    static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, int enum_);

    // Retrieves a stored sader
    static Shader GetShader(int enum_);

    // Properly de-allocates all loaded resources
    static void Clear();

private:
    ResourceManager() { }
};

#endif //RESOURCE_MANAGER_H