#include "Sprite.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {
	Sprite::Sprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram, const glm::vec2& position, const glm::vec2& size, const float rotation)
		:m_pTexture(pTexture), m_pShaderProgram(std::move(pShaderProgram)), m_position(position), m_size(size), m_rotation(rotation) {
		//установка координат вертексов текстуры 
		const GLfloat vertexCoords[] = {
			//x    y
			0.f, 0.f,//0				1--2
			0.f, 1.f,//1				| /|
			1.f, 1.f,//2				|/ |
			1.f, 0.f,//3				0--3
		};
		auto subTexture = m_pTexture->getSubTexture2D(std::move(initialSubTexture)); //загрузка подтекстуры или всей текстуры
		//установка координат текстуры к вертексам
		const GLfloat textureCoords[] = {
			//U						   //V
			subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,	//0
			subTexture.leftBottomUV.x, subTexture.rightTopUV.y,		//1
			subTexture.rightTopUV.x,   subTexture.rightTopUV.y,		//2
			subTexture.rightTopUV.x,   subTexture.leftBottomUV.y,	//3
		};

		const GLint indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_vertexCoordsBuffer.init(vertexCoords, sizeof(vertexCoords));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_textureCoordsBuffer.init(textureCoords, sizeof(textureCoords));
		VertexBufferLayout textureCoordsLayout;
		textureCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

		m_indexBuffer.init(indices, sizeof(indices));

		m_vertexArray.unbind();
		m_indexBuffer.unbind();
	}

	//очистка буферов
	Sprite::~Sprite() {
	}

	//отрисовка спрайта
	void Sprite::render() const {
		m_pShaderProgram->use();//Активация шейдера

		glm::mat4 model(1.f);//создание матрицы

		//Перемещение в точку, потом на 0.5 от размера, поворот, возврат на -0.5 от размера, увеличение
		model = glm::translate(model, glm::vec3(m_position, 0.f));
		model = glm::translate(model, glm::vec3( 0.5f * m_size.x,  0.5f * m_size.y, 0.f));
		model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.f, 0.f, 1.f));
		model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.f));
		model = glm::scale(model, glm::vec3(m_size, 1.f));

		m_vertexArray.bind();
		m_pShaderProgram->setMatrix4("modelMat", model);//передача полученой матрицы в шейдер

		glActiveTexture(GL_TEXTURE0);//установка текстуры
		m_pTexture->bind();//установка таргета на текстуру в памяти

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,nullptr);
		m_vertexArray.unbind();
	}

	//установка позиции
	void Sprite::setPosition(const glm::vec2& position) {
		m_position = position;
	}

	//установка размера
	void Sprite::setSize(const glm::vec2& size) {
		m_size = size;
	}

	//установка поворота
	void Sprite::setRotation(const float rotation) {
		m_rotation = rotation;
	}
}