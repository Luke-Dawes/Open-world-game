#pragma once

#include <memory>
#include "Mesh.h"
#include "Shader.h"

class Drawable {

public:
	virtual void draw(const glm::mat4& view, const glm::mat4& projection) = 0;
};
