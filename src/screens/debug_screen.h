#ifndef NACHO_DEBUG_SCREEN_H
#define NACHO_DEBUG_SCREEN_H

#include "../opengl/screen.h"
#include "../core/chip8.h"

class DebugScreen: public Screen {
public:
    explicit DebugScreen(Chip8 &chip8);
    void update() override;
private:
    Chip8 &chip8;

    bool showDemo = false;
    int counter = 0;
};


#endif //NACHO_DEBUG_SCREEN_H
