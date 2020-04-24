#include "SceneManager.h"

namespace {
	const char* playerFile = "WeirdBox.gltf";
	const char* groundFile = "Ground.gltf";
	const char* otherFile = "Kyubey.gltf";

	float rot = 0.f;
}

SceneManager::SceneManager(Renderer* renderer)
{
	// It'd be nice if I could put this in a loop later
	gltfGeodes.push_back(conf_new(GLTFGeode, renderer, playerFile));
	gltfGeodes.push_back(conf_new(GLTFGeode, renderer, groundFile));
	gltfGeodes.push_back(conf_new(GLTFGeode, renderer, otherFile));

	Transform* t = conf_new(Transform, mat4::identity());
	t->addChild(gltfGeodes[0]);
	this->addChild(t);
	transforms.push_back(t);

	t = conf_new(Transform, mat4::identity());
	t->addChild(gltfGeodes[1]);
	this->addChild(t);
	transforms.push_back(t);

	srand((unsigned int)time(NULL));
	for (int i = 0; i < 50; i++) {
		float x = -50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100)));
		float z = -50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (100)));
		float rot = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / PI));
		float s = 0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.5f));

		mat4 transform = mat4::translation(vec3(x, 0, z)) * mat4::rotationY(rot) * mat4::scale(vec3(s));
		t = conf_new(Transform, transform);
		t->addChild(gltfGeodes[2]);
		this->addChild(t);
		transforms.push_back(t);
	}
}

SceneManager::~SceneManager()
{
	for (auto t : transforms) conf_delete(t);
	for (auto g : gltfGeodes) conf_delete(g);
}

void SceneManager::createMaterialResources(RootSignature* pRootSignature, DescriptorSet* pBindlessTexturesSamplersSet, Sampler* defaultSampler)
{
	for (auto g : gltfGeodes) {
		g->createMaterialResources(pRootSignature, pBindlessTexturesSamplersSet, defaultSampler);
	}
}

void SceneManager::updateFromClientBuf(char buf[])
{
	GameObject::GameObjectData data = ((GameObject::GameObjectData*)buf)[0];
	player.setPosRot(data.x, data.z, data.rot);
	transforms[0]->setMatrix(player.getMatrix());
}

void SceneManager::updateFromInputBuf(float deltaTime)
{
	rot += 0.1f;
	char recvbuf[DEFAULT_BUFLEN];
	Input::EncodeToBuf(recvbuf);
	PlayerInput input = ((PlayerInput*)recvbuf)[0];
	input.view_y_rot = rot;
	player.setMoveAndDir(input);
	player.update();
	transforms[0]->setMatrix(player.getMatrix());
}

void SceneManager::setProgram(Geode::GeodeShaderDesc program)
{
	for (auto g : gltfGeodes) {
		g->setProgram(program);
	}
}
