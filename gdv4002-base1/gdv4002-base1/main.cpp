#include "Engine.h"
#include "Keys.h"
#include <bitset>


// Function prototypes
void myUpdate(GLFWwindow* window, double tDelta);

void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);

std::bitset<5> keys{ 0x0 };

//commented gravity, dont wish to use in current state
//glm::vec2 gravity = glm::vec2(0.0f, -1.0f);

// Astroid physics
glm::vec2 astroidVelocity[3] =
{
	glm::vec2(0.6f, 0.2f), glm::vec2(-0.4f, 0.5f), glm::vec2(0.3f, -0.6f)
};

float astroidRotVelocity[3] =
{
	glm::radians(45.0f), glm::radians(-90.0f), glm::radians(60.0f)
};

//bullets
const int MAX_BULLETS = 32;

glm::vec2 bulletVelocity[MAX_BULLETS];
bool bulletAlive[MAX_BULLETS] = { false };

float bulletSpeed = 3.0f;
float bulletCooldown = 0.2f;   // seconds between shots
float bulletTimer = 0.0f;      // counts down to 0

void killBullet(int b)
{
	bulletAlive[b] = false;
	bulletVelocity[b] = glm::vec2(0.0f, 0.0f); // reset
	std::string bulletName = "bullet" + std::to_string(b);
	GameObject2D* bullet = getObject(bulletName.c_str());
	if (bullet)
		bullet->position = glm::vec2(9999.0f, 9999.0f); // park off-screen
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
	// Setup rendering properties (enable blending)
	//
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_ALWAYS);


	//
	// Setup game scene objects here

	addObject("background", glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(5.0f, 5.0f), "Resources\\Textures\\background_stars.png", TextureProperties::NearestFilterTexture()
	);


	for (int b = 0; b < MAX_BULLETS; b++) // pre-create bullet objects
	{
		std::string bulletName = "bullet" + std::to_string(b);

		addObject(bulletName.c_str(), glm::vec2(9999.0f, 9999.0f), 0.0f, glm::vec2(0.15f, 0.15f), "Resources\\Textures\\laser.png", TextureProperties::NearestFilterTexture());
	}

	addObject("player", glm::vec2(-1.5f, 0.0f), 0.0f, glm::vec2(0.5f, 0.5f), "Resources\\Textures\\ship_1.png", TextureProperties::NearestFilterTexture());

	addObject("astroid", glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(0.75f, 0.75f), "Resources\\Textures\\asteroid.png", TextureProperties::NearestFilterTexture());

	addObject("astroid", glm::vec2(1.0f, 0.0f), 0.0f, glm::vec2(0.75f, 0.75f), "Resources\\Textures\\asteroid.png", TextureProperties::NearestFilterTexture());

	addObject("astroid", glm::vec2(2.0f, 0.0f), 0.0f, glm::vec2(0.75f, 0.75f), "Resources\\Textures\\asteroid.png", TextureProperties::NearestFilterTexture());

	





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
	GameObjectCollection astroids = getObjectCollection("astroid"); // get all astroid objects

	float wrapX = 2.5f;
	float wrapY = 2.5f;

	for (int i = 0; i < astroids.objectCount; i++)
	{
		astroids.objectArray[i]->position += astroidVelocity[i] * (float)tDelta; // Move asteroid using its velocity

		astroids.objectArray[i]->orientation += astroidRotVelocity[i] * (float)tDelta; // Rotate asteroid

		if (astroids.objectArray[i]->position.x > wrapX) // Screen wrap (X)
			astroids.objectArray[i]->position.x = -wrapX;
		else if (astroids.objectArray[i]->position.x < -wrapX)
			astroids.objectArray[i]->position.x = wrapX;

		if (astroids.objectArray[i]->position.y > wrapY) // Screen wrap (Y)
			astroids.objectArray[i]->position.y = -wrapY;
		else if (astroids.objectArray[i]->position.y < -wrapY)
			astroids.objectArray[i]->position.y = wrapY;
	}

	

	static float playerSpeed = 1.0f; // distance per second

	GameObject2D* player = getObject("player"); 

	static glm::vec2 playerVelocity(0.0f, 0.0f); // initial velocity
	static float acceleration = 2.0f; // units per second squared
	static float maxSpeed = 1.5f; // max speed
	float rotationSpeed = glm::radians(180.0f); // degrees per second
	float dragPerSecond = 1.0f;  // adds drag


	//if gravity is to be used, remove comment from below line
	//playerVelocity += gravity * (float)tDelta;


	if (keys.test(Key::W))
	{
		glm::vec2 forward(cos(player->orientation), sin(player->orientation)); // calculate forward vector
		playerVelocity += forward * acceleration * (float)tDelta; // accelerate forward

	}


	if (keys.test(Key::S))
	{
		glm::vec2 backward(cos(player->orientation), sin(player->orientation)); // calculate backward vector
		playerVelocity -= backward * acceleration * (float)tDelta; // accelerate backward
	}

	float dragFactor = 1.0f / (1.0f + dragPerSecond * (float)tDelta); //drag to player
	playerVelocity *= dragFactor;


	if (glm::length(playerVelocity) > maxSpeed)
	{
		playerVelocity = glm::normalize(playerVelocity) * maxSpeed; // clamp to max speed
	}

	player->position += playerVelocity * (float)tDelta; // update position based on velocity

	if (keys.test(Key::A)) 
		player->orientation += rotationSpeed * (float)tDelta; // rotate left

	if (keys.test(Key::D))
		player->orientation -= rotationSpeed * (float)tDelta; // rotate right

	bulletTimer -= (float)tDelta;

	if (keys.test(Key::SPACE) && bulletTimer <= 0.0f)
	{
		glm::vec2 forward(cos(player->orientation), sin(player->orientation));

		for (int b = 0; b < MAX_BULLETS; b++)
		{
			if (!bulletAlive[b])
			{
				bulletAlive[b] = true;
				bulletVelocity[b] = forward * bulletSpeed;

				std::string bulletName = "bullet" + std::to_string(b);
				GameObject2D* bullet = getObject(bulletName.c_str());

				if (bullet != nullptr)
				{
					bullet->position = player->position; // spawn at player
					bullet->orientation = player->orientation; // match player orientation
				}

				bulletTimer = bulletCooldown;
				break;
			}
		}
	}

	for (int b = 0; b < MAX_BULLETS; b++)
	{
		if (!bulletAlive[b]) continue;

		std::string bulletName = "bullet" + std::to_string(b);
		GameObject2D* bullet = getObject(bulletName.c_str());

		if (bullet == nullptr)
		{
			bulletAlive[b] = false;
			bulletVelocity[b] = glm::vec2(0.0f, 0.0f);
			continue;
		}


		// Move bullet
		bullet->position += bulletVelocity[b] * (float)tDelta;

		float bulletKillMargin = 0.3f; // extra distance before killing bullet

		if (bullet->position.x > (wrapX + bulletKillMargin) || bullet->position.x < (-wrapX - bulletKillMargin) ||
			bullet->position.y > (wrapY + bulletKillMargin) || bullet->position.y < (-wrapY - bulletKillMargin))
		{
			killBullet(b);
			continue;
		}


	}


	if (player->position.x > wrapX)
		player->position.x = -wrapX;
	else if (player->position.x < -wrapX)
		player->position.x = wrapX;

	if (player->position.y > wrapY)
		player->position.y = -wrapY;
	else if (player->position.y < -wrapY)
		player->position.y = wrapY;

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




