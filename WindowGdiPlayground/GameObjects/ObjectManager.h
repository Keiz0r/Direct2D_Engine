#pragma once
#include <vector>
#include "Lists/GameObjectsList.h"
#include "Scripts.h"
#include <memory>
#include <algorithm>
//gameboard probably going to gameobjects folder and gameobjectslist

//template<typename T>
class ObjectManager {
public:
	ObjectManager(const int& count);
	~ObjectManager();
	void Dispose();       // frees items
	void Clear();         // resets data members, (runs destructors* on outstanding items, *optional)

	void Alloc();           // alloc (memclear* and/or construct*, *optional) an item from freeList or items[maxUsed++], sets id to (nextKey++ << 16) | index
	void Free(GameObject&);       // puts entry on free list (uses id to store next)

	int GetID(GameObject&);       // accessor to the id part if Item

	GameObject& Get(uint32_t ID);            // return item[id & 0xFFFF]; 
	GameObject* TryToGet(uint32_t ID);      // validates id, then returns item, returns null if invalid.  for cases like AI references and others where 'the thing might have been deleted out from under me'

	bool Next(GameObject*&);      // return next item where id & 0xFFFF0000 != 0 (ie items not on free list)

	struct Item {
		std::unique_ptr<GameObject> obj;
		uint32_t id;             // (key << 16 | index) for alloced entries, (0 | nextFreeIndex) for free list entries
	};
private:
	void Init(const int& count); // allocs items (max 64k), then Clear()
private:
	std::unique_ptr<Item[]> items;
	static constexpr uint16_t maxSize = UINT16_MAX;          // total size
	uint16_t maxUsed = 0;          // highest index ever alloced
	uint16_t count = 0;            // num alloced items
	uint16_t nextKey = 1;          // [1..2^16] (don't let == 0)
	uint16_t freeHead = 0;         // index of first free entry

	//prefer go from id->T& as soon as possible, then stay in T& land as much as possible(ie function params etc)
	//and only use ids when required for storage

	//objects will have collision radius, so going through will not be possibble
	//always sorted by position to draw top->bottom. draw after dynamic
};