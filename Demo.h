#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>
#include <vector>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shaderProgram, VBO, VAO, EBO, texture, VBO2, VAO2, EBO2, texture2, VBO3, VAO3, EBO3, texture3, VBO4, VAO4, EBO4, texture4;
	float angle = 0;
	float position = 10;
	float positionx = 5;

	float t2 = 0;
	float t3 = 0;
	float t4 = 0;


	//float xArr[1000];

	int idx = 0;
	std::vector<float> xWay;
	std::vector<float> zWay;
	std::vector<float> rotasi;
	//float zWay[5000];

	float rotateX = 0;
	float translateX = 0;
	float translateZ = 0;

	float rx = 0;

	float dotProd = 0;
	float positionBalingBelakang = 17;
	bool pos = false;
	float angleHeli = 0.0f;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow* window);
	void BuildColoredCube();
	void BuildColoredPlane();
	void BuildColoredBaling();
	void DrawColoredBaling();
	void BuildColoredBalingBelakang();
	void DrawColoredBalingBelakang();
	void DrawColoredCube();
	void DrawColoredPlane();
};
