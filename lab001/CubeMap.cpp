#include "CubeMap.hpp"

CubeMap::CubeMap()
{
	static float SIZE = 500.0f;

	float points[] = {
		-SIZE,  SIZE, -SIZE,
		-SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		SIZE, -SIZE, -SIZE,
		SIZE, -SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE,  SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		-SIZE,  SIZE, -SIZE,
		SIZE,  SIZE, -SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		SIZE, -SIZE,  SIZE
	};

	glGenBuffers (1, &this->_VBO);
	glBindBuffer (GL_ARRAY_BUFFER, this->_VBO);
	glBufferData (GL_ARRAY_BUFFER, 3 * 36 * sizeof (float), &points, GL_STATIC_DRAW);


	glGenVertexArrays (1, &this->_VAO);
	glBindVertexArray (this->_VAO);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, this->_VBO);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);

	ilInit();
}

void CubeMap::use()
{
}


void CubeMap::loadCubeMap(const char* mapFolder){
	std::stringstream map[6];
	const char* format = ".jpg";

	map[0] << mapFolder << "negz" << format;
	map[1] << mapFolder << "posz" << format;
	map[2] << mapFolder << "posy" << format;
	map[3] << mapFolder << "negy" << format;
	map[4] << mapFolder << "negx" << format;
	map[5] << mapFolder << "posx" << format;

	std::cout << map[0].str().c_str() << std::endl;
	std::cout << map[1].str().c_str() << std::endl;
	std::cout << map[2].str().c_str() << std::endl;
	std::cout << map[3].str().c_str() << std::endl;
	std::cout << map[4].str().c_str() << std::endl;
	std::cout << map[5].str().c_str() << std::endl;

	this->create_cube_map(
		map[0].str().c_str(),
		map[1].str().c_str(),
		map[2].str().c_str(),
		map[3].str().c_str(),
		map[4].str().c_str(),
		map[5].str().c_str(),
		&texID
		);
}

void CubeMap::create_cube_map (
	const char* front,
	const char* back,
	const char* top,
	const char* bottom,
	const char* left,
	const char* right,
	GLuint* tex_cube
	) {
		// generate a cube-map texture to hold all the sides
		glActiveTexture (GL_TEXTURE0);
		glGenTextures (1, tex_cube);

		// load each image and copy into a side of the cube-map texture
		load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
		load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
		load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
		load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
		load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
		load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
		// format cube map texture
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

bool CubeMap::load_cube_map_side (
	GLuint texture, GLenum side_target, const char* file_name
	) {
		glBindTexture (GL_TEXTURE_CUBE_MAP, texture);
		ILuint imageID;
		ilGenImages(1, &imageID);
		ilBindImage(imageID);
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_UPPER_LEFT/*IL_ORIGIN_LOWER_LEFT*/);

		ILboolean success = ilLoadImage((ILstring)file_name);

		if(success){
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);


			// copy image data into 'target' side of cube map
			glTexImage2D (
				side_target,
				0,
				GL_RGBA,
				ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT),
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				ilGetData()
				);
			return true;
		}else{
			std::cout << "Couldn't load texture: " << file_name << std::endl;
		}
}

void CubeMap::drawSkyBox(Shader shader, glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
	glDepthMask(GL_FALSE);
	shader.enableShader();
	shader.setUniformMatrix4fv("projection", projectionMatrix);
	// remove the translation part of the matrix to create an effect of infinite environment
	shader.setUniformMatrix4fv("view", glm::mat4(glm::mat3(viewMatrix)));

	glBindVertexArray(this->_VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->texID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}