#pragma once
#include "geometry.hpp"
class ArrowHelper :
	public Geometry
{
	static GLfloat vertices[];
public:
	ArrowHelper(Shader* shader);
	ArrowHelper(Shader* shader, glm::vec3 position);
	~ArrowHelper(void);
	void ArrowHelper::set(glm::vec3 start, glm::vec3 end);
};