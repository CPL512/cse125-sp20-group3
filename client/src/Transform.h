#pragma once

#include <algorithm>

#include "../The-Forge/Common_3/Renderer/IResourceLoader.h"
#include "../The-Forge/Common_3/OS/Math/MathTypes.h"

#include "Node.h"

class Transform : public Node {
protected:
	mat4 M;
	std::vector<Node*> children;
public:
	bool active = true;
	vec4 color = vec4(1);

	Transform();
	Transform(mat4 transformation);
	~Transform();

	void addChild(Node* child);
	void removeChild(Node* child);

	void update(float deltaTime) override;
	void updateTransformBuffer(BufferUpdateDesc& desc, mat4 parentTransform, vec4 color) override;
	void updateBoneBuffer(BufferUpdateDesc& desc, OzzObject::UniformDataBones* boneData) override;
	void updateParticleBuffer(BufferUpdateDesc& desc) override;
	void cull(const vec4 planes[6], bool doCull) override;
	void draw(Cmd* cmd) override;

	void setMatrix(mat4 m);
	mat4 getMatrix() { return M; }
	void setPositionDirection(vec3 position, vec3 direction, vec3 up);
	void setPositionDirection(vec3 position, vec3 direction);
	void setPositionDirection(vec3 position, float angle);

	void setColor(vec4 c);
};