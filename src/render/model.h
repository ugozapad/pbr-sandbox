#pragma once

#include "render/material.h"

class Model
{
public:
	static Model* create_from_file(const char* filename);
};