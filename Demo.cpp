#include "Demo.h"
#include<conio.h>
#include<math.h>



Demo::Demo() {

}


Demo::~Demo() {
}

float dot_product(float vector_a[], float vector_b[]) {
	float product = 0;
	for (int i = 0; i < 2; i++)
		product = product + vector_a[i] * vector_b[i];
	return product;
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredCube();

	BuildColoredPlane();

	BuildColoredBaling();

	BuildColoredBalingBelakang();

	float xArr[24] = {
		-5.0f, -10.0f, -10.0f, //p0, p1, p2
		-10.0f, -10.0f, -10.0f, //Garis Lurus
		-10.f, -10.0f, -5.0f, //p3, p4, p5
		-5.0f, 0.0f, 5.0f, //Garis Lurus
		5.0f, 10.0f, 10.0f, //p6, p7, p8
		10.0f, 10.0f, 10.0f, // Garis Lurus
		10.f, 10.0f, 5.0f, //p9, p10, p11
		5.0f, 0.0f, -5.0f // Garis Lurus
	};

	float zArr[24] = {
		10.0f, 10.0f, 5.0f, //p0, p1, p2
		5.0f, 0.0f, -5.0f, //Garis Lurus
		-5.0f, -10.0f, -10.0f, //p3, p4, p5
		-10.0f, -10.0f, -10.0f, //Garis Lurus
		-10.0f, -10.0f, -5.0f, //p6, p7, p8
		-5.0f, 0.0f, 5.0f, //Garis Lurus
		5.0f, 10.0f, 10.0f, //p9, p10, p11
		10.0f, 10.0f, 10.0f // Garis Lurus
	};

	/*float vector_a[] = { -5.0f, 10.0f };
	float vector_b[] = { -10.0f, 5.0f, };

	dotProd = dot_product(vector_a, vector_b) / (sqrt(pow(vector_a[0],2)+pow(vector_a[1], 2)) * sqrt(pow(vector_b[0], 2) + pow(vector_b[1], 2)));*/



	float t = 0.0f;

	for (int i = 0; i < 24; i += 3) {
		for (int j = 0; j <= 500; j++) {
			t = (float)j / 500;

			

			float x = ((1 - t) * (1 - t) * xArr[i]) + (2 * (1 - t) * t * xArr[i + 1]) + (t * t * xArr[i + 2]);
			xWay.push_back(x);
			float z = ((1 - t) * (1 - t) * zArr[i]) + (2 * (1 - t) * t * zArr[i+1]) + (t * t * zArr[i+2]);
			zWay.push_back(z);

			float vector_a[] = { xArr[i], zArr[i] };
			float vector_b[] = { xArr[i + 2], zArr[i + 2] };

			float dp = dot_product(vector_a, vector_b) / (sqrt(pow(vector_a[0], 2) + pow(vector_a[1], 2)) * sqrt(pow(vector_b[0], 2) + pow(vector_b[1], 2)));
			rotasi.push_back(acos(dp));
		}
	}

}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &EBO3);
	glDeleteVertexArrays(1, &VAO4);
	glDeleteBuffers(1, &VBO4);
	glDeleteBuffers(1, &EBO4);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}



}

void Demo::Update(double deltaTime) {
	
	angle += (float)((deltaTime * 1.5f) / 40);

	translateX = xWay[idx];
	translateZ = zWay[idx];


	if (idx < 500 || idx >= 1000 && idx < 1500 || idx >= 2000 && idx < 2500 || idx >= 3000 && idx < 3500) {
		float vector_a[] = { xWay[idx], zWay[idx]};
		float vector_b[] = { xWay[idx+1], zWay[idx+1]};

		dotProd = dot_product(vector_a, vector_b) / (sqrt(pow(vector_a[0], 2) + pow(vector_a[1], 2)) * sqrt(pow(vector_b[0], 2) + pow(vector_b[1], 2)));


		rx -= acos(dotProd) * 2.45 ;
	}


	std::cout << "\nrX: " << dotProd;


	idx++;
	if (idx == 4000) {
		idx = 0;
	}
	
	
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(0, 25, 25), glm::vec3(translateX, 0, translateZ ), glm::vec3(0, 1, 0));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawColoredCube();

	DrawColoredPlane();

	DrawColoredBaling();

	DrawColoredBalingBelakang();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildColoredCube() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.8, -0.5, 0.5, 0, 0,  // 0
		0.8, -0.5, 0.5, 1, 0,   // 1
		0.8,  0.5, 0.5, 1, 1,   // 2
		-0.8,  0.5, 0.5, 0, 1,  // 3

		// right
		0.8,  0.5,  0.5, 0, 0,  // 4
		0.8,  0.5, -0.5, 1, 0,  // 5
		0.8, -0.5, -0.5, 1, 1,  // 6
		0.8, -0.5,  0.5, 0, 1,  // 7


		// back
		-0.8, -0.5, -0.5, 0, 0, // 8 
		0.8,  -0.5, -0.5, 1, 0, // 9
		0.8,   0.5, -0.5, 1, 1, // 10
		-0.8,  0.5, -0.5, 0, 1, // 11

		// left
		-0.8, -0.5, -0.5, 0, 0, // 12
		-0.8, -0.5,  0.5, 1, 0, // 13
		-0.8,  0.5,  0.5, 1, 1, // 14
		-0.8,  0.5, -0.5, 0, 1, // 15

		// upper
		0.8, 0.5,  0.5, 0, 0,   // 16
		-0.8, 0.5,  0.5, 1, 0,  // 17
		-0.8, 0.5, -0.5, 1, 1,  // 18
		0.8, 0.5, -0.5, 0, 1,   // 19

		// bottom
		-0.8, -0.5, -0.5, 0, 0, // 20
		0.8, -0.5, -0.5, 1, 0,  // 21
		0.8, -0.5,  0.5, 1, 1,  // 22
		-0.8, -0.5,  0.5, 0, 1, // 23

		//Belakang Depan
		0.8, -0.5, 0.5, 0, 0,  // 24
		2.5, 0.2, 0.1, 1, 0,   // 25
		2.5,  0.5, 0.1, 1, 1,   // 26
		0.8,  0.5, 0.5, 0, 1,  // 27

		//// right
		2.5,  0.5,  0.1, 0, 0,  // 28
		2.5,  0.5, -0.1, 1, 0,  // 29
		2.5, 0.2, -0.1, 1, 1,  // 30
		2.5, 0.2,  0.1, 0, 1,  // 31


		//// back
		0.8, -0.5, -0.5, 0, 0, // 32
		2.5, 0.2, -0.1, 1, 0, // 33
		2.5, 0.5, -0.1, 1, 1, // 34
		0.8, 0.5, -0.5, 0, 1, // 35

		//// upper
		2.5, 0.5,  0.1, 0, 0,   // 36
		0.8, 0.5,  0.5, 1, 0,  // 37
		0.8, 0.5, -0.5, 1, 1,  // 38
		2.5, 0.5, -0.1, 0, 1,   // 39

		//// bottom
		0.8, -0.5, -0.5, 0, 0, // 40
		2.5, 0.2, -0.1, 1, 0,  // 41
		2.5, 0.2,  0.1, 1, 1,  // 42
		0.8, -0.5,  0.5, 0, 1, // 43

		//Atas Depan
		-0.1, 0.5, 0.1, 0, 0,  // 44
		0.1, 0.5, 0.1, 1, 0,   // 45
		0.1,  0.65, 0.1, 1, 1,   // 46
		-0.1,  0.65, 0.1, 0, 1,  // 47

		// right
		0.1,  0.65,  0.1, 0, 0,  // 48
		0.1,  0.65, -0.1, 1, 0,  // 49
		0.1, 0.5, -0.1, 1, 1,  // 50
		0.1, 0.5,  0.1, 0, 1,  // 51


		// back
		-0.1, 0.5, -0.1, 0, 0, // 52
		0.1,  0.5, -0.1, 1, 0, // 53
		0.1,   0.65, -0.1, 1, 1, // 54
		-0.1,  0.65, -0.1, 0, 1, // 55

		// left
		-0.1, 0.5, -0.1, 0, 0, // 56
		-0.1, 0.5,  0.1, 1, 0, // 57
		-0.1,  0.65,  0.1, 1, 1, // 58
		-0.1,  0.65, -0.1, 0, 1, // 59

		// Buntut atas depan
		2.5, 0.2, 0.1, 0, 0, //60
		2.7, 0.2, 0.1, 1, 0, //61
		3, 1, 0.1, 1, 1, //62
		2.5,  0.5, 0.1, 0, 1, // 63

		// Kanan
		3, 1, 0.1, 0, 0, //64
		3, 1, -0.1, 1, 0, //65
		2.7, 0.2, -0.1, 1, 1, //66
		2.7, 0.2, 0.1, 0, 1, //67

		// Belakang
		2.5, 0.2, -0.1, 0, 0, //68
		2.7, 0.2, -0.1, 1, 0, //69
		3, 1, -0.1, 1, 1, //70
		2.5,  0.5, -0.1, 0, 1, // 71

		// Upper
		3, 1, 0.1, 0, 0, // 72
		2.5, 0.5, 0.1, 1, 0, // 73
		2.5, 0.5, -0.1, 1, 1, // 74
		3, 1, -0.1, 0, 1, // 75

		// Bottom
		2.7, 0.2, -0.1, 0, 0, // 76
		2.5, 0.2, -0.1, 1, 0, // 77
		2.5, 0.2, 0.1, 0, 1, // 78
		2.7, 0.2, 0.1, 1, 1, // 79

		// Buntut Bawah Front
		2.6, -0.1, 0.1, 0, 0, // 80
		2.8, -0.1, 0.1, 1, 0, // 81
		2.7, 0.2, 0.1, 0, 1, // 82
		2.4, 0.2, 0.1, 1, 1, // 83

		// Right
		2.7, 0.2, 0.1, 0, 0, // 84
		2.7, 0.2, -0.1, 1, 0, // 85
		2.8, -0.1, -0.1, 0, 1, // 86
		2.8, -0.1, 0.1, 1, 1, // 87

		// Back
		2.6, -0.1, -0.1, 0, 0, // 88
		2.8, -0.1, -0.1, 1, 0, // 89
		2.7, 0.2, -0.1, 0, 1, // 90
		2.4, 0.2, -0.1, 1, 1, // 91

		// Left
		2.6, -0.1, -0.1, 0, 0, // 92
		2.6, -0.1, 0.1, 1, 0, // 93
		2.5, 0.2, 0.1, 0, 1, // 94
		2.5, 0.2, -0.1, 1, 1, // 95

		// Botton
		2.8, -0.1, -0.1, 0, 0, // 96
		2.6, -0.1, -0.1, 1, 0, // 97
		2.6, -0.1, 0.1, 0, 1, // 98
		2.8, -0.1, 0.1, 1, 1, // 99


	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,  // bottom
		24, 25, 26, 24, 26, 27, // Ekor Helikopter Front
		28, 29, 30, 28, 30, 31,	// Right
		32, 33, 34, 32, 34, 35,	// Back
		36, 38, 37, 36, 39, 38,	// Upper
		40, 42, 41, 40, 43, 42,	// Bottom
		44, 45, 46, 44, 46, 47,	// Atas Helikopter front
		48, 49, 50, 48, 50, 51,	// Right
		52, 53, 54, 52, 54, 55,	// Back
		56, 58, 57, 56, 59, 58,	// Left
		60, 61, 62, 60, 62, 63, // Buntut atas depan
		64, 65, 66, 64, 66, 67, // Right
		68, 69, 70, 68, 70, 71, // Back
		72, 74, 73, 72, 75, 74, // Upper
		76, 78, 77, 76, 79, 78, // Bottom
		80, 81, 82, 80, 82, 83, // Buntut bawah depan
		84, 85, 86, 84, 86, 87, // Right
		88, 89, 90, 88, 90, 91, // Back
		92, 94, 93, 92, 95, 94, // left
		96, 98, 97, 96, 99, 98, // Bottom



	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}


void Demo::DrawColoredCube()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;


	model = glm::translate(model, glm::vec3(translateX, 0, translateZ));
	
	model = glm::rotate(model, rx , glm::vec3(0, 1 , 0));

	//model = glm::rotate(model, z, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1, 1, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("grass.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	//glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 0, GL_RGB, width, height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, 0, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0,  0,
		 50.0, -0.5, -50.0, 50,  0,
		 50.0, -0.5,  50.0, 50, 50,
		-50.0, -0.5,  50.0,  0, 50
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}



void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}


void Demo::BuildColoredBaling() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-1.2, 0.65, 0.2, 0, 0,  // 0
		1.2, 0.65, 0.2, 1, 0,   // 1
		1.2,  0.7, 0.2, 1, 1,   // 2
		-1.2,  0.7, 0.2, 0, 1,  // 3

		// right
		1.2,  0.7,  0.2, 0, 0,  // 4
		1.2,  0.7, -0.2, 1, 0,  // 5
		1.2, 0.65, -0.2, 1, 1,  // 6
		1.2, 0.65,  0.2, 0, 1,  // 7


		// back
		-1.2, 0.65, -0.2, 0, 0, // 8 
		1.2,  0.65, -0.2, 1, 0, // 9
		1.2,   0.7, -0.2, 1, 1, // 10
		-1.2,  0.7, -0.2, 0, 1, // 11

		// left
		-1.2, 0.65, -0.2, 0, 0, // 12
		-1.2, 0.65,  0.2, 1, 0, // 13
		-1.2,  0.7,  0.2, 1, 1, // 14
		-1.2,  0.7, -0.2, 0, 1, // 15

		// upper
		1.2, 0.7,  0.2, 0, 0,   // 16
		-1.2, 0.7,  0.2, 1, 0,  // 17
		-1.2, 0.7, -0.2, 1, 1,  // 18
		1.2, 0.7, -0.2, 0, 1,   // 19

		// bottom
		-1.2, 0.65, -0.2, 0, 0, // 20
		1.2, 0.65, -0.2, 1, 0,  // 21
		1.2, 0.65,  0.2, 1, 1,  // 22
		-1.2, 0.65,  0.2, 0, 1, // 23

		//==================2=========
		// front
		-0.2, 0.65, 1.2, 0, 0,  // 0 24
		0.2, 0.65, 1.2, 1, 0,   // 1 25
		0.2,  0.7, 1.2, 1, 1,   // 2 26
		-0.2,  0.7, 1.2, 0, 1,  // 3 27

		// right
		0.2,  0.7,  1.2, 0, 0,  // 4 28
		0.2,  0.7, -1.2, 1, 0,  // 5 29
		0.2, 0.65, -1.2, 1, 1,  // 6 30
		0.2, 0.65,  1.2, 0, 1,  // 7 31


		// back
		-0.2, 0.65, -1.2, 0, 0, // 8 32
		0.2,  0.65, -1.2, 1, 0, // 9 33
		0.2,   0.7, -1.2, 1, 1, // 10 34
		-0.2,  0.7, -1.2, 0, 1, // 11 35

		// left
		-0.2, 0.65, -1.2, 0, 0, // 12 36
		-0.2, 0.65,  1.2, 1, 0, // 13 37
		-0.2,  0.7,  1.2, 1, 1, // 14 38
		-0.2,  0.7, -1.2, 0, 1, // 15 39

		// upper
		0.2, 0.7,  1.2, 0, 0,   // 16 40
		-0.2, 0.7,  1.2, 1, 0,  // 17 41
		-0.2, 0.7, -1.2, 1, 1,  // 18 42
		0.2, 0.7, -1.2, 0, 1,   // 19 43

		// bottom
		-0.2, 0.65, -1.2, 0, 0, // 20 44
		0.2, 0.65, -1.2, 1, 0,  // 21 45
		0.2, 0.65,  1.2, 1, 1,  // 22 46
		-0.2, 0.65,  1.2, 0, 1, // 23 47
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22 ,  // bottom
		24,25,26,24,26,27,
		28,29,30,28,30,31,
		32,33,34,32,34,35,
		36,38,37,36,39,38,
		40,42,41,40,43,42,
		44,46,45,44,47,46

	};


	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredBaling()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO3); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;

	model = glm::translate(model, glm::vec3(translateX, 0, translateZ));

	model = glm::rotate(model, angleHeli, glm::vec3(0, 0, 1));

	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(1, 1, 1));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredBalingBelakang() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-1.2, 0.65, 0.2, 0, 0,  // 0
		1.2, 0.65, 0.2, 1, 0,   // 1
		1.2,  0.7, 0.2, 1, 1,   // 2
		-1.2,  0.7, 0.2, 0, 1,  // 3

		// right
		1.2,  0.7,  0.2, 0, 0,  // 4
		1.2,  0.7, -0.2, 1, 0,  // 5
		1.2, 0.65, -0.2, 1, 1,  // 6
		1.2, 0.65,  0.2, 0, 1,  // 7


		// back
		-1.2, 0.65, -0.2, 0, 0, // 8 
		1.2,  0.65, -0.2, 1, 0, // 9
		1.2,   0.7, -0.2, 1, 1, // 10
		-1.2,  0.7, -0.2, 0, 1, // 11

		// left
		-1.2, 0.65, -0.2, 0, 0, // 12
		-1.2, 0.65,  0.2, 1, 0, // 13
		-1.2,  0.7,  0.2, 1, 1, // 14
		-1.2,  0.7, -0.2, 0, 1, // 15

		// upper
		1.2, 0.7,  0.2, 0, 0,   // 16
		-1.2, 0.7,  0.2, 1, 0,  // 17
		-1.2, 0.7, -0.2, 1, 1,  // 18
		1.2, 0.7, -0.2, 0, 1,   // 19

		// bottom
		-1.2, 0.65, -0.2, 0, 0, // 20
		1.2, 0.65, -0.2, 1, 0,  // 21
		1.2, 0.65,  0.2, 1, 1,  // 22
		-1.2, 0.65,  0.2, 0, 1, // 23

		//==================2=========
		// front
		-0.2, 0.65, 1.2, 0, 0,  // 0 24
		0.2, 0.65, 1.2, 1, 0,   // 1 25
		0.2,  0.7, 1.2, 1, 1,   // 2 26
		-0.2,  0.7, 1.2, 0, 1,  // 3 27

		// right
		0.2,  0.7,  1.2, 0, 0,  // 4 28
		0.2,  0.7, -1.2, 1, 0,  // 5 29
		0.2, 0.65, -1.2, 1, 1,  // 6 30
		0.2, 0.65,  1.2, 0, 1,  // 7 31


		// back
		-0.2, 0.65, -1.2, 0, 0, // 8 32
		0.2,  0.65, -1.2, 1, 0, // 9 33
		0.2,   0.7, -1.2, 1, 1, // 10 34
		-0.2,  0.7, -1.2, 0, 1, // 11 35

		// left
		-0.2, 0.65, -1.2, 0, 0, // 12 36
		-0.2, 0.65,  1.2, 1, 0, // 13 37
		-0.2,  0.7,  1.2, 1, 1, // 14 38
		-0.2,  0.7, -1.2, 0, 1, // 15 39

		// upper
		0.2, 0.7,  1.2, 0, 0,   // 16 40
		-0.2, 0.7,  1.2, 1, 0,  // 17 41
		-0.2, 0.7, -1.2, 1, 1,  // 18 42
		0.2, 0.7, -1.2, 0, 1,   // 19 43

		// bottom
		-0.2, 0.65, -1.2, 0, 0, // 20 44
		0.2, 0.65, -1.2, 1, 0,  // 21 45
		0.2, 0.65,  1.2, 1, 1,  // 22 46
		-0.2, 0.65,  1.2, 0, 1, // 23 47

	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22 ,  // bottom
		24,25,26,24,26,27,
		28,29,30,28,30,31,
		32,33,34,32,34,35,
		36,38,37,36,39,38,
		40,42,41,40,43,42,
		44,46,45,44,47,46

	};


	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredBalingBelakang()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO4); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


	glm::mat4 model;


	model = glm::translate(model, glm::vec3(translateX + 2.5, 0.3, translateZ - 0.1));

	model = glm::translate(model, glm::vec3(rx, translateZ - 0.1));

	model = glm::rotate(model, 1.5f, glm::vec3(1, 0, 0));

	model = glm::rotate(model, rx, glm::vec3(0, 0, 1));

	model = glm::rotate(model, rx, glm::vec3(0, 1, 0));

	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(.6, .6, .6));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

int main(int argc, char** argv) {
	RenderEngine& app = Demo();
	app.Start("Transformation: Transform Cube", 1600, 900, false, false);
}