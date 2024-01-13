#include "moka/world/components/transform.h"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <iostream>
#include <cmath>

using namespace glm;
using namespace moka::ecs;

namespace moka::world::components
{

void Transform::_Init()
{
}

void Transform::SetPosition(const vec3& to)
{
  _position = to;
}

void Transform::SetScale(const vec3& to)
{
  _scale = to;
}

void Transform::SetRotation(const vec3& to)
{
  _rotation = to;
}

vec3 Transform::Position() const
{
  return _position;
}
vec3 Transform::Scale() const
{
  return _scale;
}

vec3 Transform::Rotation() const
{
  return _rotation;
}

vec3 Transform::GlobalPosition() const
{
  vec3 pos = vec3(0.f);

  Entity itrTrans = GetEntity();
  while (itrTrans && ECS::Get().GetComponentP<Transform>(itrTrans))
  {
    auto trans = ECS::Get().GetComponentP<Transform>(itrTrans);
    pos += trans->Position();

    itrTrans = trans->GetParent();
  }

  return pos;
}

vec3 Transform::GlobalRotation() const
{
  vec3 rot = vec3(0.f);

  Entity itrTrans = GetEntity();
  while (itrTrans && ECS::Get().GetComponentP<Transform>(itrTrans))
  {
    auto trans = ECS::Get().GetComponentP<Transform>(itrTrans);
    rot.x = fmod(rot.x + trans->Rotation().x, 360.f);
    rot.y = fmod(rot.y + trans->Rotation().y, 360.f);
    rot.z = fmod(rot.z + trans->Rotation().z, 360.f);

    itrTrans = trans->GetParent();
  }

  return rot;
}

vec3 Transform::GlobalScale() const
{
  vec3 sc = vec3(1.f);

  Entity itrTrans = GetEntity();
  while (itrTrans && ECS::Get().GetComponentP<Transform>(itrTrans))
  {
    auto trans = ECS::Get().GetComponentP<Transform>(itrTrans);
    sc *= trans->Scale();

    itrTrans = trans->GetParent();
  }

  return sc;
}

vec3 Transform::Right() const
{
  mat4 transform = Model();
  return -vec3(transform[0][0], transform[1][0], transform[2][0]);
}

vec3 Transform::Front() const
{
  mat4 transform = Model();
  return vec3(transform[0][2], transform[1][2], transform[2][2]);
}

vec3 Transform::Up() const
{
  mat4 transform = Model();
  float a1 = transform[0][1];
  float a2 = transform[1][1];
  float a3 = transform[2][1];
  vec3 ret = vec3(a1, a2, a3);
  return ret;
}

void Transform::Move(const vec3& by)
{
  _position += by;
}

void Transform::Rotate(const vec3& by)
{
  _rotation.x = fmod(_rotation.x + by.x, 360.f);
  _rotation.y = fmod(_rotation.y + by.y, 360.f);
  _rotation.z = fmod(_rotation.z + by.z, 360.f);
}

mat4 Transform::Model() const
{
  mat4 transform = mat4(1.f);

  transform = translate(transform, GlobalPosition());

  Entity itrTrans = GetEntity();
  while (itrTrans && ECS::Get().GetComponentP<Transform>(itrTrans))
  {
    auto trans = ECS::Get().GetComponentP<Transform>(itrTrans);

    vec3 rot = trans->Rotation();
    quat orientation = quat(eulerAngleXYZ(radians(rot.x), radians(rot.y), radians(rot.z)));
    mat4 rotationMat = mat4(orientation);

    glm::vec3 vecToOrigin = trans->GlobalPosition() - GlobalPosition();
    transform = translate(transform, vecToOrigin);
    transform *= rotationMat;
    transform = translate(transform, -vecToOrigin);

    itrTrans = trans->GetParent();
  }

  transform = scale(transform, GlobalScale());
  
  return transform;
}

std::ostream& operator<<(std::ostream& os, const Transform& tc)
{
  os << to_string(tc.Position());
  return os;
}

Transform& Transform::operator=(const Transform& other)
{
  if (this == &other)
  {
    return *this;
  }

  SetPosition(other.Position());
  SetRotation(other.Rotation());

  return *this;
}

void Transform::AddChild(Entity entity)
{
  _children.push_back(entity);
  ECS::Get().GetComponentP<Transform>(entity)->_parent = GetEntity();
}

void Transform::RemoveChild(int index)
{
  // TODO make consistent dependency
  _children.erase(_children.begin() + index);
}

const std::vector<Entity>& Transform::GetChildren() const
{
  return _children;
}

void Transform::SetChildren(const std::vector<Entity>& children)
{
  // TODO make consistent dependency
  _children = children;
}

Entity Transform::GetChild(int index) const
{
  return _children[index];
}

void Transform::SetParent(Entity entity)
{
  ECS::Get().GetComponentP<Transform>(entity)->AddChild(GetEntity());
}

Entity Transform::GetParent() const
{
  return _parent;
}

}

