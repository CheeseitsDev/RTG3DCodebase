#include "GameObjectFactory.h"
#include "GameObject.h"
#include "TransparentGO.h"
#include "OpaqueGO.h"
#include <assert.h>

using std::string;

GameObject* GameObjectFactory::makeNewGO(string _type)
{
	printf("GAME OBJECT TYPE: %s \n", _type.c_str());
	if (_type == "GAME_OBJECT")
	{
		return new GameObject();
	}
	else if (_type == "TRANSPARENT")
	{
		return new TransparentGO();
	}
	else if (_type == "OPAQUE")
	{
		return new OpaqueGO();
	}
	else
	{
		printf("UNKNOWN GAME OBJECT TYPE: %s \n", _type.c_str());
		assert(0);
		return nullptr;
	}
}
