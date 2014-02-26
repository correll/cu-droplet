/* *** PROGRAM DESCRIPTION ***
 * 
 */
#pragma once

#ifndef _DROPLET_CUSTOM_FOUR
#define _DROPLET_CUSTOM_FOUR

#include "DSimDroplet.h"
#include "DSimGlobals.h"
#include "DSimDataStructs.h"

#include <algorithm>
#include <map>
#include <vector>
#include <inttypes.h>
#include <stdlib.h>

class DropletCustomFour : public DSimDroplet
{
private :

	enum State
	{
		IT,
		NOT_IT
	} state;

	std::vector<object_id_t> other_droplets;
	char ack;
	char marco;
	char polo;
	object_id_t my_id, target_id;
	bool it_init_required, notit_init_required, wait_for_ack, toggle;
public :
	DropletCustomFour(ObjectPhysicsData *phyData);
	~DropletCustomFour(void);
	std::map<object_id_t,bool> droplet_status;
	void acquireNewTarget();
	void DropletInit(void);
	void DropletMainLoop(void);
};

#endif