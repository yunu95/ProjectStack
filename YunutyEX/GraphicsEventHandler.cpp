#include "YunutyEngine.h"
#include "Renderable.h"
#include "GraphicsEventHandler.h"
#include "YunuGraphicsInterface.h"
#include "_GraphicsEventHandlerImpl.h"
#include "Renderable.h"

using namespace yunutyEngine;
using namespace yunutyEngine::graphics;

std::unordered_map<GameObject*, std::set<yunutyEngine::graphics::EventHandler*>> yunutyEngine::graphics::EventHandler::Impl::eventHandlers;

yunutyEngine::graphics::EventHandler::EventHandler()
	:impl(new Impl)
{
	if (auto itr = Impl::eventHandlers.find(GetGameObject()); itr == Impl::eventHandlers.end())
		Impl::eventHandlers[GetGameObject()] = std::set<EventHandler*>{};

	Impl::eventHandlers[GetGameObject()].insert(this);
}
yunutyEngine::graphics::EventHandler::~EventHandler()
{
	delete impl;
	Impl::eventHandlers[GetGameObject()].erase(this);

	if (Impl::eventHandlers[GetGameObject()].empty())
		Impl::eventHandlers.erase(GetGameObject());
}
void yunutyEngine::graphics::EventHandler::SetClickCallback(std::function<void()> clickCallback)
{
	impl->onClick = clickCallback;
}
void yunutyEngine::graphics::EventHandler::SetHoverCallback(std::function<void()> hoverCallback)
{
	impl->onHover = hoverCallback;
}
const set<yunutyEngine::graphics::EventHandler*>* yunutyEngine::graphics::EventHandler::GetEventHandlers(yunuGI::IRenderable* renderable)
{
	auto wrapperMap = Renderable<yunuGI::IRenderable>::yunuGIWrapperMap;
	auto wrapperItr = Renderable<yunuGI::IRenderable>::yunuGIWrapperMap.find(renderable);

	// 그래픽스 객체에 대응되는 게임 Renderable 객체가 없다? 그러면 리턴
	if (wrapperItr == Renderable<yunuGI::IRenderable>::yunuGIWrapperMap.end())
		return nullptr;

	auto eventHandler = EventHandler::Impl::eventHandlers.find(wrapperItr->second->GetGameObject());
	if (eventHandler == EventHandler::Impl::eventHandlers.end())
		return nullptr;

	return &eventHandler->second;
}
void yunutyEngine::graphics::EventHandler::CallEvents(yunuGI::IRenderable* renderable, function<void(yunutyEngine::graphics::EventHandler*)> todo)
{
	auto eventHandlers = GetEventHandlers(renderable);
	if (eventHandlers == nullptr)
		return;

	for (auto eachHandler : *eventHandlers)
		todo(eachHandler);
}
