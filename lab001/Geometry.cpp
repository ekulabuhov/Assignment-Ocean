#include "Geometry.hpp"

Geometry::Geometry(Shader* shader, glm::vec3 position,
				   GLfloat vertices[], int sizeOfVertices) {
	Init(shader, position, vertices, sizeOfVertices);
}

Geometry::Geometry(Shader* shader, glm::vec3 position,
				   GLfloat vertices[], int sizeOfVertices, GLchar* texturePath) {
	
	int attribSize = texturePath ? 8 : 6;

	if (texturePath) {
		this->textureId = TextureLoader::loadTexture(texturePath);
	}

	Init(shader, position, vertices, sizeOfVertices, this->textureId, GL_TEXTURE_2D);
}

Geometry::Geometry(Shader* shader, glm::vec3 position,
				   GLfloat vertices[], int sizeOfVertices, GLuint texId, GLenum textureTarget) {
	Init(shader, position, vertices, sizeOfVertices, texId, textureTarget);
}

void Geometry::Init(Shader* shader, glm::vec3 position,
				   GLfloat vertices[], int sizeOfVertices, GLuint texId, GLenum textureTarget)
{
	this->textureId = texId;
	this->textureTarget = textureTarget;
	this->_shader = shader;
	this->_modelMatrix = glm::mat4();
	this->_modelMatrix = glm::translate(this->_modelMatrix, position);
	this->quaternion = glm::quat();
	this->drawMode = GL_TRIANGLES;

	int attribSize = textureTarget == GL_TEXTURE_2D ? 8 : 6;
	this->_triangleCount = sizeOfVertices / attribSize;

	/* Get GL to allocate space for our array buffers */
	glGenVertexArrays(1, &this->_VAO);
	glGenBuffers(1, &this->_VBO);
	//glGenBuffers(1, &this->_EBO);

	/* Tell GL we're now working on our Vertex Array Object */
	glBindVertexArray(this->_VAO);
	
	/* Give GL our vertices */
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, vertices, GL_STATIC_DRAW);

	///* Give GL our indices */
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, indices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	//glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attribSize * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attribSize * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	if (textureTarget == GL_TEXTURE_2D) {
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, attribSize * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	}

	/* Tell GL we don't need to work on our buffers any more */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Geometry::updateVertices(GLfloat vertices[], int sizeOfVertices) {
	/* Tell GL we're now working on our Vertex Array Object */
	glBindVertexArray(this->_VAO);
	
	/* Give GL our vertices */
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, vertices, GL_STATIC_DRAW);
}

void Geometry::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	_shader->enableShader();
	_shader->setUniformMatrix4fv("projection", projectionMatrix);
	_shader->setUniformMatrix4fv("view", viewMatrix);
	_shader->setUniformMatrix4fv("model", _modelMatrix);

	/* Tell GL we want to work on our VAO */
	glBindVertexArray(this->_VAO);
	if (this->textureId) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(this->textureTarget, this->textureId);
	}
	glDrawArrays(this->drawMode, 0, this->_triangleCount);
	glBindTexture(this->textureTarget, 0);
	glBindVertexArray(0);
}

void Geometry::setShader(Shader* s)
{
	this->_shader = s;
}

void Geometry::setShader(std::string shader)
{
	Shader* s = ShaderManager::getShader(shader);
	if (s != NULL)
	{
		this->setShader(s);
	}
}

Shader* Geometry::getShader()
{
	return this->_shader;
}

void Geometry::rotate(glm::vec3 rotation)
{
	this->_modelMatrix = glm::rotate(this->_modelMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	this->_modelMatrix = glm::rotate(this->_modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	this->_modelMatrix = glm::rotate(this->_modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Geometry::setRotationFromQuaternion(glm::quat quat) 
{
	auto position = this->getPosition();
	this->_modelMatrix = glm::mat4_cast(quat);
	this->setPosition(position);
	this->quaternion = quat;
}

glm::vec3 Geometry::getUpVector()
{
	auto upMatrix = this->_modelMatrix[1];
	return glm::vec3(upMatrix.x, upMatrix.y, upMatrix.z);
}

glm::vec3 Geometry::getPosition()
{
	auto upMatrix = this->_modelMatrix[3];
	return glm::vec3(upMatrix.x, upMatrix.y, upMatrix.z);
}

void Geometry::move(glm::vec3 vector) 
{
	this->_modelMatrix = glm::translate(this->_modelMatrix, vector);
}

void Geometry::translate(glm::vec3 vector) 
{
	// alias for move
	this->move(vector);
}

void Geometry::setPosition(glm::vec3 position) 
{
	if (position.x == position.x) {
		this->_modelMatrix[3] = glm::vec4(position, 1);
	} else {
		throw;
	}
}

void Geometry::scale(glm::vec3 scale)
{
	this->_modelMatrix = glm::scale(this->_modelMatrix, scale);
}

glm::vec3 Geometry::localToWorld(glm::vec3 localPoint)
{
	return glm::vec3(this->_modelMatrix * glm::vec4(localPoint, 1));
}

