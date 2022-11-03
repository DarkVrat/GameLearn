#include "GameObject.h"
#include"../Resources/ResourceManager.h"

GameObject::GameObject(glm::vec2& position, glm::vec2& size, float rotation){
	m_position=position;
	m_size=size;
	m_rotation=rotation;
}

void GameObject::render(){
}

void GameObject::setPosition(const glm::vec2& position){
	m_position = position;
}

void GameObject::setSize(const glm::vec2& size){
	m_size = size;
}

void GameObject::setRotation(const float rotation){
	m_rotation = rotation;
}
