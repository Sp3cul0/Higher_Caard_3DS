#pragma once
#include "./Renderer.hpp"
#include "./AudioManager.hpp"

class Scene
{
public:
    virtual ~Scene() {}
    virtual void update() = 0;
    virtual void draw(Renderer &r) = 0;
};

class SceneManager
{
public:
    Scene *current = nullptr;
    Scene *next = nullptr;
    float fade = 0.0f;
    bool fadingOut = false;
    float eyeOffset = 0.0f;

    // Dimensions de l'écran associé à ce manager
    float screenW = 400.0f;
    float screenH = 240.0f;

    // Constructeur avec dimensions (défaut = écran du haut)
    SceneManager(float w = 400.0f, float h = 240.0f)
        : screenW(w), screenH(h) {}

    void set(Scene *scene)
    {
        next = scene;
        fadingOut = true;
        fade = 0.0f;
    }

    void setEyeOffset(float offset) { eyeOffset = offset; }

    void update()
    {
        if (fadingOut)
        {
            fade += 0.05f;
            if (fade >= 1.0f)
            {
                fade = 1.0f;
                fadingOut = false;
                delete current;
                current = next;
                next = nullptr;
            }
        }
        else if (fade > 0.0f)
        {
            fade -= 0.05f;
            if (fade < 0.0f)
                fade = 0.0f;
        }

        if (current)
            current->update();
    }

    void draw(Renderer &r)
    {
        if (current)
        {
            // Applique le décalage 3D stéréoscopique
            C2D_ViewTranslate(eyeOffset, 0.0f);
            current->draw(r);
            C2D_ViewReset(); // Reset après le draw !
        }

        if (fade > 0.0f)
        {
            u8 alpha = (u8)(fade * 255);
            C2D_DrawRectSolid(0, 0, 0.5f, screenW, screenH,
                              C2D_Color32(0, 0, 0, alpha));
        }
    }
};

// Deux managers — un par écran
extern SceneManager sceneManagerTop; // 400x240
extern SceneManager sceneManagerBot; // 320x240
extern AudioManager audioManager;

Scene *createScene(const char *name);
void goToTop(const char *name);
void goToBot(const char *name);