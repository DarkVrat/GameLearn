#include "GameObject.h"
#include"../Resources/ResourceManager.h"

GameObject::GameObject(glm::vec2& position, glm::vec2& size, float rotation, std::string startState){
	m_position=position;
	m_size=size;
	m_rotation=rotation;
	m_stateControll = std::make_shared<StateController>(startState);
}



void GameObject::update(uint64_t duration){
	m_stateControll->update(duration);
}

void GameObject::render(){
	m_stateControll->render(m_position, m_size, m_rotation);
}

void GameObject::attack(){
	m_stateControll->setState("Attack1");
}

void GameObject::idle(){
	m_stateControll->setState("Idle");
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
