#ifndef ENGINE_H
#define ENGINE_H

#include "Window.h"

class Engine
{
public:
    ~Engine();

    static Engine* Instance();

    void UpdateEngine();

private:
    Engine();

    static Engine* engineInstance;
};

#endif