#pragma once

#include "FrameData.hpp"
#include <vector>

class GJBaseGameLayer;

namespace bot {

struct InputEvent {
    int frame = 0;
    int player = 1;
    int button = 0;
    bool push = true;
};

extern std::vector<InputEvent> g_events;
extern bool g_recording;
extern bool g_playing;

void startRecording();
void stopRecording();
void startPlayback();
void stopPlayback();
void clearEvents();
void rewindPlayback();

void recordPush(int player, int button, int frame);
void recordRelease(int player, int button, int frame);
void tick(GJBaseGameLayer* layer, int frame);

std::vector<FrameWindow> analyze();
int eventCount();

}
