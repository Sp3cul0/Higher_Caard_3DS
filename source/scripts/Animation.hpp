#pragma once

class Animation {
public:
    int     totalFrames;  // nombre de frames dans l'animation
    float   speed;        // frames par update (ex: 0.15 = lent)
    float   current;      // frame courante (float pour la vitesse)
    bool    loop;         // boucle ou s'arrête à la dernière frame

    Animation(int totalFrames, float speed, bool loop = true)
        : totalFrames(totalFrames), speed(speed), current(0), loop(loop) {}

    void update() {
        current += speed;
        if (current >= totalFrames) {
            current = loop ? 0 : totalFrames - 1;
        }
    }

    int frame() const { return (int)current; }
    bool finished() const { return !loop && current >= totalFrames - 1; }
    void reset() { current = 0; }
};