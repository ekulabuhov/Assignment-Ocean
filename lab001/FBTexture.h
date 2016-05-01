#pragma once
#include <GL\glew.h>
#include <iostream>

class FBTexture
{
public:
	FBTexture(void);
	~FBTexture(void);
	GLuint framebufferId, textureColorbuffer;
private:
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
	static const int screenWidth = 800;
	static const int screenHeight = 800;
};

