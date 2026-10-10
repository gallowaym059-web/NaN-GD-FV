#pragma once

#include <Geode/Geode.hpp>
#include "FrameData.hpp"
#include <set>

class GJBaseGameLayer;

class VisualizerNode : public cocos2d::CCNode {
public:
    static VisualizerNode* create();

    void tick(GJBaseGameLayer* layer, int frame);
    void resetAttempt();

protected:
    bool init() override;

    void spawnMarker(cocos2d::CCNode* player, FrameWindow const& w);
    void updateHud(int frame);
    void onMarkerDone(cocos2d::CCNode* node);

    cocos2d::CCLabelBMFont* m_hud = nullptr;
    std::set<int> m_hitIndices;
};
