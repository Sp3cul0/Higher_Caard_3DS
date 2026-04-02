#pragma once

class Coroutine {
public:
    bool  active = false;
    int   totalFrames = 0;
    int   currentFrame = 0;

    void start(float seconds) {
        active       = true;
        totalFrames  = (int)(seconds * 60.0f);
        currentFrame = 0;
    }

    void update() {
        if (!active) return;
        currentFrame++;
        if (currentFrame >= totalFrames) {
            active = false;
        }
    }

    float progress() {
        if (totalFrames == 0) return 1.0f;
        return (float)currentFrame / (float)totalFrames;
    }

    bool finished() { return !active && currentFrame >= totalFrames; }
    void reset()    { active = false; currentFrame = 0; }
};