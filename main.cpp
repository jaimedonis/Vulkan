#include "my_application.h"

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;

int main(int argc, char *argv[])
{    
    MyApplication app;

    EngineConfig config;
    config._window_width = WINDOW_WIDTH;
    config._window_height = WINDOW_HEIGHT;

    app.Initialize(config);

    while(app.IsRunning()) {        
        app.Update();
    }

    app.Deinitialize();

    return 0;
}
