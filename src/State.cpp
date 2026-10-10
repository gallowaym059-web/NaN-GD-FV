#include "State.hpp"

#include <Geode/Geode.hpp>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace geode::prelude;

namespace fwv {

std::vector<FrameWindow> g_windows;
int g_currentFrame = 0;

static bool readSettingBool(char const* key) {
    auto mod = Mod::get();
    if (!mod->hasSetting(key)) return true;
    return mod->getSettingValue<bool>(key);
}

bool enabled() { return readSettingBool("enabled"); }
bool showHud() { return readSettingBool("show-hud"); }
bool showMarkers() { return readSettingBool("show-markers"); }

float markerSize() {
    auto mod = Mod::get();
    if (!mod->hasSetting("marker-size")) return 1.f;
    return mod->getSettingValue<float>("marker-size");
}

void loadState() {
    static bool s_loaded = false;
    if (s_loaded) return;
    s_loaded = true;

    g_windows = Mod::get()->getSavedValue<std::vector<FrameWindow>>("windows", {});
    sortWindows();
    log::info("Frame Window Visualizer: loaded {} window(s)", g_windows.size());
}

void saveWindows() {
    Mod::get()->setSavedValue("windows", g_windows);
}

void addWindow(FrameWindow const& w) {
    g_windows.push_back(w);
    sortWindows();
    saveWindows();
}

void clearWindows() {
    g_windows.clear();
    saveWindows();
}

void sortWindows() {
    std::stable_sort(g_windows.begin(), g_windows.end(), [](FrameWindow const& a, FrameWindow const& b) {
        return a.frame < b.frame;
    });
}

int enabledWindowCount() {
    int n = 0;
    for (auto const& w : g_windows) if (w.enabled) n++;
    return n;
}

std::filesystem::path dataFilePath() {
    return Mod::get()->getSaveDir() / "frame-windows.json";
}

geode::Result<> exportToFile() {
    auto path = dataFilePath();

    std::vector<matjson::Value> arr;
    arr.reserve(g_windows.size());
    for (auto const& w : g_windows) {
        arr.push_back(matjson::Serialize<FrameWindow>::toJson(w));
    }

    matjson::Value root = matjson::makeObject({ { "windows", arr } });

    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out.is_open()) return geode::Err(fmt::format("Could not open file for writing: {}", path.string()));
    out << root.dump(2);
    out.close();
    return geode::Ok();
}

geode::Result<> importFromFile() {
    auto path = dataFilePath();
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open()) return geode::Err(fmt::format("Could not open file: {}", path.string()));

    std::stringstream buffer;
    buffer << in.rdbuf();
    in.close();

    auto parsed = matjson::parse(buffer.str());
    if (!parsed) return geode::Err(fmt::format("Invalid JSON in {}", path.string()));

    auto value = parsed.unwrap();
    if (!value.contains("windows") || !value["windows"].isArray()) {
        return geode::Err(fmt::format("Missing 'windows' array in {}", path.string()));
    }

    auto list = value["windows"].as<std::vector<FrameWindow>>();
    if (!list) return geode::Err("Failed to parse windows");

    g_windows = list.unwrap();
    sortWindows();
    saveWindows();
    return geode::Ok();
}

}
