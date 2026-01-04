#include "Engine.h"
#include "Keys.h"
#include <bitset>


// Function prototypes
void myUpdate(GLFWwindow* window, double tDelta);

float enemyPhase[3] = { 0.0f, 0.0f, 0.0f };
float enemyPhaseVelocity[3] = { glm::radians(90.0f), glm::radians(90.0f), glm::radians(90.0f) };

void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);

std::bitset<5> keys{ 0x0 };

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
	

	addObject("player", glm::vec2(-1.5f, 0.0f), 0.0f, glm::vec2(0.5f, 0.5f), "Resources\\Textures\\player1_ship.png");

	addObject("enemy", glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(0.75f, 0.75f), "Resources\\Textures\\alien01.png");

	addObject("enemy", glm::vec2(1.0f, 0.0f), 0.0f, glm::vec2(0.75f, 0.75f), "Resources\\Textures\\alien01.png");

	addObject("enemy", glm::vec2(2.0f, 0.0f), 0.0f, glm::vec2(0.75f, 0.75f), "Resources\\Textures\\alien01.png");




	setUpdateFunction(myUpdate);
	listGameObjectKeys();
	listObjectCounts();
	setKeyboardHandler(myKeyboardHandler);


	// Enter main loop - this handles update and render calls
	engineMainLoop();


	// When we quit (close window for example), clean up engine resources
	engineShutdown();

	// return success :)
	return 0;
}

void myUpdate(GLFWwindow* window, double tDelta)
{
	GameObjectCollection enemies = getObjectCollection("enemy");

	for (int i = 0; i < enemies.objectCount; i++) {

		enemies.objectArray[i]->position.y = sinf(enemyPhase[i]); // assume phase stored in radians so no conversion needed

		enemyPhase[i] += enemyPhaseVelocity[i] * tDelta;
	}

	static float playerSpeed = 1.0f; // distance per second

	GameObject2D* player = getObject("player");

	if (keys.test(Key::W) == true) {

		player->position.y += playerSpeed * (float)tDelta;
	}

	if (keys.test(Key::S) == true) {

		player->position.y -= playerSpeed * (float)tDelta;
	}

	if (keys.test(Key::A) == true) {

		player->position.x -= playerSpeed * (float)tDelta;
	}

	if (keys.test(Key::D) == true) {

		player->position.x += playerSpeed * (float)tDelta;
	}



}

void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check if the key was just pressed
	if (action == GLFW_PRESS) {

		// now check which key was pressed...
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// If escape is pressed tell GLFW we want to close the window(and quit)
				glfwSetWindowShouldClose(window, true);
			break;

		case GLFW_KEY_W:
			keys[Key::W] = true;
			break;

		case GLFW_KEY_S:
			keys[Key::S] = true;
			break;

		case GLFW_KEY_A:
			keys[Key::A] = true;
			break;

		case GLFW_KEY_D:
			keys[Key::D] = true;
			break;

		case GLFW_KEY_SPACE:
			keys[Key::SPACE] = true;
			break;




		}
	}
	// If not pressed, check the key has just been released
	else if (action == GLFW_RELEASE) {

		// handle key release events

// handle key release events
		switch (key)
		{
		case GLFW_KEY_W:
			keys[Key::W] = false;
			break;

		case GLFW_KEY_S:
			keys[Key::S] = false;
			break;

		case GLFW_KEY_A:
			keys[Key::A] = false;
			break;

		case GLFW_KEY_D:
			keys[Key::D] = false;
			break;

		case GLFW_KEY_SPACE:
			keys[Key::SPACE] = false;
			break;

		}

	}


}




