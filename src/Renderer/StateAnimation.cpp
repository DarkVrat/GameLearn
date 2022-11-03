#include "StateAnimation.h"

namespace Renderer{
	StateAnimation::StateAnimation(std::vector<std::pair<std::string, uint64_t>> frames, std::vector<std::string> sources, bool uninterrupted)
		:m_frames(std::move(frames)),m_sources(std::move(sources)), m_uninterrupted(uninterrupted) {
	}


	bool StateAnimation::canChange(const std::string stateName) {
		for (std::string current : m_sources) {
			if (current == stateName) return true;
		}
		return false;
	}
}