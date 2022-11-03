#pragma once

#include<vector>
#include<string>
#include<map>
#include"Sprite.h"

namespace Renderer{
	class StateAnimation {
	public:
		StateAnimation(std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, uint64_t>> frames, std::vector<std::string> sources, std::string nextState, bool uninterrupted);

		StateAnimation& operator=(const StateAnimation&) = delete;
		StateAnimation(const StateAnimation&) = delete;
		
		bool canChange(const std::string stateName);

		bool getUninterrupted() { return m_uninterrupted; }
		std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, uint64_t>> getFrames() { return m_frames; }
		std::string getNextStateName() { return m_nextState; }

	private:
		std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, uint64_t>> m_frames;
		std::vector<std::string> m_sources;
		std::string m_nextState;
		bool m_uninterrupted;
	};
}
