#include "AnimSprite.h"
#include "Texture2D.h"
#include <iostream>

namespace Renderer {
	// онструктор с предачей атласа и начальной текстурой
	AnimSprite::AnimSprite(std::shared_ptr<Texture2D> pTexture,std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram, const glm::vec2& position, const glm::vec2& size, const float rotation):Sprite(std::move(pTexture), std::move(initialSubTexture), std::move(pShaderProgram), position, size, rotation) {
		m_pCurrAnimDura = m_statesMap.end();
	}

	//добавление анимации, название, вектор из состо€ний, ноход€щихс€ в переданом атласе, и времени
	void AnimSprite::insertState(std::string state, std::vector<std::pair<std::string, uint64_t>> subTexturesDura) {
		m_statesMap.emplace(std::move(state), std::move(subTexturesDura));
	}

	//установка определЄнного состо€ни€
	void AnimSprite::setState(const std::string newState){
		auto it = m_statesMap.find(newState); //ѕоиск
		if (it == m_statesMap.end()) { //ѕроверка есть ли
			std::cout << "ERROR ANIM" << std::endl;
			return; 
		}
		if (it != m_pCurrAnimDura) { //”становка состо€ни€, если отличаетс€ от текущего
			m_currentAnimTime = 0;
			m_currentFrame = 0;
			m_dirty = true;
			m_pCurrAnimDura = it;
		}
	}

	//обновление анимации
	void AnimSprite::update(const uint64_t delta){
		if (m_pCurrAnimDura != m_statesMap.end()) {//установлено ли состо€ние
			m_currentAnimTime += delta; //увеличение времени
			while (m_currentAnimTime >= m_pCurrAnimDura->second[m_currentFrame].second) {//пока врем€ больше чем прошло
				m_currentAnimTime -= m_pCurrAnimDura->second[m_currentFrame].second;//уменьшаем врем€
				m_currentFrame++;//переключаем кадр
				m_dirty = true;//”становка дл€ смены изображени€
				if (m_currentFrame == m_pCurrAnimDura->second.size()) m_currentFrame = 0;//≈сли кадр больше чем есть в состо€нии, то сброс
			}
		}
	}

	void AnimSprite::render() const {
		if (m_dirty) { //≈сли нужно сменить подтекстуру
			auto subTexture = m_pTexture->getSubTexture2D(m_pCurrAnimDura->second[m_currentFrame].first);//получение текстуры
			const GLfloat textureCoords[] = { //”становка координат
				//U							V
				subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,	//1.1
				subTexture.leftBottomUV.x, subTexture.rightTopUV.y,		//1.2
				subTexture.rightTopUV.x,   subTexture.rightTopUV.y,		//1.3
																		//
				subTexture.rightTopUV.x,   subTexture.rightTopUV.y,		//2.1
				subTexture.rightTopUV.x,   subTexture.leftBottomUV.y,	//2.2
				subTexture.leftBottomUV.x, subTexture.leftBottomUV.y	//2.3
			};
			//ѕрив€зка к буферу и загрузка нового массива
			glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordsVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textureCoords), &textureCoords);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			m_dirty = false;//¬озврат m_dirty что бы не мен€ть спрайт
		}
		Sprite::render();//ќтрисовка
	}
}