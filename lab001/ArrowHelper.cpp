#include "ArrowHelper.h"

GLfloat ArrowHelper::vertices[] = {
    // Positions          // Normals     
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
};


ArrowHelper::ArrowHelper(Shader* shader) : Geometry(shader, glm::vec3(0, 0, 0), vertices, sizeof(vertices)) {
}

ArrowHelper::ArrowHelper(Shader* shader, glm::vec3 position) : Geometry(shader, position, vertices, sizeof(vertices)) {
	this->drawMode = GL_LINES;
}

void ArrowHelper::set(glm::vec3 start, glm::vec3 end) {
	GLfloat vertices[12];
	vertices[0] = start.x;
	vertices[1] = start.y;
	vertices[2] = start.z;

	vertices[3] = 0;
	vertices[4] = 1;
	vertices[5] = 0;

	vertices[6] = end.x;
	vertices[7] = end.y;
	vertices[8] = end.z;

	vertices[9] = 0;
	vertices[10] = 1;
	vertices[11] = 0;

	this->updateVertices(vertices, 48);
}