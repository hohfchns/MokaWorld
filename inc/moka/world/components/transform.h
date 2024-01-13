#pragma once
#include "moka/ecs/core/component.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace moka::world::components
{

class Transform : public moka::ecs::Component
{
public:
  void Move(float x, float y, float z);

  glm::vec3 Right() const;
  glm::vec3 Front() const;
  glm::vec3 Up() const;
  glm::mat4 Model() const;

  glm::vec3 Position() const;
  glm::vec3 Rotation() const;
  glm::vec3 Scale() const;

  glm::vec3 GlobalPosition() const;
  glm::vec3 GlobalRotation() const;
  glm::vec3 GlobalScale() const;

  void SetPosition(const glm::vec3& to);
  void SetRotation(const glm::vec3& to);
  void SetScale(const glm::vec3& to);

  void SetParent(moka::ecs::Entity entity);
  moka::ecs::Entity GetParent() const;

  void AddChild(moka::ecs::Entity entity);
  void RemoveChild(int index);
  const std::vector<moka::ecs::Entity>& GetChildren() const;
  void SetChildren(const std::vector<moka::ecs::Entity>& children);
  moka::ecs::Entity GetChild(int index) const;

  void Move(const glm::vec3& by);
  void Rotate(const glm::vec3& by);

  void _Init() override;

  friend std::ostream& operator<<(std::ostream& os, const Transform& dt);
  Transform& operator=(const Transform& other);

private:
  glm::vec3 _position = glm::vec3(0.f);
  glm::vec3 _scale = glm::vec3(1.f);
  glm::vec3 _rotation = glm::vec3(0.f);

  moka::ecs::Entity _parent = 0;
  std::vector<moka::ecs::Entity> _children;
private:
};

}
