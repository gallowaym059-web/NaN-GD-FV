#include "FrameWindowLayer.hpp"

#include "../State.hpp"
#include "../BotController.hpp"

#include <algorithm>

using namespace geode::prelude;

FrameWindowLayer* FrameWindowLayer::create() {
    auto ret = new FrameWindowLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool FrameWindowLayer::init() {
    if (!Popup::init(420.f, 310.f)) return false;
    this->setTitle("Frame Window Visualizer");

    auto size = m_mainLayer->getContentSize();
    float w = size.width;
    float h = size.height;

    m_status = CCLabelBMFont::create("", "chatFont.fnt");
    if (m_status) {
        m_status->setScale(0.45f);
        m_status->setAlignment(cocos2d::kCCTextAlignmentLeft);
        m_status->setAnchorPoint({ 0.f, 0.5f });
        m_status->setPosition({ 14.f, h - 16.f });
        m_mainLayer->addChild(m_status);
    }

    auto menu = CCMenu::create();
    menu->setPosition({ 0.f, 0.f });
    m_mainLayer->addChild(menu);

    auto addTextButton = [&](const char* text, cocos2d::SEL_MenuHandler cb,
                             float x, float y) -> ButtonSprite* {
        auto spr = ButtonSprite::create(text);
        spr->setScale(0.58f);
        auto item = CCMenuItemSpriteExtra::create(spr, this, cb);
        item->setPosition({ x, y });
        menu->addChild(item);
        return spr;
    };

    float row1 = h - 52.f;
    float row2 = h - 92.f;

    m_recordSpr = addTextButton(
        "Record", menu_selector(FrameWindowLayer::onRecord), w * 0.18f, row1
    );
    m_playSpr = addTextButton(
        "Play", menu_selector(FrameWindowLayer::onPlay), w * 0.50f, row1
    );
    addTextButton(
        "Analyze", menu_selector(FrameWindowLayer::onAnalyze), w * 0.82f, row1
    );

    addTextButton(
        "Import", menu_selector(FrameWindowLayer::onImport), w * 0.18f, row2
    );
    addTextButton(
        "Export", menu_selector(FrameWindowLayer::onExport), w * 0.50f, row2
    );
    addTextButton(
        "Clear", menu_selector(FrameWindowLayer::onClear), w * 0.82f, row2
    );

    m_input = TextInput::create(90.f, "frame #");
    m_input->setFilter("0123456789");
    m_input->setPosition({ 70.f, h - 128.f });
    m_mainLayer->addChild(m_input);

    auto addBtnSpr = ButtonSprite::create("Add Frame");
    addBtnSpr->setScale(0.58f);
    auto addBtn = CCMenuItemSpriteExtra::create(
        addBtnSpr, this, menu_selector(FrameWindowLayer::onAdd)
    );
    addBtn->setPosition({ w - 90.f, h - 128.f });
    menu->addChild(addBtn);

    float listW = w - 40.f;
    float listH = h - 165.f;
    m_list = ScrollLayer::create({ listW, listH });
    m_list->setPosition({ 20.f, 18.f });
    m_mainLayer->addChild(m_list);

    this->refreshStatus();
    this->rebuildList();

    return true;
}

void FrameWindowLayer::refreshStatus() {
    if (m_status) {
        char const* state =
            bot::g_recording ? "REC" : (bot::g_playing ? "PLAY" : "IDLE");
        auto text = fmt::format(
            "{} | windows: {} | recorded inputs: {}",
            state, fwv::g_windows.size(), bot::eventCount()
        );
        m_status->setString(text.c_str());
    }

    if (m_recordSpr) {
        m_recordSpr->setString(bot::g_recording ? "Stop" : "Record");
    }
    if (m_playSpr) {
        m_playSpr->setString(bot::g_playing ? "Stop" : "Play");
    }
}

void FrameWindowLayer::rebuildList() {
    if (!m_list) return;

    auto content = m_list->m_contentLayer;
    if (!content) return;

    content->removeAllChildrenWithCleanup(true);

    float width = m_list->getContentSize().width;
    float rowH = 26.f;
    int count = static_cast<int>(fwv::g_windows.size());
    float contentH = std::max(
        m_list->getContentSize().height,
        rowH * static_cast<float>(count) + 8.f
    );

    auto menu = CCMenu::create();
    menu->setPosition({ 0.f, 0.f });
    menu->setContentSize({ width, contentH });
    content->addChild(menu);

    for (int i = 0; i < count; ++i) {
        auto const& win = fwv::g_windows[i];
        float y = contentH - rowH * (static_cast<float>(i) + 0.5f);

        auto text = fmt::format(
            "{} #{}  P{}  +/-{}  {}",
            win.enabled ? "[ON]" : "[--]",
            win.frame, win.player, win.window, win.label
        );
        auto lbl = CCLabelBMFont::create(text.c_str(), "chatFont.fnt");
        lbl->setScale(0.42f);
        lbl->setAnchorPoint({ 0.f, 0.5f });
        lbl->setPosition({ 6.f, y });
        menu->addChild(lbl);

        auto toggleSpr = ButtonSprite::create(win.enabled ? "ON" : "OFF");
        toggleSpr->setScale(0.42f);
        auto toggleBtn = CCMenuItemSpriteExtra::create(
            toggleSpr, this, menu_selector(FrameWindowLayer::onToggle)
        );
        toggleBtn->setTag(i);
        toggleBtn->setPosition({ width - 68.f, y });
        menu->addChild(toggleBtn);

        auto delSpr = ButtonSprite::create("X");
        delSpr->setScale(0.42f);
        auto delBtn = CCMenuItemSpriteExtra::create(
            delSpr, this, menu_selector(FrameWindowLayer::onDelete)
        );
        delBtn->setTag(i);
        delBtn->setPosition({ width - 28.f, y });
        menu->addChild(delBtn);
    }

    content->setContentSize({ width, contentH });
    m_list->scrollToTop();
}

void FrameWindowLayer::onRecord(CCObject*) {
    if (bot::g_recording) {
        bot::stopRecording();
    } else {
        bot::startRecording();
    }
    this->refreshStatus();
}

void FrameWindowLayer::onPlay(CCObject*) {
    if (bot::g_playing) {
        bot::stopPlayback();
    } else {
        bot::startPlayback();
    }
    this->refreshStatus();
}

void FrameWindowLayer::onAnalyze(CCObject*) {
    auto windows = bot::analyze();
    if (windows.empty()) {
        Notification::create(
            "No recorded inputs to analyze.",
            NotificationIcon::Warning
        )->show();
        return;
    }

    fwv::g_windows = windows;
    fwv::sortWindows();
    fwv::saveWindows();
    this->rebuildList();
    this->refreshStatus();

    Notification::create(
        fmt::format("Analyzed {} window(s)", windows.size()),
        NotificationIcon::Success
    )->show();
}

void FrameWindowLayer::onClear(CCObject*) {
    fwv::clearWindows();
    this->rebuildList();
    this->refreshStatus();
}

void FrameWindowLayer::onExport(CCObject*) {
    auto res = fwv::exportToFile();
    if (res.isOk()) {
        Notification::create(
            "Exported to frame-windows.json",
            NotificationIcon::Success
        )->show();
    } else {
        Notification::create("Export failed", NotificationIcon::Error)->show();
    }
}

void FrameWindowLayer::onImport(CCObject*) {
    auto res = fwv::importFromFile();
    if (res.isOk()) {
        this->rebuildList();
        this->refreshStatus();
        Notification::create(
            "Imported windows", NotificationIcon::Success
        )->show();
    } else {
        Notification::create(
            "Import failed (missing file?)",
            NotificationIcon::Error
        )->show();
    }
}

void FrameWindowLayer::onAdd(CCObject*) {
    if (!m_input) return;

    std::string text = m_input->getString();
    if (text.empty()) return;

    int frame = 0;
    try {
        frame = std::stoi(text);
    } catch (...) {
        return;
    }

    FrameWindow w;
    w.frame = frame;
    w.window = 1;
    w.player = 1;
    w.enabled = true;
    w.label = fmt::format("F{} P1", frame);
    w.color = { 1.f, 0.8f, 0.3f, 1.f };

    fwv::addWindow(w);
    m_input->setString("");
    this->rebuildList();
    this->refreshStatus();
}

void FrameWindowLayer::onToggle(CCObject* sender) {
    int i = static_cast<cocos2d::CCNode*>(sender)->getTag();
    if (i < 0 || i >= static_cast<int>(fwv::g_windows.size())) return;

    fwv::g_windows[i].enabled = !fwv::g_windows[i].enabled;
    fwv::saveWindows();
    this->rebuildList();
    this->refreshStatus();
}

void FrameWindowLayer::onDelete(CCObject* sender) {
    int i = static_cast<cocos2d::CCNode*>(sender)->getTag();
    if (i < 0 || i >= static_cast<int>(fwv::g_windows.size())) return;

    fwv::g_windows.erase(fwv::g_windows.begin() + i);
    fwv::saveWindows();
    this->rebuildList();
    this->refreshStatus();
}
