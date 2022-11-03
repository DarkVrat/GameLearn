#pragma once


#include<memory>
#include<string>
#include<glad/glad.h>
#include<glm/vec2.hpp>
#include"VertexBuffer.h"
#include"VertexArray.h"
#include"IndexBuffer.h"

namespace Renderer {
	class Texture2D;
	class ShaderProgram;

	class Sprite {
	public:
		//������������ � �����������
		Sprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram);
		Sprite& operator=(const Sprite&) = delete;
		Sprite(const Sprite&) = delete;
		~Sprite();
		
		//���������
		void render(const glm::vec2& position, const glm::vec2& size, const float rotation)const;

	protected:
		std::shared_ptr<Texture2D> m_pTexture; //��������
		std::shared_ptr<ShaderProgram> m_pShaderProgram; //��������� ���������

		//������
		VertexArray m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_textureCoordsBuffer;
		IndexBuffer m_indexBuffer;
	};
}