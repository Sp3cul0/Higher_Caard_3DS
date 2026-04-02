#pragma once
#include <3ds.h>
#include <citro2d.h>
#include <string>
#include <map>

class Renderer
{
public:
    C2D_TextBuf buf;
    C2D_Font font = nullptr;
    std::map<std::string, C2D_SpriteSheet> sheets;
    bool active = true;

    Renderer()
    {
        buf = C2D_TextBufNew(4096);
        romfsInit();
        font = C2D_FontLoad("romfs:/fonts/font.bcfnt");
        if (!font)
            font = C2D_FontLoadSystem(CFG_REGION_EUR);
    }

    ~Renderer()
    {
        if (font)
            C2D_FontFree(font);
        for (auto &pair : sheets)
            C2D_SpriteSheetFree(pair.second);
        C2D_TextBufDelete(buf);
        romfsExit();
    }

    void loadSheet(const std::string &key, const std::string &path)
    {
        C2D_SpriteSheet sheet = C2D_SpriteSheetLoad(path.c_str());
        if (sheet)
            sheets[key] = sheet;
        else
            svcOutputDebugString("Failed to load sheet", 20);
    }

    void beginFrame() { C2D_TextBufClear(buf); }

    void drawRect(float x, float y, float w, float h, u32 color)
    {
        C2D_DrawRectSolid(x, y, 0, w, h, color);
    }

    void drawImage(const std::string &key, int index, float x, float y, float sizeX = 1.0f, float sizeY = 1.0f)
    {
        auto it = sheets.find(key);
        if (it == sheets.end())
            return;

        C2D_Sprite sprite;
        C2D_SpriteFromSheet(&sprite, it->second, index);
        C2D_SpriteSetCenter(&sprite, 0.0f, 0.0f);
        C2D_SpriteSetPos(&sprite, x, y);
        C2D_SpriteSetScale(&sprite, sizeX, sizeY);
        C2D_DrawSprite(&sprite);
    }

    void drawText(const std::string &str, float x, float y, float scale, u32 color)
    {
        C2D_Text txt;
        C2D_TextFontParse(&txt, font, buf, str.c_str());
        C2D_TextOptimize(&txt);
        C2D_DrawText(&txt, C2D_WithColor, x, y, 0, scale, scale, color);
    }

    void drawTextUI(const std::string &str, float x, float y, float scale, u32 color)
    {
        C2D_Text txt;
        C2D_TextFontParse(&txt, font, buf, str.c_str());
        C2D_TextOptimize(&txt);
        C2D_DrawText(&txt, C2D_WithColor, x, y, 0, scale, scale, color);
    }

    C2D_Image getImage(const std::string &key, int index)
    {
        auto it = sheets.find(key);
        if (it != sheets.end())
            return C2D_SpriteSheetGetImage(it->second, index);
        return C2D_Image{};
    }

    void drawImageRotated(const std::string &key, int index, float x, float y, float sizeX, float sizeY, float angle)
    {
        auto it = sheets.find(key);
        if (it == sheets.end())
            return;

        C2D_Sprite sprite;
        C2D_SpriteFromSheet(&sprite, it->second, index);
        C2D_SpriteSetCenter(&sprite, 0.5f, 0.5f); // pivot au centre
        C2D_SpriteSetPos(&sprite, x, y);
        C2D_SpriteSetScale(&sprite, sizeX, sizeY);
        C2D_SpriteSetRotation(&sprite, angle);
        C2D_DrawSprite(&sprite);
    }
};