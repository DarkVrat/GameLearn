#pragma once

#include"../Resources/ResourceManager.h"

class StateController {
public:
	StateController(const std::string initState);

	void setState(const std::string nextState);
	void update(const uint64_t duration);
	void render(glm::vec2& position, glm::vec2& size, float rotation);

private:
	std::string m_nameState;
	std::string m_bufferState;
	std::shared_ptr<Renderer::StateAnimation> m_State;
	std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, uint64_t>> m_Sprites;
	size_t m_indexFrame;
	uint64_t m_durationFrame;
	uint64_t m_time;
};