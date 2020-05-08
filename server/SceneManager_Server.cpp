#include "SceneManager_Server.h"

SceneManager_Server::SceneManager_Server() {
	next_player_id = ID_PLAYER_MIN;
	next_base_id = ID_BASE_MIN;
	next_minion_id = ID_MINION_MIN;
	next_tower_id = ID_TOWER_MIN;
	next_resource_id = ID_RESOURCE_MIN;

	//team1 = new Team();
	//team2 = new Team();

	this->populateScene();
}

void SceneManager_Server::processInput(std::string player, PlayerInput in) {
	((Player*)idMap[player])->setMoveAndDir(in);
	//std::cout << "processing input for player " << player << ": " << in.move_x << " " << in.move_z << " " << in.view_y_rot << "\n";
}

bool SceneManager_Server::addPlayer(std::string player_id) {
	if (idMap.find(player_id) == idMap.end()) { //player_id not in map, create a new player
		idMap[player_id] = new Player();
		std::cout << "created new player id: " << player_id << " at " << idMap[player_id] << "\n";

		return true; //return true that a player was added
	}
	else {
		return false; //return false, player was not added
	}
}

void SceneManager_Server::spawnEntity(char spawnType, float pos_x, float pos_z, float rot_y) {
	/*Entity* ent;
	int id_int;

	switch (spawnType) { //WARN: possible id wrap issues
	case BASE_TYPE:
		ent = new Base();
		id_int = next_base_id;
		next_base_id++;
		if (next_base_id == ID_BASE_MAX) next_base_id = ID_BASE_MIN;
		break;
	case MINION_TYPE:
		ent = new Minion();
		id_int = next_minion_id;
		next_minion_id++;
		if (next_minion_id == ID_MINION_MAX) next_minion_id = ID_MINION_MIN;
		break;
	case TOWER_TYPE:
		ent = new Tower();
		id_int = next_tower_id;
		next_tower_id++;
		if (next_tower_id == ID_TOWER_MAX) next_tower_id = ID_TOWER_MIN;
		break;
	case RESOURCE_TYPE:
		ent = new Resource();
		id_int = next_resource_id;
		next_resource_id++;
		if (next_resource_id == ID_RESOURCE_MAX) next_resource_id = ID_RESOURCE_MIN;
		break;
	default:
		ent = new Minion();
		id_int = -1;
		std::cout << "spawnEntity encountered unknown entity type\n";
	}

	GameObject::GameObjectData data = { pos_x, pos_z, rot_y };
	ent->setData(data);
	idMap[std::to_string(id_int)] = ent;*/
}

void SceneManager_Server::resetClocks() {
	for (std::pair<std::string, Entity*> idEntPair : idMap) {
		idEntPair.second->resetClock();
	}
}

void SceneManager_Server::update() {
	for (std::pair<std::string, Entity*> idEntPair: idMap) {
		idEntPair.second->update();
	}
}

int SceneManager_Server::encodeScene(char buf[]) {
	/* buf structure
	 * ID,{GameObjectData},health,
	 * ID,{GameObjectData},health,
	 * ...
	 * ID,{GameObjectData},health,,
	 */

	int i = 0;
	for (std::pair<std::string, Entity*> idEntPair : idMap) { //iterate through all entities in scene
		//std::cout << "id: " << idEntPair.first << "\n";

		//std::cout << "writing id at i: " << i << "\n";
		strncpy(buf + i, idEntPair.first.c_str(), idEntPair.first.length()); //write id bytes without null term
		i += idEntPair.first.length();

		//std::cout << "delimiter 1 at i: " << i << "\n";
		buf[i] = DELIMITER; //write delimiter
		i++;

		//std::cout << "writing data at i: " << i << "\n";
		int bytes = idEntPair.second->writeData(buf, i); //write GameObjectData at i, increase i by number of bytes written
		i += bytes;

		//std::cout << "delimiter 2 at i: " << i << "\n";
		buf[i] = DELIMITER; //write delimiter
		i++;

		//std::cout << "writing health at i: " << i << "\n";
		((int*)(buf + i))[0] = idEntPair.second->getHealth(); //write entity's health
		//std::cout << " health: " << idEntPair.second->getHealth() << "\n";
		i += sizeof(int);

		//std::cout << "delimiter 3 at i: " << i << "\n";
		buf[i] = DELIMITER; //write delimiter
		i++;
	}
	
	//std::cout << "closing delimiter at i: " << i << "\n";
	buf[i] = DELIMITER; //append an extra delimiter to indicate the end of the data
	i++;
	return i;
}

void SceneManager_Server::populateScene() { //testing only
	//NOTE: number of objects should not exceed sendbuf capacity
	//sendbufsize >= (NUM_PLAYERS * 20) + (# of minions and towers * 23) + 1
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 12; i++) {
		float x = -100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (200)));
		float z = -100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (200)));
		float rot = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / PI));
		float s = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));

		mat4 transform = mat4::translation(vec3(x, 0, z)) * mat4::rotationY(rot) * mat4::scale(vec3(s));
		Minion* m = new Minion(MINION_HEALTH, MINION_ATTACK);
		m->setMatrix(transform);
		idMap[std::to_string(next_minion_id)] = m;

		std::cout << "created new minion: " << std::to_string(next_minion_id) << " at " << idMap[std::to_string(next_minion_id)] << "\n";

		next_minion_id++;
		if (next_minion_id == ID_MINION_MAX) next_minion_id = ID_MINION_MIN;
	}
	for (int i = 0; i < 5; i++) {
		float x = -100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (200)));
		float z = -100 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (200)));
		float rot = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / PI));
		float s = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0f));

		mat4 transform = mat4::translation(vec3(x, 0, z)) * mat4::rotationY(rot) * mat4::scale(vec3(s));
		Tower* t = new Tower(TOWER_HEALTH, TOWER_ATTACK);
		t->setMatrix(transform);
		idMap[std::to_string(next_tower_id)] = t;
		next_tower_id++;
		if (next_tower_id == ID_TOWER_MAX) next_tower_id = ID_TOWER_MIN;
	}
}