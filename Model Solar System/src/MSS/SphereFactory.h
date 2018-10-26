// SphereFactory.h -
// Generate s a sphere
#ifndef SPHEREFACTORY_H
#define SPHEREFACTORY_H

#include <vector>

#include <GLM\vec3.hpp>
#include <GLM\vec2.hpp>
#include <GLM\trigonometric.hpp>
#include <GLM\gtc\constants.hpp>

inline void generateSphere(float radius, int rings, int sectors,
	std::vector<glm::vec3>& vertices,
	std::vector<glm::vec2>& uvs,
	std::vector<glm::vec3>& normals,
	std::vector<int>& indices)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors);
	normals.resize(rings * sectors);
	uvs.resize(rings * sectors);
	std::vector<glm::vec3>::iterator v = vertices.begin();
	std::vector<glm::vec3>::iterator n = normals.begin();
	std::vector<glm::vec2>::iterator t = uvs.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		float const y = glm::sin(-glm::half_pi<float>() + glm::pi<float>() * r * R);
		float const x = glm::cos(2 * glm::pi<float>() * s * S) * glm::sin(glm::pi<float>() * r * R);
		float const z = glm::sin(2 * glm::pi<float>() * s * S) * glm::sin(glm::pi<float>() * r * R);

		*t++ = { s * S, r * R };

		*v++ = { x * radius, y * radius, z * radius };

		*n++ = { x, y, z };
	}

	indices.clear();
	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
		indices.push_back(r * sectors + (s + 1));
		indices.push_back(r * sectors + s);
		indices.push_back((r + 1) * sectors + s);

		indices.push_back(r * sectors + (s + 1));
		indices.push_back((r + 1) * sectors + s);
		indices.push_back((r + 1) * sectors + (s + 1));
	}
}

#endif // !SPHEREFACTORY_H
