#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include "ParticleSystem.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(257.0f, 56.0f, -64.0f), glm::vec3(0.0f, 1.0f, 0.0f), -271.0f, -12.6f);
//Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int cubeVAO = 0;

glm::vec3 Skin = glm::vec3(231 / 255.0f, 200 / 255.0f, 173 / 255.0f);
glm::vec3 Blue = glm::vec3(0.25f, 0.25f, 0.85f);
glm::vec3 Green = glm::vec3(0.25f, 0.85f, 0.25f);

glm::vec3 treePosition1{-30.0f, -2.5f, 243.0f};
glm::vec3 treePosition2{0.0f, -2.5f, 243.0f };

glm::vec3 particlePosition1{ 0.0f, -2.0f, 100.0f };
glm::vec3 particlePosition2{ 0.0f, -2.0f, 100.0f };
glm::vec3 particlePosition3{ 0.0f, -2.0f, 100.0f };
glm::vec3 particlePosition4{ 0.0f, -2.0f, 100.0f };
glm::vec3 particlePosition5{ 0.0f, -2.0f, 100.0f };
glm::vec3 particlePosition6{ 0.0f, -2.0f, 100.0f };
glm::vec3 particlePosition7{ 0.0f, -2.0f, 100.0f };
glm::vec3 particlePosition8{ 0.0f, -2.0f, 100.0f };

std::vector<glm::vec3> particlePositions
{
	{ -40.0f, -2.0f, 100.0f },
	{ -40.0f, -2.0f, 120.0f },
	{ -40.0f, -2.0f, 140.0f },
	{ -40.0f, -2.0f, 160.0f },
	{ -40.0f, -2.0f, 180.0f },
	{ -40.0f, -2.0f, 200.0f },
	{ -40.0f, -2.0f, 220.0f },
	{ -60.0f, -2.0f, 240.0f },
	{ -60.0f, -2.0f, 100.0f },
	{ -60.0f, -2.0f, 120.0f },
	{ -60.0f, -2.0f, 140.0f },
	{ -60.0f, -2.0f, 160.0f },
	{ -60.0f, -2.0f, 180.0f },
	{ -60.0f, -2.0f, 200.0f },
	{ -60.0f, -2.0f, 220.0f },
	{ -60.0f, -2.0f, 240.0f },
	{ 60.0f, -2.0f, 100.0f },
	{ 60.0f, -2.0f, 120.0f },
	{ 60.0f, -2.0f, 140.0f },
	{ 60.0f, -2.0f, 160.0f },
	{ 60.0f, -2.0f, 180.0f },
	{ 60.0f, -2.0f, 200.0f },
	{ 60.0f, -2.0f, 220.0f },
	{ 60.0f, -2.0f, 240.0f },
	{ 80.0f, -2.0f, 100.0f },
	{ 80.0f, -2.0f, 120.0f },
	{ 80.0f, -2.0f, 140.0f },
	{ 80.0f, -2.0f, 160.0f },
	{ 80.0f, -2.0f, 180.0f },
	{ 80.0f, -2.0f, 200.0f },
	{ 80.0f, -2.0f, 220.0f },
	{ 80.0f, -2.0f, 240.0f }
};

int name[6][36] =
{
	{ 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0 },
	{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0 },
	{ 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0 },
	{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0 },
	{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0 },
	{ 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0 },
};

ParticleSystem particleSystem;

glm::vec3 position = glm::vec3(324.0f, 14.0f, 102.0f);

struct BoundingBox
{
	glm::vec3 min;
	glm::vec3 max;
};

std::vector<BoundingBox> aabbs;

struct Sphere
{
	glm::vec3 center;
	float radius;
};

Sphere player;

BoundingBox calculateBoundingBox(const std::vector<glm::vec3>& vertices)
{
	glm::vec3 min = vertices[0];
	glm::vec3 max = vertices[0];

	for (const auto& vertex : vertices)
	{
		min.x = std::min(min.x, vertex.x);
		min.y = std::min(min.y, vertex.y);
		min.z = std::min(min.z, vertex.z);

		max.x = std::max(max.x, vertex.x);
		max.y = std::max(max.y, vertex.y);
		max.z = std::max(max.z, vertex.z);
	}

	return { min, max };
}

bool checkSphereBoundingBoxIntersection(const Sphere& sphere, BoundingBox box)
{
	// 计算最接近的点
	glm::vec3 closestPoint;
	closestPoint.x = glm::clamp(sphere.center.x, box.min.x, box.max.x);
	closestPoint.y = glm::clamp(sphere.center.y, box.min.y, box.max.y);
	closestPoint.z = glm::clamp(sphere.center.z, box.min.z, box.max.z);

	// 计算球心到最近点的距离
	float distance = glm::distance(sphere.center, closestPoint);

	//std::cout << "distance:" << distance << std::endl;

	return distance <= sphere.radius; // 如果距离小于等于球体半径，则相交
}

std::vector<glm::vec3> getVertices(const Model& model)
{
	std::vector<glm::vec3> vertices;

	for (const auto& mesh : model.meshes)
	{
		for (const auto& vertex : mesh.vertices)
		{
			vertices.emplace_back(vertex.Position);
		}
	}

	return vertices;
}

void addAABB(const Model& model)
{
	auto vertices = getVertices(model);

	aabbs.emplace_back(calculateBoundingBox(vertices));
}

void drawCube(Shader& shader, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& color = glm::vec3(1.0f))
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

	model = glm::scale(model, scale);

	shader.setMat4("model", model);
	shader.setVec3("color", color);

	glBindVertexArray(cubeVAO);
	glActiveTexture(GL_TEXTURE0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void drawHuman(Shader shader, const glm::vec3& position, const glm::vec3& skinColor, const glm::vec3& clothColor, const glm::vec3& shoesColor)
{
	// 头
	drawCube(shader, position + glm::vec3(0.0f, 1.6f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), skinColor);

	// 躯干
	drawCube(shader, position + glm::vec3(0.0f, 1.13f, 0.0f), glm::vec3(0.3f, 0.6f, 0.6f), clothColor);

	// 腿
	drawCube(shader, position + glm::vec3(0.0f, 0.4f, -0.15f), glm::vec3(0.2f, 0.8f, 0.2f), skinColor);
	drawCube(shader, position + glm::vec3(0.0f, 0.4f, 0.15f), glm::vec3(0.2f, 0.8f, 0.2f), skinColor);

	// 鞋子
	drawCube(shader, position + glm::vec3(0.0f, 0.0f, -0.15f), glm::vec3(0.3f, 0.1f, 0.25f), shoesColor);
	drawCube(shader, position + glm::vec3(0.0f, 0.0f, 0.15f), glm::vec3(0.3f, 0.1f, 0.25f), shoesColor);

	// 手臂
	drawCube(shader, position + glm::vec3(0.0f, 1.1f, -0.4f), glm::vec3(0.2f, 0.6f, 0.15f), skinColor);
	drawCube(shader, position + glm::vec3(0.0f, 1.1f, 0.4f), glm::vec3(0.2f, 0.6f, 0.15f), skinColor);
}

void drawHuman90(Shader shader, const glm::vec3& position, const glm::vec3& skinColor, const glm::vec3& clothColor, const glm::vec3& shoesColor)
{
	// 头
	drawCube(shader, position + glm::vec3(0.0f, 1.6f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), skinColor);

	// 躯干
	drawCube(shader, position + glm::vec3(0.0f, 1.13f, 0.0f), glm::vec3(0.6f, 0.6f, 0.3f), clothColor);

	// 腿
	drawCube(shader, position + glm::vec3(-0.15f, 0.4f, 0.0f), glm::vec3(0.2f, 0.8f, 0.2f), skinColor);
	drawCube(shader, position + glm::vec3(0.15f, 0.4f, 0.0f), glm::vec3(0.2f, 0.8f, 0.2f), skinColor);

	// 鞋子
	drawCube(shader, position + glm::vec3(-0.15f, 0.0f, 0.0f), glm::vec3(0.25f, 0.1f, 0.3f), shoesColor);
	drawCube(shader, position + glm::vec3(0.15f, 0.0f, 0.0f), glm::vec3(0.25f, 0.1f, 0.3f), shoesColor);

	// 手臂
	drawCube(shader, position + glm::vec3(-0.4f, 1.1f, 0.0f), glm::vec3(0.15f, 0.6f, 0.2f), skinColor);
	drawCube(shader, position + glm::vec3(0.4f, 1.1f, 0.0f), glm::vec3(0.15f, 0.6f, 0.2f), skinColor);
}

// shader:		使用的shader
// position:	小人的起始位置
// space:		小人间隔
// count:		小人个数
// skinColor:   皮肤颜色
// clothColor:  衣服颜色
// shoesColor:  鞋子颜色
void drawHumanGroup(Shader shader, const glm::vec3& position, float space, int count, const glm::vec3& skinColor, const glm::vec3& clothColor, const glm::vec3& shoesColor)
{
	for (int i = 0; i < count; i++)
	{
		drawHuman(shader, position + glm::vec3(0.0f, 0.0f, space * i), skinColor, clothColor, shoesColor);
	}
}

void drawHumanGroup90(Shader shader, const glm::vec3& position, float space, int count, const glm::vec3& skinColor, const glm::vec3& clothColor, const glm::vec3& shoesColor)
{
	for (int i = 0; i < count; i++)
	{
		drawHuman90(shader, position + glm::vec3(space * i, 0.0f, 0.0f), skinColor, clothColor, shoesColor);
	}
}

void drawTreeGroup(Model& model, Shader shader, const glm::vec3& position, const glm::vec3& direction, float space, int count)
{
	for (int i = 0; i < count; i++)
	{
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position + direction * static_cast<float>(space * i));

		shader.setMat4("model", translation);

		model.Draw(shader);
	}
}

void drawName(Shader shader, const glm::vec3& position, float scale)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 35; j >= 0; j--)
		{
			if (name[i][j] == 1)
			{
				drawCube(shader, position + glm::vec3(-j * scale, -i * scale, 0.0f), glm::vec3(scale));
			}
		}
	}
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, keyCallback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// build and compile shaders
	// -------------------------
	Shader shader("shaders/cubemaps.vert", "shaders/cubemaps.frag");
	Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");
	Shader modelShader("shaders/model_loading.vert", "shaders/model_loading.frag");

	Model stadiums("models/stadiums.obj");
	Model road("models/road.obj");
	Model terrain("models/terrain.obj");
	Model tree("models/tree.obj");
	Model buildings("models/buildings.obj");
	Model grass("models/grass.obj");
	Model ground("models/ground.obj");
	Model path("models/path.obj");
	Model cars("models/cars.obj");

	addAABB(buildings);
	addAABB(stadiums);

	particleSystem.initScene();

	float cubeVertices[] = {
		// positions          // normals
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	//
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// load textures
	// -------------
	vector<std::string> faces
	{
		"textures/skybox/right.jpg",
		"textures/skybox/left.jpg",
		"textures/skybox/top.jpg",
		"textures/skybox/bottom.jpg",
		"textures/skybox/front.jpg",
		"textures/skybox/back.jpg",
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	// shader configuration
	// --------------------
	shader.use();
	shader.setInt("skybox", 0);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	player.center = camera.Position;
	player.radius = 10.0f;

	camera.MovementSpeed *= 10.0f;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		particleSystem.update(float(glfwGetTime()));

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw scene as normal
		shader.use();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("cameraPos", camera.Position);

		shader.setVec3("lightPosition", camera.Position);

		drawHumanGroup90(shader, glm::vec3(245.0f, -2.0f, 100.0f), 5.0f, 5, Skin, Blue, Blue);

		drawCube(shader, position + glm::vec3(-8.0f, -1.25f, 0.2f), glm::vec3(20.0f, 4.0f, 0.2f), glm::vec3(0.5f));

		drawName(shader, position, 0.5f);

		modelShader.use();

		modelShader.setMat4("model", model);
		modelShader.setMat4("view", view);
		modelShader.setMat4("projection", projection);

		stadiums.Draw(modelShader);

		terrain.Draw(modelShader);

		road.Draw(modelShader);

		buildings.Draw(modelShader);

		grass.Draw(modelShader);

		path.Draw(modelShader);

		ground.Draw(modelShader);

		cars.Draw(modelShader);

		drawTreeGroup(tree, modelShader, glm::vec3(150.0f, -2.5f, 80.0f), glm::vec3(1.0f, 0.0f, 0.0f), 10.0f, 20);

		auto rotation = glm::rotate(glm::mat4(1.0f), glm::radians(64.8737f), glm::vec3(0.0f, 1.0f, 0.0f));

		auto direction = rotation * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		drawTreeGroup(tree, modelShader, treePosition1, direction, 10.0f, 19);
		drawTreeGroup(tree, modelShader, treePosition2, direction, 10.0f, 19);

		for (int i = 0; i < particlePositions.size(); i++)
		{
			model = glm::translate(glm::mat4(1.0f), particlePositions[i]);
			model = glm::scale(model, glm::vec3(20.0f, 10.0f, 20.0f));

			particleSystem.render(model, view, projection);
		}

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glActiveTexture(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		//camera.ProcessKeyboard(DOWN, deltaTime);
		position.y -= 0.1f;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(UP, deltaTime);
		position.y += 0.1f;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT))
	{
		//camera.ProcessMouseMovement(-1.0f, 0.0f);
		position.x -= 0.1f;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT))
	{
		//camera.ProcessMouseMovement(1.0f, 0.0f);
		position.x += 0.1f;
	}

	if (glfwGetKey(window, GLFW_KEY_UP))
	{
		//camera.ProcessMouseMovement(0.0f, 1.0f);
		position.z -= 0.1f;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN))
	{
		//camera.ProcessMouseMovement(9.0f, -1.0f);
		position.z += 0.1f;
	}

	player.center = camera.Position;

	//for (const auto& aabb : aabbs)
	//{
	//	if (checkSphereBoundingBoxIntersection(player, aabb))
	//	{
	//		camera.UndoProcessKeyboard(deltaTime);
	//	}
	//}

	std::cout << "x = " << camera.Position.x 
		      << " y = " << camera.Position.y 
			  << " z = " << camera.Position.z 
		      << " Yaw = " << camera.Yaw 
			  << " Pitch = " << camera.Pitch << std::endl;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{

	}
}