#pragma once

#include <Geode/Geode.hpp>
#include <matjson/std.hpp>
#include <string>

using namespace geode::prelude;

struct FrameWindow {
    int frame = 0;
    int window = 1;
    int player = 1;
    bool enabled = true;
    std::string label = "";
    cocos2d::ccColor4F color = { 1.f, 1.f, 1.f, 1.f };

    bool operator==(FrameWindow const& other) const {
        return frame == other.frame &&
            window == other.window &&
            player == other.player &&
            enabled == other.enabled &&
            label == other.label &&
            color.r == other.color.r &&
            color.g == other.color.g &&
            color.b == other.color.b &&
            color.a == other.color.a;
    }
};

template <>
struct matjson::Serialize<FrameWindow> {
    static geode::Result<FrameWindow> fromJson(matjson::Value const& value) {
        if (!value.isObject()) return geode::Err("FrameWindow must be an object");
        FrameWindow w;
        w.frame = value["frame"].asInt().unwrapOr(0);
        w.window = value["window"].asInt().unwrapOr(1);
        w.player = value["player"].asInt().unwrapOr(1);
        w.enabled = value["enabled"].asBool().unwrapOr(true);
        w.label = value["label"].asString().unwrapOr("");
        w.color.r = static_cast<float>(value["r"].asDouble().unwrapOr(1.0));
        w.color.g = static_cast<float>(value["g"].asDouble().unwrapOr(1.0));
        w.color.b = static_cast<float>(value["b"].asDouble().unwrapOr(1.0));
        w.color.a = static_cast<float>(value["a"].asDouble().unwrapOr(1.0));
        if (w.window < 0) w.window = 0;
        if (w.player != 2) w.player = 1;
        return geode::Ok(w);
    }

    static matjson::Value toJson(FrameWindow const& w) {
        return matjson::makeObject({
            { "frame", w.frame },
            { "window", w.window },
            { "player", w.player },
            { "enabled", w.enabled },
            { "label", w.label },
            { "r", static_cast<double>(w.color.r) },
            { "g", static_cast<double>(w.color.g) },
            { "b", static_cast<double>(w.color.b) },
            { "a", static_cast<double>(w.color.a) }
        });
    }
};
