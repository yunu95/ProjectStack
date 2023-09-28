#pragma once
#include "GraphicsEventHandler.h"

struct yunutyEngine::graphics::EventHandler::Impl
{
	static std::unordered_map<GameObject*, std::set<EventHandler*>> eventHandlers;
	std::function<void()> onClick = []() {};
	std::function<void()> onHover = []() {};
};
