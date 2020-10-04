#include "ObjectManager.h"


//void sss() {
//	ObjectManager<GameObject> ojb;
//	ojb.Init();
//}

ObjectManager::ObjectManager(const uint16_t& count) {
	items = std::make_unique<Item[]>(count);
}

ObjectManager::~ObjectManager() {
}