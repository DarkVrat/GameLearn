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
			0.f, 0.f,//1.1				2-----3  1
			0.f, 1.f,//1.2				|    /  /|
			1.f, 1.f,//1.3				| 1 /  / |
					 //					|  /  /  |
			1.f, 1.f,//2.1				| /  / 2 |
			1.f, 0.f,//2.2				|/  /    |
			0.f, 0.f //2.3				1  3-----2
		};
		auto subTexture = m_pTexture->getSubTexture2D(std::move(initialSubTexture)); //загрузка подтекстуры или всей текстуры
		//установка координат текстуры к вертексам
		const GLfloat textureCoords[] = {
			//U						   //V
			subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,	//1.1
			subTexture.leftBottomUV.x, subTexture.rightTopUV.y,		//1.2
			subTexture.rightTopUV.x,   subTexture.rightTopUV.y,		//1.3
																	//
			subTexture.rightTopUV.x,   subTexture.rightTopUV.y,		//2.1
			subTexture.rightTopUV.x,   subTexture.leftBottomUV.y,	//2.2
			subTexture.leftBottomUV.x, subTexture.leftBottomUV.y	//2.3
		};

		glGenVertexArrays(1, &m_VAO); //создание добавление 1 вертексного массива в массив, и передача её номера в m_ID 
		glBindVertexArray(m_VAO); //связка на вертексный массив

		glGenBuffers(1, &m_vartexCoordsVBO); //создание вертексного буфера
		glBindBuffer(GL_ARRAY_BUFFER, m_vartexCoordsVBO); //связка к буферу
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), &vertexCoords, GL_STATIC_DRAW); //загрузка вертексного массива
		glEnableVertexAttribArray(0); //Включение буфера 0
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr); //установка параметров 0 буфера, 2 числа на 1 вертекс, float тип, параметр нормализации (то есть перевод из int в float), смещение между данными, и смещение к первому элементу 

		glGenBuffers(1, &m_textureCoordsVBO); //так же как и для вертексного
		glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), &textureCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, 0);//отмена привязки к буферу
		glBindVertexArray(0);//отмена привязки к вертексному массиву
	}

	//очистка буферов
	Sprite::~Sprite() {
		glDeleteBuffers(1, &m_vartexCoordsVBO);
		glDeleteBuffers(1, &m_textureCoordsVBO);
		glDeleteVertexArrays(1, &m_VAO);
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

		glBindVertexArray(m_VAO);//связка на вертексный массив
		m_pShaderProgram->setMatrix4("modelMat", model);//передача полученой матрицы в шейдер

		glActiveTexture(GL_TEXTURE0);//установка текстуры
		m_pTexture->bind();//установка таргета на текстуру в памяти

		glDrawArrays(GL_TRIANGLES, 0, 6);//отрисовка 2 треугольников
		glBindVertexArray(0);//освобождение привязки
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