#ifndef _PLAYER_H_
#define _PLAYER_H_

//#include "../client/src/Transform.h" //only include if client-side?
#include "../client/The-Forge/Common_3/OS/Math/MathTypes.h"
#include <iostream>
#include <chrono>
#include <ctime>

#define MOVE_SPEED 1

class Player {
private:
    std::chrono::steady_clock::time_point lastTime;
    mat4 model;
    float velocity_x, velocity_z, acceleration_x, acceleration_z;
public:
    struct PlayerData {
        float x, z, rot;
    };

    Player(mat4 model_mat, std::string objFilename); //client-side, create geometry to load model
    Player(mat4 model_mat); //server-side, only init state management logic
	
    void update(); //server-side state management
	void setVelocity(float vel_x, float vel_z);
	void setMove(int move_x, int move_z);
    void setPosRot(float pos_x, float pos_z, float rot_y); //client-side setting model to match server

    void setData(char buf[], int index);
    mat4 getMatrix();

    void buildTower();
    void spawnUnit();
};
#endif