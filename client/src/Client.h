#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT  0x501
#endif

#include "../../common/macros.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "../../common/entity.h"
#include "../../common/player.h"
#include "../../common/team.h"
#include <vector>
#include <map>

#include "UIUtils.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class Client {
private:
	SOCKET ConnectSocket = INVALID_SOCKET;
public:
	struct IDEntData {
		int id;
		Entity::EntityData ent_data;
	};

	struct StateUpdateData {
		Team::TeamData redTeamData;
		Team::TeamData blueTeamData;
	};

	struct SceneUpdateData {
		std::vector<IDEntData> entUpdates;
	};

	struct UpData {
		StateUpdateData stateUpdate;
		SceneUpdateData sceneUpdate;
	};

	Client(std::string servername); //establish connection with given server
	int sendData(char sendbuf[], int buflen, int flags); //send data to client's connected server
	//int recvData(char recvbuf[], int buflen, int flags); //recv data from client's connected server
	int recvPlayerID();
	UpData recvAndFormatData();
	int closeConnection(int how); //close client's connection with server
};