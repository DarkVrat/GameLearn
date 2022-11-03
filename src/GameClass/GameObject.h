#pragma once

#include<glad/glad.h>
#include<glm/vec2.hpp>
#include<memory>

class GameObject {
public:
	GameObject(glm::vec2& position, glm::vec2& size, float rotation);

	void render();

	void setPosition(const glm::vec2& position);
	void setSize(const glm::vec2& size);
	void setRotation(const float rotation);
private:
	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_rotation;
};