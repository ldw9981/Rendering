#pragma once

namespace Graphics
{

class Entity
{
public:
	Entity(void);
	virtual ~Entity(void);

	int uniqueID;
	int gameEntityID;
};
}