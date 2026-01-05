#pragma once //future updates

#include "GameObject2D.h"

class Player : public GameObject2D {

private:

    glm::vec2 velocity;
    float acceleration;
    float maxSpeed;
    float rotationSpeed;


public:

	Player(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID, float initialPlayerSpeed);
	void update(double tDelta) override;



};
