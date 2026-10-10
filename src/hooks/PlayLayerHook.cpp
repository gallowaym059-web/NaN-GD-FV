#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include "../State.hpp"
#include "../BotController.hpp"
#include "../VisualizerNode.hpp"

using namespace geode::prelude;

class $modify(FWVBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        VisualizerNode* m_viz = nullptr;
        int m_frame = 0;
    };

    void update(float dt) {
        auto pl = PlayLayer::get();
        bool active = pl && (static_cast<GJBaseGameLayer*>(pl) == this) && !pl->m_isPaused;

        if (active) {
            if (!m_fields->m_viz) {
                m_fields->m_viz = VisualizerNode::create();
                if (m_fields->m_viz) {
                    this->addChild(m_fields->m_viz, 1000);
                }
            }

            fwv::g_currentFrame = m_fields->m_frame;
            if (bot::g_playing) {
                bot::tick(this, m_fields->m_frame);
            }
        }

        GJBaseGameLayer::update(dt);

        if (active && m_fields->m_viz) {
            m_fields->m_viz->tick(this, m_fields->m_frame);
            m_fields->m_frame++;
        }
    }
};

class $modify(FWVPlayLayer, PlayLayer) {
    void resetLevel() {
        PlayLayer::resetLevel();

        fwv::g_currentFrame = 0;
        bot::rewindPlayback();

        if (auto base = static_cast<GJBaseGameLayer*>(this)) {
            if (auto viz = base->getChildByType<VisualizerNode>(0)) {
                viz->resetAttempt();
            }
        }
    }
};
