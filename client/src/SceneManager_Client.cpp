#include "SceneManager_Client.h"

namespace {
	const char* playerFile = "female-char.gltf";
	const char* groundFile = "Ground.gltf";
	const char* minionFile = "minion-retry.gltf";
	const char* towerFile = "tower-1-laser.gltf";
}

SceneManager_Client::SceneManager_Client(Renderer* renderer)
{
	// It'd be nice if I could put this in a loop later
	gltfGeodes[ENV_GEODE] = conf_new(GLTFGeode, renderer, groundFile);
	gltfGeodes[PLAYER_GEODE] = conf_new(GLTFGeode, renderer, playerFile);
	//gltfGeodes[BASE_GEODE] = conf_new(GLTFGeode, renderer, baseFile);
	gltfGeodes[MINION_GEODE] = conf_new(GLTFGeode, renderer, minionFile);
	gltfGeodes[TOWER_GEODE] = conf_new(GLTFGeode, renderer, towerFile);
	//gltfGeodes[RESOURCE_GEODE] = conf_new(GLTFGeode, renderer, resourceFile);

	trackedPlayer_ID = "";

	transforms["ground"] = conf_new(Transform, mat4::identity());
	transforms["ground"]->addChild(gltfGeodes[ENV_GEODE]);
	this->addChild(transforms["ground"]);

	/*
	Transform* t = conf_new(Transform, mat4::identity());
	Transform* t2 = conf_new(Transform, mat4::rotationY(-PI/2));
	t->addChild(t2);
	t2->addChild(gltfGeodes[0]);
	this->addChild(t);
	transforms.push_back(t);
	transforms.push_back(t2);

	t = conf_new(Transform, mat4::identity());
	t->addChild(gltfGeodes[1]);
	this->addChild(t);
	transforms.push_back(t);

	srand((unsigned int)time(NULL));
	for (int i = 0; i < 500; i++) {
		float x = -100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (200)));
		float z = -100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (200)));
		float rot = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / PI));
		float s = 1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));

		mat4 transform = mat4::translation(vec3(x, 0, z)) * mat4::rotationY(rot) * mat4::scale(vec3(s));
		t = conf_new(Transform, transform);
		t->addChild(gltfGeodes[3]);
		this->addChild(t);
		transforms.push_back(t);
	}
	for (int i = 0; i < 100; i++) {
		float x = -100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (200)));
		float z = -100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (200)));
		float rot = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / PI));
		float s = 0.75f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.5f));

		mat4 transform = mat4::translation(vec3(x, 0, z)) * mat4::rotationY(rot) * mat4::scale(vec3(s));
		t = conf_new(Transform, transform);
		t->addChild(gltfGeodes[2]);
		this->addChild(t);
		transforms.push_back(t);
	}
	*/
}

SceneManager_Client::~SceneManager_Client()
{
	for (std::pair<std::string, Entity*> e : idMap) conf_delete(e.second);
	for (std::pair<std::string, Transform*> t : transforms) conf_delete(t.second);
	for (std::pair<std::string, GLTFGeode*> g : gltfGeodes) conf_delete(g.second);

	for (Transform* a : player_adjustments) conf_delete(a);
}

void SceneManager_Client::createMaterialResources(RootSignature* pRootSignature, DescriptorSet* pBindlessTexturesSamplersSet, Sampler* defaultSampler)
{
	for (std::pair<std::string, GLTFGeode*> g : gltfGeodes) {
		g.second->createMaterialResources(pRootSignature, pBindlessTexturesSamplersSet, defaultSampler);
	}
}

void SceneManager_Client::updateFromClientBuf(std::vector<Client::UpdateData> updateBuf)
{
	std::cout << "updating from client buf of size " << updateBuf.size() << "\n";
	for (Client::UpdateData data : updateBuf) {
		std::cout << "update obj\n";
		if (idMap.find(data.id_str) == idMap.end()) { //new id encountered, spawn new object
			int id_int = stoi(data.id_str);
			//std::cout << "id_int: " << id_int << "\n";
			if (ID_PLAYER_MIN <= id_int && id_int <= ID_PLAYER_MAX) {
				std::cout << "creating new player, id: " << data.id_str << "\n";

				idMap[data.id_str] = conf_new(Player);
				transforms[data.id_str] = conf_new(Transform, mat4::identity());
				Transform* adjustment = conf_new(Transform, mat4::rotationY(-PI / 2));

				adjustment->addChild(gltfGeodes[PLAYER_GEODE]);
				transforms[data.id_str]->addChild(adjustment);

				player_adjustments.push_back(adjustment); //save to be deleted upon closing
			}
			else if (ID_BASE_MIN <= id_int && id_int <= ID_BASE_MAX) {
				//idMap[id_str] = new Base();
			}
			else if (ID_MINION_MIN <= id_int && id_int <= ID_MINION_MAX) {
				std::cout << "creating new minion, id: " << data.id_str << "\n";
				idMap[data.id_str] = conf_new(Minion, MINION_HEALTH, MINION_ATTACK);
				transforms[data.id_str] = conf_new(Transform, mat4::identity());
				transforms[data.id_str]->addChild(gltfGeodes[MINION_GEODE]);
			}
			else if (ID_TOWER_MIN <= id_int && id_int <= ID_TOWER_MAX) {
				std::cout << "creating new tower, id: " << data.id_str << "\n";
				idMap[data.id_str] = conf_new(Tower, TOWER_HEALTH, TOWER_ATTACK);
				transforms[data.id_str] = conf_new(Transform, mat4::identity());
				transforms[data.id_str]->addChild(gltfGeodes[TOWER_GEODE]);
			}
			else if (ID_RESOURCE_MIN <= id_int && id_int <= ID_RESOURCE_MAX) {
				//idMap[id_str] = new Resource();
			}
			this->addChild(transforms[data.id_str]);
		}
		
		idMap[data.id_str]->setEntData(data.ent_data);
		transforms[data.id_str]->setMatrix(idMap[data.id_str]->getMatrix());
	}
}

void SceneManager_Client::updateFromInputBuf(float deltaTime)
{
	char recvbuf[DEFAULT_BUFLEN];
	Input::EncodeToBuf(recvbuf);
	PlayerInput input = ((PlayerInput*)recvbuf)[0];
	//printf("%d %d %f\n", input.move_x, input.move_z, input.view_y_rot);
	//player.setMoveAndDir(input);
	//player.update();
	//transforms[0]->setMatrix(player.getMatrix());
}

void SceneManager_Client::setProgram(Geode::GeodeShaderDesc program)
{
	for (std::pair<std::string, GLTFGeode*> g : gltfGeodes) {
		g.second->setProgram(program);
	}
}

void SceneManager_Client::trackPlayer(std::string player_id) {
	std::cout << "tracking player " + player_id + "\n";
	trackedPlayer_ID = player_id;
}

mat4 SceneManager_Client::getPlayerTransformMat() {
	
	if (trackedPlayer_ID != "" && transforms.find(trackedPlayer_ID) != transforms.end()) {
		return transforms[trackedPlayer_ID]->getMatrix();
	}
	else return mat4::identity();
}

/***** legacy code *****/
/*if (trackedPlayer_ID == "" && bufsize == 1) {	//first message from server should be 1 byte message
	trackPlayer(std::string(1, buf[0]));		//of the player's connection id
}
else {
	std::string id_str = "";
	GameObject::GameObjectData data;
	int health;
	int state = 0; // 0 for reading id, 1 for reading gameobjectdata, 2 for reading health, 3 for checking closing delimiter
	for (int i = 0; i < DEFAULT_BUFLEN; i++) {
		//std::cout << "i: " << i << "\n";
		if (state == 0) {
			if (buf[i] == DELIMITER) {
				state++; //end of state found, advance to next state
				//std::cout << "id_str: " + id_str + "\n";
			}
			else { //read id bytes one by one, appending to id_str
				id_str += buf[i];
			}

		}
		else if (state == 1) {
			int move_x = ((int*)(buf + i))[0];
			int move_z = ((int*)(buf + i + 4))[0];
			float rot_y = ((float*)(buf + i + 8))[0];
			//std::cout << "move_x: " << move_x << " move_z: " << move_z << " rot_y: " << rot_y << "\n";
			data = ((GameObject::GameObjectData*)(buf + i))[0];
			i += (sizeof GameObject::GameObjectData); //advance i to where delimiter should be

			if (buf[i] == DELIMITER) {
				//std::cout << "state 1 delimiter at i: " << i << "\n";
				state++; //end of state found, advance to next state
			}
			else {
				//std::cout << "state 1 delimiter expected but not found, i: " << i << "\n";
			}
		}
		else if (state == 2) {
			health = ((int*)(buf + i))[0];
			i += sizeof(int); //advance i to where delimiter should be

			if (buf[i] == DELIMITER) { //end of data line found, write to map, advance to possible final state
				//std::cout << "id: " << id_str << " x: " << data.x << " z: " << data.z << " y: " << data.rot << " health: " << health << "\n";
				//std::cout << "state 2 delimiter at i: " << i << "\n";


			}
			else {
				std::cout << "state 2 delimiter not found when expected, i: " << i << "\n";
			}
		}
		else if (state == 3) {
			if (buf[i] == DELIMITER) {
				std::cout << "closing delimiter found at i: " << i << "\n";
				break;
			}
			else {
				std::cout << "non-closing byte in state 3 at i: " << i << ", rechecking\n";
				state = 0; //reset state and read this byte again
				i--;
			}
		}
		else {
			std::cout << "SceneManager_Client updateFromClientBuf state out of sync";
		}
	}
}*/