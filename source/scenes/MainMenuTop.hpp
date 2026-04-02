#pragma once
#include "../scripts/UIAlign.hpp"
#include "../scripts/Coroutine.hpp"

class MainMenuTop : public Scene
{
public:
    Coroutine blinkCoroutine;
    bool logoVisible = true;

    float angle = 0.0f;
    float speed = 0.0f;
    bool rotating = false;

    MainMenuTop()
    {
        blinkCoroutine.start(0.5f);
        audioManager.play("menu", false);
    }

    void update() override
    {
        blinkCoroutine.update();
        if (blinkCoroutine.finished())
        {
            logoVisible = !logoVisible;
            blinkCoroutine.start(0.5f);
        }

        touchPosition touch;
        hidTouchRead(&touch);

        if (hidKeysHeld() & KEY_TOUCH)
        {
            rotating = true;
            speed = 0.15f;
        }

        if (rotating)
        {
            // Ease out : ralentit quand on approche de 2*PI
            float remaining = (2.0f * M_PI) - angle;
            speed = 0.02f + 0.13f * (remaining / (2.0f * M_PI)); // décélère progressivement

            angle += speed;

            if (angle >= 2.0f * M_PI)
            {
                angle = 0.0f;
                speed = 0.0f;
                rotating = false;
            }
        }
    }

    void draw(Renderer &r) override
    {
        r.drawImage("background_top", 0, 0, 0);

        // ↓ ICI dans draw(), remplace l'ancien drawImageRotated
        float scaleX = 0.5f * cosf(angle);
        r.drawImageRotated("menu", 0, 135 + 64, 70 + 48, scaleX, 0.4f, 0.0f);
        r.drawText("By Sp3culo & Le_Sims", 125, 170, 0.6f, C2D_Color32(255, 255, 255, 255));

        if (logoVisible)
        {
            r.drawText("Touch the screen of your 3DS", 100, 218, 0.6f, C2D_Color32(255, 255, 255, 255));
        }
    }
};