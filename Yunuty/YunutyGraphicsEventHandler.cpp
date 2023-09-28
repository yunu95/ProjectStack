#include "YunutyEngine.h"
#include "YunutyRenderable.h"
#include "YunutyGraphicsEventHandler.h"
#include "YunuGraphicsInterface.h"
#include "YunutyRenderable.h"

std::unordered_map<GameObject*, std::set<yunutyEngine::graphics::EventHandler*>>
yunutyEngine::graphics::EventHandler::eventHandlers;
const set<yunutyEngine::graphics::EventHandler*>* yunutyEngine::graphics::EventHandler::GetEventHandlers(yunuGI::IRenderable* renderable)
{
    auto wrapperMap = Renderable<yunuGI::IRenderable>::yunuGIWrapperMap;
    auto wrapperItr = Renderable<yunuGI::IRenderable>::yunuGIWrapperMap.find(renderable);

    // �׷��Ƚ� ��ü�� �����Ǵ� ���� Renderable ��ü�� ����? �׷��� ����
    if (wrapperItr == Renderable<yunuGI::IRenderable>::yunuGIWrapperMap.end())
        return nullptr;

    auto eventHandler = EventHandler::eventHandlers.find(wrapperItr->second->GetGameObject());
    if (eventHandler == EventHandler::eventHandlers.end())
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
