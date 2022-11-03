#pragma once

#include<glad/glad.h>
#include<glm/vec2.hpp>
#include<memory>
#include"StateController.h"

class GameObject {
public:
	GameObject(glm::vec2& position, glm::vec2& size, float rotation, std::string startState);

	void update(uint64_t duration);
	void render();
	void attack();
	void idle();

	void setPosition(const glm::vec2& position);
	void setSize(const glm::vec2& size);
	void setRotation(const float rotation);
private:
	std::shared_ptr<StateController> m_stateControll;
	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_rotation;
};