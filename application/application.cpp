#include "application.h"

#include "../diagnostics/logger.h"

#include "SDL2/SDL.h"
#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>

namespace {
SDL_Window* g_sdl_window = nullptr;
}
SDL_Window* InitializeSDL(int window_width, int window_height);


Application::Application()
    : _gl_context(nullptr),
      _is_running(false)
{
}

bool Application::Initialize(const EngineConfig& config)
{
    // == initialize windowing system ==

    g_sdl_window = InitializeSDL(config._window_width, config._window_height);
    if (g_sdl_window == nullptr) {
        std::cout << "Unable to initialize OpenGL, your must create a window context first" << std::endl;
        return false;
    }

    // == initialize opengl ==

    _gl_context = SDL_GL_CreateContext(g_sdl_window);
    if (_gl_context == nullptr) {
        std::cout << "Unable to create GL context" << std::endl;
        return false;
    }

    // disable vsync
    SDL_GL_SetSwapInterval(0);

    // set depth buffer to the context
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "Unable to initialize GLEW" << std::endl;
        return false;
    }
    while ((err = glGetError()) != GL_NO_ERROR)

    GL_INVOKE(glDepthFunc(GL_LEQUAL))
    GL_INVOKE(glClearDepthf( 1.0f ))
    GL_INVOKE(glDepthRange(1.0f, -1.0f))
    GL_INVOKE(glEnable(GL_DEPTH_TEST))

    GL_INVOKE(glEnable(GL_BLEND))
    GL_INVOKE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

    GL_INVOKE(glEnable(GL_CULL_FACE))
    GL_INVOKE(glCullFace(GL_BACK))
    GL_INVOKE(glFrontFace(GL_CCW))
    GL_INVOKE(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS))

    GL_INVOKE(glClearColor(0.0f, 0.0f, 0.0f, 1.0f))

    // == setup the main engine ==

    Engine::GetInstance()->OnSetup(config);    

    _is_running = true;

    // == allow the app side to be setup ==
    OnSetup();

    return true;
}

bool Application::Deinitialize()
{
    bool is_success = true;

    // allow the app side to cleanup gracefully
    OnTeardown();

    // cleanup the engine
    Engine::GetInstance()->OnTeardown();

    // cleanup the windowing system
    if( _gl_context != nullptr ) {
        SDL_GL_DeleteContext(_gl_context);
        _gl_context = nullptr;
    } else {
        is_success = false;
    }

    if( g_sdl_window != nullptr ) {
        SDL_DestroyWindow(g_sdl_window);
        g_sdl_window = nullptr;
    } else {
        is_success = false;
    }

    return is_success;
}

void Application::Update()
{
    GL_INVOKE(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

    // poll the windowing subsystem
    SDL_Event event;
    while( SDL_PollEvent(&event) )
    {
        switch (event.type)
        {
        case SDL_QUIT:
            //app->RequestExit();
            _is_running = false;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            //app->HandleInput(event.key.keysym, event.type, event.key.repeat, currentTime, deltaTime);
            break;
        case SDL_WINDOWEVENT:
            //app->HandleWindowEvent(static_cast<SDL_WindowEventID>(event.window.event), event.window.data1, event.window.data2, currentTime);
            break;
        default:
            break;
        }
    }

    // engine update tick
    Engine::GetInstance()->OnUpdate();    

    // application side update callback
    OnUpdate();

    SDL_GL_SwapWindow(g_sdl_window);
}

SDL_Window* InitializeSDL(int window_width, int window_height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Unable to initialize SDL" << std::endl;
        return nullptr;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

    SDL_Window* sdl_window = SDL_CreateWindow("application",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       window_width,
                                       window_height,
                                       SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (sdl_window == nullptr) {
        std::cout << "Unable to create windowing system" << std::endl;
    }

    return sdl_window;
}
