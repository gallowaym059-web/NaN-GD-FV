#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/ui/ScrollLayer.hpp>

class FrameWindowLayer : public geode::Popup {
public:
    static FrameWindowLayer* create();

protected:
    bool init() override;

    void rebuildList();
    void refreshStatus();

    void onRecord(cocos2d::CCObject*);
    void onPlay(cocos2d::CCObject*);
    void onAnalyze(cocos2d::CCObject*);
    void onClear(cocos2d::CCObject*);
    void onExport(cocos2d::CCObject*);
    void onImport(cocos2d::CCObject*);
    void onAdd(cocos2d::CCObject*);
    void onToggle(cocos2d::CCObject* sender);
    void onDelete(cocos2d::CCObject* sender);

    cocos2d::CCLabelBMFont* m_status = nullptr;
    ButtonSprite* m_recordSpr = nullptr;
    ButtonSprite* m_playSpr = nullptr;
    geode::ScrollLayer* m_list = nullptr;
    geode::TextInput* m_input = nullptr;
};
