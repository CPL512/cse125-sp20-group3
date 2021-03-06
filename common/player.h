#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../client/The-Forge/Common_3/OS/Math/MathTypes.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include "entity.h"
#include "client2server.h"
#include "macros.h"
#include "BuildNode.h"

#define MOVE_SPEED 1
#define INTERACT_DISTANCE 5

enum BUILD_MODE { NEUTRAL, LASER, CLAW, SUPER_MINION };

class Player : public Entity {
private:
    float velocity_x, velocity_z, acceleration_x, acceleration_z;
    float rotation_y;
	BUILD_MODE buildMode;
	vec3 interactPos;

public:
    Player(GameObjectData data, int id, Team* t, SceneManager_Server* sm);
	
    void update(float deltaTime) override; //server-side state management
	void processInput(PlayerInput in);
	//void setVelocity(float vel_x, float vel_z);
	//std::pair<float, float> getVelocities();
	void setMoveAndDir(int move_x, int move_z, float view_y_rot);
	BUILD_MODE getBuildMode();

	void setEntData(EntityData data) override;
	int writeData(char buf[], int index) override;
};
#endif