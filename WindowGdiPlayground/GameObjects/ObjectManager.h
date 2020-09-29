#pragma once
#include <vector>
#include "Lists/GameObjectsList.h"
#include "Scripts.h"
//gameboard probably going to gameobjects folder and gameobjectslist
template<typename T>
class ObjectManager {
	void Init(int count); // allocs items (max 64k), then Clear()
	void Dispose();       // frees items
	void Clear();         // resets data members, (runs destructors* on outstanding items, *optional)

	T& Alloc();           // alloc (memclear* and/or construct*, *optional) an item from freeList or items[maxUsed++], sets id to (nextKey++ << 16) | index
	void Free(T&);       // puts entry on free list (uses id to store next)

	int GetID(T&);       // accessor to the id part if Item

	T& Get(id);            // return item[id & 0xFFFF]; 
	T* TryToGet(id);      // validates id, then returns item, returns null if invalid.  for cases like AI references and others where 'the thing might have been deleted out from under me'

	bool Next(T*&);      // return next item where id & 0xFFFF0000 != 0 (ie items not on free list)

	struct Item {
		T item;
		int id;             // (key << 16 | index) for alloced entries, (0 | nextFreeIndex) for free list entries
	};

	Item* items;
	uint64_t maxSize;          // total size
	uint64_t maxUsed;          // highest index ever alloced
	uint64_t count;            // num alloced items
	uint64_t nextKey;          // [1..2^16] (don't let == 0)
	uint64_t freeHead;         // index of first free entry

	//prefer go from id->T & as soon as possible, then stay in T& land as much as possible(ie function params etc)
	//and only use ids when required for storage

	//objects will have collision radius, so going through will not be possibble
	//always sorted by position to draw top->bottom. draw after dynamic
};