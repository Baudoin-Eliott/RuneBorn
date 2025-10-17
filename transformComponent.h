#pragma once
#include "components.h"
#include "vector2D.h"

class TransformComponent : public Component {



public:

	Vector2D position;
	SDL_FRect newPos;
	Vector2D velocity;

	int height = 16;
	int width = 16;
	int scale;
	std::string state = "idle";
	int side = 2;
	int frame = 0;


	int speed = 4;

	TransformComponent() { 

		position.Zero(); 
	}

	TransformComponent(int sc) {
		position.Zero();
		scale = sc;
	}

	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
		
	}

	TransformComponent(float x, float y, int h, int w, int sc) {
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}
	
	void init() override {
		velocity.Zero();
	}


	void update() override {
		
		newPos = { position.x + velocity.x * speed, position.y + velocity.y * speed ,(float) width * scale , (float) height * scale};
	}

	void NewPos() {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

};