#pragma once

#include<glad/glad.h>
#include<glm/vec2.hpp>
#include<memory>
#include<string>
#include"VertexBuffer.h"
#include"VertexArray.h"
#include"IndexBuffer.h"

namespace Renderer {
	class Texture2D;
	class ShaderProgram;

	class Sprite {
	public:
		//Конструкторы и деструкторы
		Sprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram, const glm::vec2& position = glm::vec2(0.f), const glm::vec2& size = glm::vec2(1.f), const float rotation = 0.f);
		Sprite& operator=(const Sprite&) = delete;
		Sprite(const Sprite&) = delete;
		~Sprite();
		
		//отрисовка
		virtual void render()const;

		//установка парметров спрайта
		void setPosition(const glm::vec2& position);
		void setSize(const glm::vec2& size);
		void setRotation(const float rotation);

	protected:
		std::shared_ptr<Texture2D> m_pTexture; //текстура
		std::shared_ptr<ShaderProgram> m_pShaderProgram; //Шейдерная программа

		//Папаметры спрайта
		glm::vec2 m_position; 
		glm::vec2 m_size;
		float m_rotation;

		//буферы
		VertexArray m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_textureCoordsBuffer;
		IndexBuffer m_indexBuffer;
	};
}