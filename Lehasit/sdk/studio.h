#pragma once

// Credits: Valve

#include "Vector.h"

// intersection boxes
struct mstudiobbox_t {
	int					bone;
	int					group;				// intersection group
	Vector				bbmin;				// bounding box
	Vector				bbmax;

	char pad0[9 * sizeof(int)];
};

struct mstudiohitboxset_t {
	int					sznameindex;
	int					numhitboxes;
	int					hitboxindex;
	inline mstudiobbox_t* pHitbox(int i) const {
		return (mstudiobbox_t *)(((char *)this) + hitboxindex) + i;
	};
};

struct studiohdr_t {
	char pad0[(5 * sizeof(int)) + (64 * sizeof(char)) + (6 * sizeof(Vector))];

	int numbones;
	int boneindex;

	char pad1[(2 * sizeof(int))];

	int numhitboxsets;
	int hitboxsetindex;

	mstudiohitboxset_t* pHitboxSet(int i) const {
		return (mstudiohitboxset_t*)(((char *)this) + hitboxsetindex) + i;
	};

	inline mstudiobbox_t* pHitbox(int i, int set) const {
		mstudiohitboxset_t const* pSet = pHitboxSet(set);
		if (!pSet)
			return nullptr;

		return pSet->pHitbox(i);
	};
};