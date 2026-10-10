#include "BotController.hpp"

#include <Geode/Geode.hpp>
#include <algorithm>
#include <set>
#include <utility>

using namespace geode::prelude;

namespace bot {

std::vector<InputEvent> g_events;
bool g_recording = false;
bool g_playing = false;

static size_t s_playIndex = 0;

void startRecording() {
    g_events.clear();
    s_playIndex = 0;
    g_playing = false;
    g_recording = true;
    Notification::create("Recording inputs...", NotificationIcon::Info)->show();
}

void stopRecording() {
    g_recording = false;
    Notification::create(
        fmt::format("Recorded {} input(s)", g_events.size()),
        NotificationIcon::Success
    )->show();
}

void startPlayback() {
    if (g_events.empty()) {
        Notification::create("Nothing to play. Record or import first.", NotificationIcon::Warning)->show();
        return;
    }
    g_recording = false;
    g_playing = true;
    s_playIndex = 0;
    Notification::create("Playback started", NotificationIcon::Success)->show();
}

void stopPlayback() {
    g_playing = false;
    Notification::create("Playback stopped", NotificationIcon::Info)->show();
}

void clearEvents() {
    g_events.clear();
    s_playIndex = 0;
}

void rewindPlayback() {
    s_playIndex = 0;
}

void recordPush(int player, int button, int frame) {
    g_events.push_back({ frame, player, button, true });
}

void recordRelease(int player, int button, int frame) {
    g_events.push_back({ frame, player, button, false });
}

void tick(GJBaseGameLayer* layer, int frame) {
    if (!g_playing || !layer) return;

    while (s_playIndex < g_events.size()) {
        auto const& e = g_events[s_playIndex];
        if (e.frame > frame) break;

        auto player = (e.player == 2) ? layer->m_player2 : layer->m_player1;
        if (player) {
            auto button = static_cast<PlayerButton>(e.button);
            if (e.push) player->pushButton(button);
            else player->releaseButton(button);
        }
        s_playIndex++;
    }
}

std::vector<FrameWindow> analyze() {
    std::vector<FrameWindow> out;
    std::set<std::pair<int, int>> seen;

    for (auto const& e : g_events) {
        if (!e.push) continue;
        auto key = std::make_pair(e.frame, e.player);
        if (!seen.insert(key).second) continue;

        FrameWindow w;
        w.frame = e.frame;
        w.window = 1;
        w.player = e.player;
        w.enabled = true;
        w.label = fmt::format("F{} P{}", e.frame, e.player);
        w.color = (e.player == 2)
            ? cocos2d::ccColor4F { 0.40f, 0.70f, 1.00f, 1.00f }
            : cocos2d::ccColor4F { 1.00f, 0.80f, 0.30f, 1.00f };
        out.push_back(w);
    }

    std::stable_sort(out.begin(), out.end(), [](FrameWindow const& a, FrameWindow const& b) {
        return a.frame < b.frame;
    });
    return out;
}

int eventCount() {
    return static_cast<int>(g_events.size());
}

}
