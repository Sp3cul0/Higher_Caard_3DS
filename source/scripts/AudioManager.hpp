#pragma once
#include <3ds.h>
#include <string>
#include <map>
#include <cstring>

// Structure pour un canal audio
struct AudioChannel {
    ndspWaveBuf waveBuf;
    u8*         data    = nullptr;
    bool        playing = false;
    int         channel = 0;
};

class AudioManager {
public:
    std::map<std::string, AudioChannel> channels;
    int nextChannel = 0;
    bool initialized = false;

    AudioManager() {
        if (R_SUCCEEDED(ndspInit())) {
            ndspSetOutputMode(NDSP_OUTPUT_STEREO);
            initialized = true;
        }
    }

    ~AudioManager() {
        stopAll();
        for (auto& pair : channels) {
            if (pair.second.data)
                linearFree(pair.second.data);
        }
        if (initialized) ndspExit();
    }

    // Charge un fichier .wav depuis le romfs
    bool load(const std::string& key, const std::string& path) {
        if (!initialized) return false;

        FILE* f = fopen(path.c_str(), "rb");
        if (!f) {
            svcOutputDebugString("Failed to open audio file", 25);
            return false;
        }

        // Lire le header WAV (44 bytes)
        u8 header[44];
        fread(header, 1, 44, f);

        // Extraire infos du header WAV
        u16 channels_count = *(u16*)(header + 22);
        u32 sampleRate     = *(u32*)(header + 24);
        u16 bitsPerSample  = *(u16*)(header + 34);
        u32 dataSize       = *(u32*)(header + 40);

        // Allouer mémoire linéaire (obligatoire pour ndsp)
        u8* audioData = (u8*)linearAlloc(dataSize);
        if (!audioData) { fclose(f); return false; }

        fread(audioData, 1, dataSize, f);
        fclose(f);

        // Configurer le canal
        AudioChannel ch;
        ch.data    = audioData;
        ch.channel = nextChannel++;
        ch.playing = false;

        ndspChnReset(ch.channel);
        ndspChnSetInterp(ch.channel, NDSP_INTERP_LINEAR);
        ndspChnSetRate(ch.channel, (float)sampleRate);
        ndspChnSetFormat(ch.channel,
            channels_count == 2
                ? (bitsPerSample == 16 ? NDSP_FORMAT_STEREO_PCM16 : NDSP_FORMAT_STEREO_PCM8)
                : (bitsPerSample == 16 ? NDSP_FORMAT_MONO_PCM16   : NDSP_FORMAT_MONO_PCM8)
        );

        memset(&ch.waveBuf, 0, sizeof(ch.waveBuf));
        ch.waveBuf.data_vaddr = audioData;
        ch.waveBuf.nsamples   = dataSize / (channels_count * (bitsPerSample / 8));
        ch.waveBuf.looping    = false;

        DSP_FlushDataCache(audioData, dataSize);

        channels[key] = ch;
        return true;
    }

    void play(const std::string& key, bool loop = false) {
        auto it = channels.find(key);
        if (it == channels.end()) return;

        AudioChannel& ch = it->second;
        ch.waveBuf.looping = loop;
        ndspChnWaveBufAdd(ch.channel, &ch.waveBuf);
        ch.playing = true;
    }

    void stop(const std::string& key) {
        auto it = channels.find(key);
        if (it == channels.end()) return;
        ndspChnWaveBufClear(it->second.channel);
        it->second.playing = false;
    }

    void stopAll() {
        for (auto& pair : channels)
            ndspChnWaveBufClear(pair.second.channel);
    }

    void setVolume(const std::string& key, float volume) {
        auto it = channels.find(key);
        if (it == channels.end()) return;
        float mix[12] = {0};
        mix[0] = mix[1] = volume; // gauche + droite
        ndspChnSetMix(it->second.channel, mix);
    }

    bool isPlaying(const std::string& key) {
        auto it = channels.find(key);
        if (it == channels.end()) return false;
        return ndspChnIsPlaying(it->second.channel);
    }
};