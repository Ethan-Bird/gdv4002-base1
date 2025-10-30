#include "Engine.h"


// Function prototypes
void myUpdate(GLFWwindow* window, double tDelta);

void myUpdate(GLFWwindow* window, double tDelta)
{
	float player1RotationSpeed = glm::radians(90.0f);
}

int main(void) {




	// Initialise the engine (create window, setup OpenGL backend)
	int initResult = engineInit("GDV4002 - Applied Maths for Games", 1024, 1024);

	// If the engine initialisation failed report error and exit
	if (initResult != 0) {

		printf("Cannot setup game window!!!\n");
		return initResult; // exit if setup failed
	}

	//
	// Setup game scene objects here
	addObject("player1");
	addObject("player2");

	GameObject2D* player1Object = getObject("player1");
	GameObject2D* player2Object = getObject("player2");

	if (player1Object != nullptr)
	{
		player1Object->position = glm::vec2(-1.0f, 1.0f);
		player1Object->orientation = glm::radians(-30.0f);
		player1Object->size = glm::vec2(0.5f, 0.5f);
		player1Object->textureID = loadTexture("Resources\\Textures\\mcblock01.png", TextureProperties::NearestFilterTexture());
	}

	if (player2Object != nullptr)
	{
		player2Object->position = glm::vec2(1.5f, 1.0f);
		player2Object->orientation = glm::radians(45.0f);
		player2Object->size = glm::vec2(0.75f, 0.75f);
		player2Object->textureID = loadTexture("Resources\\Textures\\player1_ship.png", TextureProperties::NearestFilterTexture());
	}

	setUpdateFunction(myUpdate);

	float anglesPerSecond = glm::radians(45.0f);
	float playerVelocity = 2.0f;

	GameObject2D* player1 = getObject("player1");
	player1->orientation += player1RotatingSpeed * tDelta;

	// Enter main loop - this handles update and render calls
	engineMainLoop();
	
	while (!glfwWindowShouldClose(window))
	{
		gameClock->tick();
		double tDelta = gameClock->gameTimeDelta();

		if (overrideUpdateFn)
			overrideUpdateFn(window, tDelta);
		else
			defaultUpdateScene(tDelta);

		defaultRenderScene();

		glfwSwapBuffers(window);

		glfwPollEvents();

		char timingString[256];

		sprintf_s(timingString, 256, "%s: Average fps: %.0f; Average spf: %f", windowTitleString.c_str(), gameClock->averageFPS(), gameClock->averageSPF() / 1000.0f);
		glfwSetWindowTitle(window, timingString);
	}

	// When we quit (close window for example), clean up engine resources
	engineShutdown();

	// return success :)
	return 0;
}


