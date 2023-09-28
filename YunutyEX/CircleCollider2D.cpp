#include "YunutyEngine.h"

using namespace yunutyEngine;

struct CircleCollider2D::Impl
{
	double radius = 0;
};
CircleCollider2D::CircleCollider2D()
{
	impl = new Impl{};
}
CircleCollider2D::~CircleCollider2D()
{
	delete impl;
}
double CircleCollider2D::GetArea()const
{
	return impl->radius * impl->radius * math::PI;
}
bool CircleCollider2D::isOverlappingWith(const Collider2D* other)const
{
	return other->isOverlappingWith(this);
}
bool CircleCollider2D::isOverlappingWith(const BoxCollider2D* other)const
{
	return Collider2D::isOverlapping(other, this);
}
bool CircleCollider2D::isOverlappingWith(const CircleCollider2D* other)const
{
	return Collider2D::isOverlapping(other, this);
}
bool CircleCollider2D::isOverlappingWith(const LineCollider2D* other)const
{
	return Collider2D::isOverlapping(other, this);
}
bool CircleCollider2D::isInsideNode(const QuadTreeNode* node)const
{
	auto position = GetTransform()->GetWorldPosition();
	return node->xInterval.left - impl->radius < position.x&& position.x < node->xInterval.right + impl->radius &&
		node->yInterval.left - impl->radius < position.y&& position.y < node->yInterval.right + impl->radius;
}
void CircleCollider2D::SetRadius(double radius)
{
	this->impl->radius = radius;
}
double CircleCollider2D::GetRadius()const
{
	return this->impl->radius;
}
Interval CircleCollider2D::projectedInterval(const Vector2d& v)const
{
	auto projectedCenter = Vector2d::Dot(GetTransform()->GetWorldPosition(), v);
	return Interval::FromRadius(projectedCenter, impl->radius * GetTransform()->GetWorldScale().x);
}