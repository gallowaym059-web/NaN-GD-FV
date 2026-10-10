#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

#include "../State.hpp"
#include "../BotController.hpp"

using namespace geode::prelude;

class $modify(FWVPlayerObject, PlayerObject) {
    static int playerNumberFor(PlayerObject* self) {
        auto pl = PlayLayer::get();
        if (pl && pl->m_player2 == self) return 2;
        return 1;
    }

    void pushButton(PlayerButton button) {
        if (bot::g_recording && !bot::g_playing) {
            bot::recordPush(playerNumberFor(this), static_cast<int>(button), fwv::g_currentFrame);
        }
        PlayerObject::pushButton(button);
    }

    void releaseButton(PlayerButton button) {
        if (bot::g_recording && !bot::g_playing) {
            bot::recordRelease(playerNumberFor(this), static_cast<int>(button), fwv::g_currentFrame);
        }
        PlayerObject::releaseButton(button);
    }
};
