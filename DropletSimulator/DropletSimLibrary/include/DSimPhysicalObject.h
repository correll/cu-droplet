#pragma once

#ifndef _DSIM_PHYSICAL_OBJECT
#define _DSIM_PHYSICAL_OBJECT

#ifdef _WIN32
#include "inttypes.h"   // inttypes.h is included in UNIX systems
#else 
#include <inttypes.h>
#endif

#include "btBulletDynamicsCommon.h"
#include "DSimGlobals.h"
#include "DSimDataStructs.h"

#include <cstdlib>
#include <utility>

class DSimPhysicalObject
{
private:
	ObjectPhysicsData *phyData;
	ObjectLocalizationData *locData;
	ObjectCapabilities *capData;

	ObjectActuationData *actData;
	ObjectSensorData *senseData;
	ObjectCommData *commData;
	ObjectPowerData *compData;
	ObjectTimerData *timerData;
	ObjectLEDData *ledData;

	object_id_t objectID;

public:

	DSimPhysicalObject(ObjectPhysicsData *phyData);

	DS_RESULT _InitPhysics(
		SimPhysicsData *simPhysics,
		std::pair<float, float> startPosition,
		float startAngle);

	DS_RESULT _InitPhysics(
		SimPhysicsData *simPhysics,
		std::pair<float, float> startPosition,
		float startHeight,
		float startAngle);
};

#endif
