#include "VisualizerNode.hpp"
#include "State.hpp"

using namespace geode::prelude;

VisualizerNode* VisualizerNode::create() {
    auto ret = new VisualizerNode();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool VisualizerNode::init() {
    if (!CCNode::init()) return false;

    m_hud = CCLabelBMFont::create("Frame: 0", "chatFont.fnt");
    if (m_hud) {
        m_hud->setAnchorPoint({ 0.f, 1.f });
        m_hud->setScale(0.55f);
        m_hud->setAlignment(cocos2d::kCCTextAlignmentLeft);
        auto win = cocos2d::CCDirector::sharedDirector()->getWinSize();
        m_hud->setPosition({ 8.f, win.height - 8.f });
        this->addChild(m_hud);
    }

    return true;
}

void VisualizerNode::resetAttempt() {
    m_hitIndices.clear();
}

void VisualizerNode::onMarkerDone(CCNode* node) {
    if (node) node->removeFromParentAndCleanup(true);
}

void VisualizerNode::spawnMarker(CCNode* player, FrameWindow const& w) {
    if (!player) return;

    float scale = fwv::markerSize();

    auto container = CCNode::create();

    auto dot = cocos2d::CCDrawNode::create();
    if (dot) {
        dot->drawDot({ 0.f, 0.f }, 14.f * scale, w.color);
        container->addChild(dot);
    }

    if (!w.label.empty()) {
        auto label = CCLabelBMFont::create(w.label.c_str(), "chatFont.fnt");
        if (label) {
            label->setScale(0.5f * scale);
            label->setPosition({ 0.f, 30.f * scale });
            label->setColor(cocos2d::ccc3(
                static_cast<GLubyte>(w.color.r * 255.f),
                static_cast<GLubyte>(w.color.g * 255.f),
                static_cast<GLubyte>(w.color.b * 255.f)
            ));
            container->addChild(label);
        }
    }

    auto playerSize = player->getContentSize();
    container->setPosition({ playerSize.width * 0.5f, playerSize.height * 0.5f });
    container->setZOrder(999);
    player->addChild(container);

    auto seq = cocos2d::CCSequence::create(
        cocos2d::CCDelayTime::create(0.35f),
        cocos2d::CCFadeOut::create(0.35f),
        cocos2d::CCCallFuncN::create(this, callfuncN_selector(VisualizerNode::onMarkerDone)),
        nullptr
    );
    container->runAction(seq);
}

void VisualizerNode::updateHud(int frame) {
    if (!m_hud) return;

    int total = 0;
    int hit = 0;
    for (int i = 0; i < static_cast<int>(fwv::g_windows.size()); ++i) {
        if (!fwv::g_windows[i].enabled) continue;
        total++;
        if (m_hitIndices.count(i)) hit++;
    }

    auto text = fmt::format(
        "Frame: {}\nWindows: {}   Hits: {}/{}",
        frame, fwv::g_windows.size(), hit, total
    );
    m_hud->setString(text.c_str());
}

void VisualizerNode::tick(GJBaseGameLayer* layer, int frame) {
    if (!fwv::enabled() || !layer) return;

    if (frame == 0) m_hitIndices.clear();

    if (fwv::showHud()) updateHud(frame);
    if (!fwv::showMarkers()) return;

    for (int i = 0; i < static_cast<int>(fwv::g_windows.size()); ++i) {
        auto const& w = fwv::g_windows[i];
        if (!w.enabled) continue;
        if (frame != w.frame) continue;
        if (m_hitIndices.count(i)) continue;

        m_hitIndices.insert(i);

        auto player = (w.player == 2) ? layer->m_player2 : layer->m_player1;
        spawnMarker(player, w);
    }
}
