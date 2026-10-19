//Práctica#9
//Guerra Marcelino Brenda Paola
//Fecha de entrega: 18 de octubre de 2026

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <string>

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(6.2f, 1.05f, -3.0f),  // Farola exterior 1
	glm::vec3(6.2f, 1.05f, 3.5f),   // Farola exterior 2 
	glm::vec3(-0.35f, 1.7f, 0.65f), // Lampara de la entrada (pendiente)
	glm::vec3(-1.5f, -0.7f, 5.5f)     // Luz decorativa de la fuente
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

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



// Encendido y apagado de las farolas
bool farolasEncendidas = true;
bool luzEntradaEncendida = true;
// Control de la luz decorativa de la fuente
bool luzFuenteEncendida = true;

// Control del reflector del garaje
bool reflectorEncendido = true;

// Posicion y direccion del reflector
glm::vec3 posicionReflector(-1.4f, 0.8f, -2.1f);
glm::vec3 direccionReflector(1.0f, -5.0f, -0.2f); 

// Posiciones de los faros delanteros del automovil
glm::vec3 posicionFaroIzquierdo(3.33f, -0.69f, -2.25f);
glm::vec3 posicionFaroDerecho(4.22f, -0.69f, -2.18f);

// Direccion de los faros hacia la calle
glm::vec3 direccionFaros(0.0f, -0.05f, 1.0f);

// Control de los faros del automovil
bool farosAutoEncendidos = true;

// Posiciones de los modelos del Sol y la Luna
glm::vec3 posicionSol(2.0f, 5.0f, -3.0f);
glm::vec3 posicionLuna(0.0f, 5.0f, -2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Práctica#9 - Brenda Guerra", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	// Carga de modelos
	Model Dog((char*)"Models/RedDog.obj");
	Model cat((char*)"Models/12221_Cat_v1_l3.obj");
	Model house((char*)"Models/model.obj");
	Model mailbox((char*)"Models/mailbox.obj");
	Model fountain((char*)"Models/fountain.obj");
	Model moon((char*)"Models/moon (1).obj");
	Model farola((char*)"Models/street_lamp.obj");
	Model farolaCasa((char*)"Models/security_light.obj");



	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.025f, 0.045f, 0.12f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 0.3f, -1.0f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.12f, 0.14f, 0.22f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.22f, 0.28f, 0.42f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.25f, 0.30f, 0.45f);


		// Point light 1
		// Luz puntual 1 - Farola exterior 1
		glUniform3fv(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
			1, glm::value_ptr(pointLightPositions[0]));

		float intensidadFarola = farolasEncendidas ? 1.0f : 0.0f;

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
			0.08f * intensidadFarola,
			0.06f * intensidadFarola,
			0.03f * intensidadFarola);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
			1.0f * intensidadFarola,
			0.75f * intensidadFarola,
			0.35f * intensidadFarola);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),
			1.0f * intensidadFarola,
			0.8f * intensidadFarola,
			0.5f * intensidadFarola);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.44f);



		// Point light 2
		// Luz puntual 2 - Farola exterior 2
		glUniform3fv(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"),
			1, glm::value_ptr(pointLightPositions[1]));

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"),
			0.08f * intensidadFarola,
			0.06f * intensidadFarola,
			0.03f * intensidadFarola);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"),
			1.0f * intensidadFarola,
			0.75f * intensidadFarola,
			0.35f * intensidadFarola);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"),
			1.0f * intensidadFarola,
			0.8f * intensidadFarola,
			0.5f * intensidadFarola);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.44f);

		// Luz puntual 3 - Lampara de la entrada

		float intensidadEntrada = luzEntradaEncendida ? 1.0f : 0.0f;

		// Posicion
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"),
			pointLightPositions[2].x,
			pointLightPositions[2].y,
			pointLightPositions[2].z);

		// Iluminacion ambiental
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"),
			0.04f * intensidadEntrada,
			0.03f * intensidadEntrada,
			0.015f * intensidadEntrada);

		// Iluminacion difusa
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"),
			0.75f * intensidadEntrada,
			0.55f * intensidadEntrada,
			0.25f * intensidadEntrada);

		// Iluminacion especular
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"),
			0.85f * intensidadEntrada,
			0.65f * intensidadEntrada,
			0.35f * intensidadEntrada);

		// Atenuacion
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.44f);

		// Luz puntual 4 - Fuente decorativa
		float intensidadFuente = luzFuenteEncendida ? 1.0f : 0.0f;
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"),
			pointLightPositions[3].x,
			pointLightPositions[3].y,
			pointLightPositions[3].z);

		// Iluminacion ambiental
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"),
			0.06f * intensidadFuente,
			0.015f * intensidadFuente,
			0.12f * intensidadFuente);

		// Iluminacion difusa
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"),
			0.85f * intensidadFuente,
			0.15f * intensidadFuente,
			1.0f * intensidadFuente);

		// Iluminacion especular
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"),
			0.90f * intensidadFuente,
			0.30f * intensidadFuente,
			1.0f * intensidadFuente);

		// Atenuación
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.30f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.40f);

		// ======================================================
// REFLECTORES
// ======================================================

// Reflector 1 - Seguridad del garaje

		float intensidadReflector = reflectorEncendido ? 1.0f : 0.0f;

		// Posicion
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].position"),
			posicionReflector.x,
			posicionReflector.y,
			posicionReflector.z);

		// Direccion
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].direction"),
			direccionReflector.x,
			direccionReflector.y,
			direccionReflector.z);

		// Iluminacion ambiental
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].ambient"),
			0.02f * intensidadReflector,
			0.02f * intensidadReflector,
			0.02f * intensidadReflector);

		// Iluminacion difusa
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].diffuse"),
			1.0f * intensidadReflector,
			0.95f * intensidadReflector,
			0.80f * intensidadReflector);

		// Iluminacion especular
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].specular"),
			1.0f * intensidadReflector,
			0.95f * intensidadReflector,
			0.85f * intensidadReflector);

		// Atenuacion
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].linear"), 0.22f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].quadratic"), 0.20f);

		// Apertura del reflector
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].cutOff"),
			glm::cos(glm::radians(25.0f)));

		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].outerCutOff"),
			glm::cos(glm::radians(40.0f)));


		// ======================================================
		// Reflectores 2 y 3 - Faros delanteros del automovil
		// ======================================================

		float intensidadFaros = farosAutoEncendidos ? 1.0f : 0.0f;

		glm::vec3 posicionesFaros[] = {
			posicionFaroIzquierdo,
			posicionFaroDerecho
		};

		for (int i = 0; i < 2; i++)
		{
			std::string nombre = "spotLights[" + std::to_string(i + 1) + "].";

			// Posicion
			glUniform3fv(glGetUniformLocation(lightingShader.Program,
				(nombre + "position").c_str()),
				1, glm::value_ptr(posicionesFaros[i]));

			// Direccion hacia la izquierda
			glUniform3fv(glGetUniformLocation(lightingShader.Program,
				(nombre + "direction").c_str()),
				1, glm::value_ptr(direccionFaros));

			// Iluminacion ambiental
			glUniform3f(glGetUniformLocation(lightingShader.Program,
				(nombre + "ambient").c_str()),
				0.01f * intensidadFaros,
				0.01f * intensidadFaros,
				0.015f * intensidadFaros);

			// Iluminacion difusa - Faros blancos intensos
			glUniform3f(glGetUniformLocation(lightingShader.Program,
				(nombre + "diffuse").c_str()),
				1.8f * intensidadFaros,
				1.75f * intensidadFaros,
				1.55f * intensidadFaros);

			// Iluminacion especular
			glUniform3f(glGetUniformLocation(lightingShader.Program,
				(nombre + "specular").c_str()),
				1.5f * intensidadFaros,
				1.45f * intensidadFaros,
				1.30f * intensidadFaros);

			// Atenuacion
			glUniform1f(glGetUniformLocation(lightingShader.Program,
				(nombre + "constant").c_str()), 1.0f);

			glUniform1f(glGetUniformLocation(lightingShader.Program,
				(nombre + "linear").c_str()), 0.09f);

			glUniform1f(glGetUniformLocation(lightingShader.Program,
				(nombre + "quadratic").c_str()), 0.032f);

			// Apertura del cono
			glUniform1f(glGetUniformLocation(lightingShader.Program,
				(nombre + "cutOff").c_str()),
				glm::cos(glm::radians(12.0f)));

			glUniform1f(glGetUniformLocation(lightingShader.Program,
				(nombre + "outerCutOff").c_str()),
				glm::cos(glm::radians(20.0f)));
		}

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		// Carga de modelos del escenario
		view = camera.GetViewMatrix();

		// Perro 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.5f, -0.73f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, glm::radians(150.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		Dog.Draw(lightingShader);


		// Gato
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.8f, 2.15f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		model = glm::rotate(model, glm::radians(230.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cat.Draw(lightingShader);


		// Casa
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f));
		model = glm::translate(model, glm::vec3(0.0f, -6.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		house.Draw(lightingShader);


		// Buzon
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		model = glm::translate(model, glm::vec3(1.5f, -0.4f, 2.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mailbox.Draw(lightingShader);


		// Fuente
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::translate(model, glm::vec3(-2.8f, -1.6f, 7.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		fountain.Draw(lightingShader);


		// Luna
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.0f, 8.0f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		moon.Draw(lightingShader);


		// Farola 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.0f, -1.28f, -3.0f));
		model = glm::scale(model, glm::vec3(16.0f, 16.0f, 16.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		farola.Draw(lightingShader);


		// Farola 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.0f, -1.28f, 3.5f));
		model = glm::scale(model, glm::vec3(16.0f, 16.0f, 16.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		farola.Draw(lightingShader);


		// Farola Casa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.4f, 0.8f, -2.1f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		farolaCasa.Draw(lightingShader);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 3; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.12f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			/*glDrawArrays(GL_TRIANGLES, 0, 36);*/
		}
		glBindVertexArray(0);


		// Faros delanteros del automovil
		if (farosAutoEncendidos)
		{
			// Faro izquierdo
			model = glm::mat4(1.0f);
			model = glm::translate(model, posicionFaroIzquierdo);
			model = glm::scale(model, glm::vec3(0.10f));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);


			// Faro derecho
			model = glm::mat4(1.0f);
			model = glm::translate(model, posicionFaroDerecho);
			model = glm::scale(model, glm::vec3(0.10f));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindVertexArray(0);
		}


		// Cubo de referencia del reflector
		model = glm::mat4(1.0f);
		model = glm::translate(model, posicionReflector);
		model = glm::scale(model, glm::vec3(0.08f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.001f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.001f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.001f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.001f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.001f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.001f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	// Encender y apagar las farolas exteriores
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		farolasEncendidas = !farolasEncendidas;
	}

	// Encender y apagar la luz de la entrada
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		luzEntradaEncendida = !luzEntradaEncendida;
	}

	// Encender y apagar el reflector del garaje
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		reflectorEncendido = !reflectorEncendido;
	}

	// Encender y apagar la luz decorativa de la fuente
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		luzFuenteEncendida = !luzFuenteEncendida;
	}

	// Encender y apagar los faros delanteros del automovil
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		farosAutoEncendidos = !farosAutoEncendidos;
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}