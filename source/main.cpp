#include <3ds.h>
#include <citro2d.h>
#include <thread>
#include <chrono>
#include "scripts/SceneManager.hpp"
#include "scenes/MainMenuTop.hpp"
#include "scenes/MainMenuBot.hpp"
#include <cstring>
#include <vector>
#include "scripts/Coroutine.hpp"
#include "scripts/AudioManager.hpp"

C3D_RenderTarget *top;
C3D_RenderTarget *bot;

C3D_RenderTarget *topLeft;
C3D_RenderTarget *topRight;

float slider = osGet3DSliderState();
float offset = slider * 15.0f;

SceneManager sceneManagerTop(400.0f, 240.0f);
SceneManager sceneManagerBot(320.0f, 240.0f);
AudioManager audioManager;

Scene *createSceneTop(const char *name)
{
    if (strcmp(name, "MainMenuTop")   == 0) return new MainMenuTop();
    if (strcmp(name, "MainMenuBot")   == 0) return new MainMenuBot();
    //if (strcmp(name, "Credits")    == 0) return new Credits();
    return nullptr;
}

Scene *createSceneBot(const char *name)
{
    if (strcmp(name, "MainMenuTop")   == 0) return new MainMenuTop();
    if (strcmp(name, "MainMenuBot")   == 0) return new MainMenuBot();
    //if (strcmp(name, "Credits")    == 0) return new Credits();
    return nullptr;
}

void goToTop(const char *name)
{
    sceneManagerTop.set(createSceneTop(name));
}

void goToBot(const char *name)
{
    sceneManagerBot.set(createSceneBot(name));
}

int main(int argc, char **argv)
{
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bot = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    topLeft  = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    topRight = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);

    Renderer renderer;
    renderer.loadSheet("level",      "romfs:/assets/bglevel_1.t3x");
    renderer.loadSheet("menu",       "romfs:/assets/logo.t3x");
    renderer.loadSheet("background_top", "romfs:/assets/output_top.t3x");
    renderer.loadSheet("background_bot", "romfs:/assets/output_bottom.t3x");
    renderer.loadSheet("steam", "romfs:/assets/steam.t3x");
    renderer.loadSheet("discord", "romfs:/assets/discord.t3x");

    goToTop("MainMenuTop");
    goToBot("MainMenuBot");

    while (aptMainLoop())
    {
        hidScanInput();

        sceneManagerTop.update();
        sceneManagerBot.update();

        float slider = osGet3DSliderState();
        float offset = slider * 6.0f;

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        renderer.beginFrame();

        C2D_TargetClear(topLeft, C2D_Color32(0, 0, 0, 255));
        C2D_SceneBegin(topLeft);
        sceneManagerTop.setEyeOffset(-offset); // décale à gauche
        sceneManagerTop.draw(renderer);

        C2D_TargetClear(topRight, C2D_Color32(0, 0, 0, 255));
        C2D_SceneBegin(topRight);
        sceneManagerTop.setEyeOffset(+offset); // décale à droite
        sceneManagerTop.draw(renderer);


        C2D_TargetClear(bot, C2D_Color32(0, 0, 0, 255));
        C2D_SceneBegin(bot);
        sceneManagerBot.draw(renderer);

        C3D_FrameEnd(0);
    }

    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}