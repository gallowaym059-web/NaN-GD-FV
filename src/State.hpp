#pragma once

#include "FrameData.hpp"
#include <vector>
#include <string>
#include <filesystem>

namespace fwv {

extern std::vector<FrameWindow> g_windows;
extern int g_currentFrame;

bool enabled();
bool showHud();
bool showMarkers();
float markerSize();

void loadState();
void saveWindows();

void addWindow(FrameWindow const& w);
void clearWindows();
void sortWindows();
int enabledWindowCount();

std::filesystem::path dataFilePath();
geode::Result<> exportToFile();
geode::Result<> importFromFile();

}
