#pragma once
#include "geometry.hpp"
class Grid :
	public Geometry
{
public:
	Grid(Shader* shader, glm::vec3 position, GLuint texId, GLenum textureTarget);
	~Grid(void);

	static GLfloat vertices[];
	void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

private:
	GLuint reflectionTexId, refractionTexId;
};