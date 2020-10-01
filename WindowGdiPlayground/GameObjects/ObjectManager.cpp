#include "ObjectManager.h"


//void sss() {
//	ObjectManager<GameObject> ojb;
//	ojb.Init();
//}

ObjectManager::ObjectManager(const int& count) {
	std::vector<size_t> sizes{ sizeof(Sonic), sizeof(Barrel), sizeof(Tree) };
	static size_t objsize = *std::max_element(sizes.begin(), sizes.end());
	Init(count);
}

ObjectManager::~ObjectManager() {
}

void ObjectManager::Alloc() {
	D2D1_POINT_2F tmp{ -12.0f, 18.0f };
	items[freeHead].obj = std::make_unique<Tree>( tmp , 1.0f, 1.0f, 0.0f, false);	//where do I allocate? nextkey or freehead? how to decide?
	items[freeHead].id = nextKey << 16;
	nextKey++;
}

void ObjectManager::Init(const int& count) {
	items = std::make_unique<Item[]>(count);
}
