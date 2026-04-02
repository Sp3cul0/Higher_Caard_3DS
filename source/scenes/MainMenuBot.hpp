#pragma once
#include "../scripts/SceneManager.hpp"
#include "../scripts/UIAlign.hpp"
#include "../scripts/Coroutine.hpp"

class MainMenuBot : public Scene {
public:
    Coroutine blinkCoroutine;
    bool logoVisible = true;

    MainMenuBot() {
        blinkCoroutine.start(0.5f);
        audioManager.play("menu", false);
    }

    void update() override {
        //if (hidKeysDown() & KEY_A) goToTop("Level");

        blinkCoroutine.update();

        if (blinkCoroutine.finished()) {
            logoVisible = !logoVisible;
            blinkCoroutine.start(0.5f);
        }
    }

    void draw(Renderer& r) override {
        r.drawImage("background_bot", 0, 0,  0,  1.0f, 1.0f);
        r.drawImage("steam",0, 20, 55, 0.3f, 0.3f);
        r.drawImage("discord",0, 170, 55, 0.4f, 0.38f);
        
        r.drawText("Steam Page", 52, 190, 0.5f, C2D_Color32(255, 255, 255, 255));
        r.drawText("Join our community", 182, 190, 0.5f, C2D_Color32(255, 255, 255, 255));
    }
};