#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

#include "../ui/FrameWindowLayer.hpp"

using namespace geode::prelude;

class $modify(FWVPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto rightMenu = this->getChildByID("right-button-menu");
        if (!rightMenu) return;

        auto btn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png"),
            this,
            menu_selector(FWVPauseLayer::onOpenVisualizer)
        );
        btn->setScale(0.8f);
        btn->setID("frame-window-visualizer-button"_spr);
        rightMenu->addChild(btn);
        rightMenu->updateLayout();
    }

    void onOpenVisualizer(CCObject*) {
        if (auto layer = FrameWindowLayer::create()) {
            layer->show();
        }
    }
};
