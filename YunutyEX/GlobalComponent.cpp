#include "YunutyEngine.h"
#include "GlobalComponent.h"

using namespace yunutyEngine;

struct yunutyEngine::GlobalComponent::GlobalComponent::Impl
{
	static std::unordered_set<GlobalComponent*> globalComponents;
};
//std::map<GlobalComponent*, unique_ptr<GlobalComponent>> YunutyEngine::GlobalComponent::globalComponents;
std::unordered_set<GlobalComponent*> yunutyEngine::GlobalComponent::GlobalComponent::Impl::globalComponents;
std::unordered_set<GlobalComponent*>& GlobalComponent::RefGlobalComponents()
{
	return Impl::globalComponents;
}
GlobalComponent::GlobalComponent()
{
	Impl::globalComponents.insert(this);
}
GlobalComponent::~GlobalComponent()
{
	Impl::globalComponents.erase(this);
}
