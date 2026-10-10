#include <Geode/Geode.hpp>
#include "State.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    fwv::loadState();
    log::info("Frame Window Visualizer by bedrock loaded");
}
