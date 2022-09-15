#pragma once

#include "Sprite.h"
#include <map>
#include <vector>

namespace Renderer{
	class AnimSprite : public Sprite {
	public:
		//�����������
		AnimSprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram, const glm::vec2& position = glm::vec2(0.f), const glm::vec2& size = glm::vec2(1.f), const float rotation = 0.f);

		void insertState(std::string state, std::vector<std::pair<std::string, size_t>> subTexturesDura);
		void setState(const std::string newState);
		void update(const uint64_t delta);
		void render() const override;

	private:
		size_t m_currentFrame = 0;//����� ������� � ���������
		uint64_t m_currentAnimTime = 0;//��������� �����
		std::map < std::string, std::vector<std::pair<std::string, uint64_t>>> m_statesMap;//MAP ���������
		std::map < std::string, std::vector<std::pair<std::string, uint64_t>>>::const_iterator m_pCurrAnimDura;//�������� ��� ���������
		mutable bool m_dirty = false; //���������� ��� ������������ ��������
	};
}