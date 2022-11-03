#pragma once

#include<vector>
#include<string>
#include<map>
#include"Sprite.h"

namespace Renderer{
	class StateAnimation {
	public:
		StateAnimation(std::vector<std::pair<std::string, uint64_t>> frames, std::vector<std::string> sources, bool uninterrupted);

		StateAnimation& operator=(const StateAnimation&) = delete;
		StateAnimation(const StateAnimation&) = delete;
		
		bool canChange(const std::string stateName);

		bool getUinterrupted() { return m_uninterrupted; }
		size_t sizeOfFrames() { return m_frames.size(); }
		uint64_t getDurationFrame(const size_t index) { return m_frames[index].second; }
		std::string getSprite(const size_t index) { return m_frames[index].first; }

	private:
		std::vector<std::pair<std::string, uint64_t>> m_frames;
		std::vector<std::string> m_sources;
		std::string m_nextState;
		bool m_uninterrupted;
	};
}
