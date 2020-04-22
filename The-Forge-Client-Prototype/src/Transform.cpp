#include "Transform.h"

Transform::Transform(mat4 transformation)
{
	this->M = transformation;
}

Transform::~Transform()
{
	for (auto child : children) {
		conf_delete(child);
	}
}

void Transform::unload()
{
	for (auto child : children) {
		child->unload();
	}
}

void Transform::addChild(Node* child)
{
	children.push_back(child);
}

void Transform::removeChild(Node* child)
{
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void Transform::update(float deltaTime)
{
	for (auto child : children) {
		child->update(deltaTime);
	}
}

void Transform::updateTransformBuffer(BufferUpdateDesc& desc, mat4 parentTransform)
{
	for (auto child : children) {
		child->updateTransformBuffer(desc, parentTransform * this->M);
	}
}

void Transform::draw(Cmd* cmd)
{
	for (auto child : children) {
		child->draw(cmd);
	}
}

void Transform::setMatrix(mat4 m)
{
	this->M = m;
}

void Transform::setPositionDirection(vec3 position, vec3 direction, vec3 up)
{
	vec3 forward = normalize(direction);
	vec3 right = cross(forward, up);

	this->M[0] = vec4(right, 0);
	this->M[1] = vec4(up, 0);
	this->M[2] = vec4(-forward, 0);
	this->M[3] = vec4(position, 1);
}

void Transform::setPositionDirection(vec3 position, vec3 direction)
{
	setPositionDirection(position, direction, vec3(0, 1, 0));
}

void Transform::setPositionDirection(vec3 position, float angle)
{
	vec3 direction = vec3(cos(angle), 0, sin(angle));
	setPositionDirection(position, direction);
}