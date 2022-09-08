#include "AnimSprite.h"
#include "Texture2D.h"
#include <iostream>

namespace Renderer {
	AnimSprite::AnimSprite(	std::shared_ptr<Texture2D> pTexture,
							std::string initialSubTexture, 
							std::shared_ptr<ShaderProgram> pShaderProgram, 
							const glm::vec2& position, 
							const glm::vec2& size, 
							const float rotation):Sprite(std::move(pTexture), std::move(initialSubTexture), std::move(pShaderProgram), position, size, rotation) {
		m_pCurrAnimDura = m_statesMap.end();
	}

	void AnimSprite::insertState(std::string state, std::vector<std::pair<std::string, uint64_t>> subTexturesDura) {
		m_statesMap.emplace(std::move(state), std::move(subTexturesDura));
	}

	void AnimSprite::setState(const std::string newState){
		auto it = m_statesMap.find(newState);
		if (it == m_statesMap.end()) {
			std::cout << "ERROR ANIM" << std::endl;
			return;
		}

		if (it != m_pCurrAnimDura) {
			m_currentAnimTime = 0;
			m_currentFrame = 0;
			m_dirty = true;
			m_pCurrAnimDura = it;
		}
	}

	void AnimSprite::update(const uint64_t delta){
		if (m_pCurrAnimDura != m_statesMap.end()) {
			m_currentAnimTime += delta;

			while (m_currentAnimTime >= m_pCurrAnimDura->second[m_currentFrame].second) {
				m_currentAnimTime -= m_pCurrAnimDura->second[m_currentFrame].second;
				m_currentFrame++;
				m_dirty = true;
				if (m_currentFrame == m_pCurrAnimDura->second.size()) {
					m_currentFrame = 0;
				}
			}
		}
	}

	void AnimSprite::render() const {
		if (m_dirty) {
			auto subTexture = m_pTexture->getSubTexture2D(m_pCurrAnimDura->second[m_currentFrame].first);

			const GLfloat textureCoords[] = {
				//U							V
				subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,	//1.1
				subTexture.leftBottomUV.x, subTexture.rightTopUV.y,		//1.2
				subTexture.rightTopUV.x,   subTexture.rightTopUV.y,		//1.3
																		//
				subTexture.rightTopUV.x,   subTexture.rightTopUV.y,		//2.1
				subTexture.rightTopUV.x,   subTexture.leftBottomUV.y,	//2.2
				subTexture.leftBottomUV.x, subTexture.leftBottomUV.y	//2.3
			};

			glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordsVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textureCoords), &textureCoords);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_dirty = false;
		}

		Sprite::render();
	}
}