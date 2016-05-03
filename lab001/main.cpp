#include <iostream>

#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <math.h>
#define _USE_MATH_DEFINES

#include "ShaderManager.hpp"
#include "CubeMap.hpp"
#include "Cube.hpp"
#include "Camera.h"
#include "model.h"
#include "Plane.h"
#include "Cone.h"
#include "ArrowHelper.h"
#include "Grid.h"
#include "FBTexture.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseKey_callback(GLFWwindow* window, int button, int action, int mods);
void do_movement();
void RenderScene(Shader shader, bool wireframe, bool renderOcean);

// Camera
Camera camera(glm::vec3(0.0f, 0.5f, 5.0f));

// Properties
GLuint SCR_WIDTH = 800, SCR_HEIGHT = 800;

GLFWwindow* window;
Cube *lamp;
Grid *grid1;
CubeMap* cubeMap;
Plane *reflectionPlane, *refractionPlane, *seaBed1, *seaBed2;
FBTexture *reflectionTexture, *refractionTexture;
bool keys[1024];
int xRot, yRot, zRot = 0;
GLfloat fov = 45.0f;
glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;
double lastMouseX, lastMouseY = 0;
bool firstMouse = true;
map<string,glm::quat> KinematicTransforms;	// maps a bone name to its transform

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Shader skyboxShader;
Shader reflectAndRefractShader;
Shader lightingShader;
Shader simpleDepthShader;
Shader shadowShader;
Shader waterShader;

glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 cube1Color(1.0f, 0.5f, 0.31f);

// maps 1:1 to screen -> 0,0 for bottom left
glm::mat4 orthogonalMatrix(glm::ortho(0.0f, float(SCR_WIDTH), 0.0f, float(SCR_HEIGHT)));

bool initWindow()
{
	if (!glfwInit())
	{
		std::cout << "ERROR: Could not initialise GLFW...";
		std::cin;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 800, "Assignment #6 Ocean - OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "ERROR: Could not create winodw...";
		std::cin;
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	glfwSetMouseButtonCallback(window, mouseKey_callback);

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		std::cout << "ERROR: Problem initialising GLEW: " << glewGetErrorString(err);
		std::cin;
		return false;
	}

	glViewport(0, 0, 800, 800);

	return true;
}

void mouseKey_callback(GLFWwindow* window, int button, int action, int mods)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	glfwSetCursorPosCallback(window, mouse_callback); 
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
		glfwSetCursorPosCallback(window, NULL); 
		firstMouse = true;
		//glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	float deltaX = xpos - lastMouseX;
	float deltaY = lastMouseY - ypos;
	lastMouseX = xpos;
	lastMouseY = ypos;

	camera.ProcessMouseMovement(deltaX, deltaY);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset / 100;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

#include "windows.h"
#include "MyForm.h"

using namespace lab001;

public ref class X
{
public:
	static MyForm^ myForm = gcnew MyForm();
	bool wireframe;
	float kludgeCoef, windspeed;
	void EntryPoint()
	{
		Application::EnableVisualStyles();
		Application::Run(myForm);
	}

	void Update()
	{
		this->wireframe = myForm->cbWireframe->Checked;
		this->kludgeCoef = myForm->tbKludgeCoef->Value / 10.0f;
		myForm->lblKludgeCoef->Text = "Kludge Coefficient: " + this->kludgeCoef * 10.0f;
		this->windspeed = myForm->tbWindspeed->Value / 10.0f;
		myForm->lblWindspeed->Text = "Windspeed: " + this->windspeed * 10.0f;
	}
};

using namespace System::Threading;

int main()
{
	X^ o1 = gcnew X();
	Thread^ thread = gcnew Thread(gcnew ThreadStart(o1, &X::EntryPoint));
	thread->Start();	
	
	/* Create GL Window */
	if (!initWindow())
		return -1;

	/* Initialise vertex buffers for cube */
	cubeMap = new CubeMap();
	cubeMap->loadCubeMap("../textures/cubemaps/Ocean/");

	/* Load shaders needed */
	skyboxShader = *ShaderManager::loadShader("skybox");
	reflectAndRefractShader = *ShaderManager::loadShader("reflectAndRefract");
	lightingShader = *ShaderManager::loadShader("lighting");
	simpleDepthShader = *ShaderManager::loadShader("simpleDepth");
	shadowShader = *ShaderManager::loadShader("shadow");
	waterShader = *ShaderManager::loadShader("water");

	// Setup correct textures
	//shadowShader.enableShader();
	//glUniform1i(glGetUniformLocation(shadowShader._shaderProgramID, "diffuseTexture"), 0);
    //glUniform1i(glGetUniformLocation(shadowShader._shaderProgramID, "shadowMap"), 1);

	lamp = new Cube(&shadowShader, lightPos);
	lamp->scale(glm::vec3(0.2f));

	reflectionTexture = new FBTexture();
	refractionTexture = new FBTexture();

	reflectionPlane = new Plane(&lightingShader, glm::vec3(1.0f, 1.0f, 0.0f), reflectionTexture->textureColorbuffer, GL_TEXTURE_2D);
	reflectionPlane->translate(glm::vec3(80.0f, 400.0f, 0.0f));
	reflectionPlane->rotate(glm::vec3(glm::radians(90.0f), 0, 0));
	reflectionPlane->scale(glm::vec3(75.0f, 1.0f, 75.0f));

	refractionPlane = new Plane(&lightingShader, glm::vec3(1.0f, 1.0f, 0.0f), refractionTexture->textureColorbuffer, GL_TEXTURE_2D);
	refractionPlane->translate(glm::vec3(80.0f, 600.0f, 0.0f));
	refractionPlane->rotate(glm::vec3(glm::radians(90.0f), 0, 0));
	refractionPlane->scale(glm::vec3(75.0f, 1.0f, 75.0f));

	seaBed1 = new Plane(&lightingShader, glm::vec3(-7.5f, -2.0f, 0.0f), "../textures/wood.png");
	seaBed1->scale(glm::vec3(7.5f, 1.0f, 15.0f));

	seaBed2 = new Plane(&lightingShader, glm::vec3(7.5f, -1.0f, 0.0f), "../textures/wood.png");
	seaBed2->scale(glm::vec3(7.5f, 1.0f, 15.0f));

	glm::vec3 grid1Position(0.0f, 0.0f, 0.0F);
	grid1 = new Grid(&waterShader, grid1Position, reflectionTexture->textureColorbuffer, refractionTexture->textureColorbuffer);
	grid1->rotate(glm::vec3(glm::radians(90.0f), 0, 0));

	projectionMatrix = glm::perspective(
		45.0f,
		1.0f,
		0.1f,
		1000.0f
	);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		/* Rendering Code */
		do_movement();
		viewMatrix = camera.GetViewMatrix();

		waterShader.enableShader();
		waterShader.setUniform1f("time", currentFrame);
		waterShader.setUniform1f("deltaTime", deltaTime);
		waterShader.setUniform1f("kludgeCoef", o1->kludgeCoef);
		waterShader.setUniform1f("windspeed", o1->windspeed);

		// Render reflection texture
		glBindFramebuffer(GL_FRAMEBUFFER, reflectionTexture->framebufferId);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float distance = camera.Position.y * 2;
		camera.Position.y -= distance;
		// invert the pitch
		camera.Pitch = -camera.Pitch;
		RenderScene(lightingShader, o1->wireframe, false);
		// move it back up
		camera.Position.y += distance;
		camera.Pitch = -camera.Pitch;

		// Render refraction texture
		glBindFramebuffer(GL_FRAMEBUFFER, refractionTexture->framebufferId);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderScene(lightingShader, o1->wireframe, false);
		
        /////////////////////////////////////////////////////
        // Bind to default framebuffer again and draw the 
        // quad plane with attched screen texture.
        // //////////////////////////////////////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderScene(lightingShader, o1->wireframe, true);
		// draw UI outside RenderScene to avoid infinite mirror
		reflectionPlane->draw(orthogonalMatrix, glm::mat4());
		refractionPlane->draw(orthogonalMatrix, glm::mat4());
		
		glfwSwapBuffers(window);
		glfwPollEvents();

		o1->myForm->Invoke(gcnew Action(o1, &X::Update));
	}

	glfwTerminate();
	thread->Abort();
	return 0;
}

long long m_startTime = GetTickCount();

void RenderScene(Shader shader, bool wireframe, bool renderOcean)
{
	viewMatrix = camera.GetViewMatrix();

	//shader.setUniformVector3f("lightPos", lightPos);
	//shader.setUniformVector3f("viewPos", camera.Position);
	//shader.setUniformVector3f("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));

	if (renderOcean) {
		if (wireframe) {
			// Enable wirefame mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}	
		grid1->draw(projectionMatrix, viewMatrix);
		// Disable wireframe mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	seaBed1->draw(projectionMatrix, viewMatrix);
	seaBed2->draw(projectionMatrix, viewMatrix);
	cubeMap->drawSkyBox(skyboxShader, projectionMatrix, viewMatrix);
}

// Moves/alters the object positions based on user input
void do_movement()
{
	    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
	if(keys[GLFW_KEY_SPACE])
        camera.ProcessKeyboard(UP, deltaTime);
	if(keys[GLFW_KEY_LEFT_SHIFT])
        camera.ProcessKeyboard(DOWN, deltaTime);
}