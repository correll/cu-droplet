/**
 * \file	cu-object\DropletSimulator\DropletSimLibrary\include\DSimDataStructs.h
 *
 * \brief	This file contains private data structures to be used by the simulator only!
 *			Public data structures used for returning information to an external caller are 
 *			defined in DSimDataLogger.h.
 */
#pragma once

#ifndef _DROPLET_DATA_STRUCTS
#define _DROPLET_DATA_STRUCTS

#ifdef _WIN32
#include "inttypes.h"   // inttypes.h is included in UNIX systems
#else 
#include <inttypes.h>
#endif

#include <vector>

#include "DSimGlobals.h"
#include "btBulletDynamicsCommon.h"

/**
 * Defines an alias representing information describing the simulator physics.
 */

typedef struct _Simulator_Physics_Data
{
	// Bullet Physics Stuff
	btDefaultCollisionConfiguration		*collisionConfig;
	btCollisionDispatcher				*collisionDispatch;
	btBroadphaseInterface				*broadphase;
	btConstraintSolver					*constraintSolver;
	btDiscreteDynamicsWorld				*dynWorld;
	
	static const int _staticObjCollisionBM	= 1;	// 0b01
	static const int _dynObjCollisionBM		= 2;    // 0b10

	btAlignedObjectArray<btCollisionShape *> *collisionShapes;
	unsigned int _colShapeIDCounter, _physicsWorldObjCounter, _dropletDataIDCounter;	// Values READ ONLY. Handled by simulator.
} SimPhysicsData;

/**
 * Defines an alias representing information describing the object physics.
 */

typedef struct _Object_Physics_Data
{
	btScalar mass, friction;
	btVector3 localInertia;
	int colShapeIndex;	// Collsion Shape Index
	unsigned int _worldID; // Physics World ID. Value READ ONLY. Set automatically by simulator.
	unsigned int _dataID; // Index of object in position and other state data structs. Value is READ ONLY. Set automatically by simulator.

} ObjectPhysicsData;

/**
 * Defines all the capabilities an object has in the simulation.
 */
typedef struct _Object_Capabilities
{
	bool	hasActuation,
			hasLED,
			hasComm,
			hasPower,
			hasSensing,
			hasClock;
} ObjectCapabilities;

/** 
 * Stores localization information for objects.
 */

typedef struct _Object_Localization_Data
{
	float rotX, rotY, rotZ, rotA; // (rotX, rotY, rotZ) describe the axis of rotation
	float posX, posY, posZ;
	bool movedSinceLastUpdate;
	double lastRelPosUpdate;

}ObjectLocalizationData;

typedef struct _Object_LED_Data
{
	uint8_t rOut, gOut, bOut;
	uint8_t hue, sat, bright;
} ObjectLEDData;

/**
 * Defines an alias representing information describing the object actuator.
 */

typedef struct _Object_Actuator_Data
{
	float moveTimeRemaining, rotateTimeRemaining;	// in milliseconds
	float moveStepRemaining, rotateStepRemaining;
	bool _oscillator;
	move_direction currMoveDir;
	turn_direction currTurnDir;

} ObjectActuationData;

/**
 * Defines an alias representing information describing the object sensor.
 */

typedef struct _Object_Sensor_Data
{
	uint8_t rIn, gIn, bIn;	// Color being read by RGB sensor
	
} ObjectSensorData;


/**
 * Defines an internally used struct to store message information per channel.
 */
typedef struct _Object_Comm_Channel_Data
{
	unsigned char inBuf[IR_BUFFER_SIZE];
	unsigned char outBuf[IR_BUFFER_SIZE];
	uint16_t lastMsgOutTimestamp, lastMsgInTimestamp;
	uint16_t outMsgLength, inMsgLength;
} ObjectCommChannelData;

/**
 * Defines an alias representing information describing the object communication.
 */
typedef struct _Object_Comm_Data
{
	bool sendActive;
	ObjectCommChannelData commChannels[NUM_COMM_CHANNELS];

} ObjectCommData;

/**
 * Defines an alias representing information describing the object component.
 */

typedef struct _Object_Power_Data
{
	int8_t leg1Status, leg2Status, leg3Status, capStatus;
} ObjectPowerData;

typedef struct _Object_Timer_Data
{
	float timer[DROPLET_NUM_TIMERS];
	uint8_t trigger[DROPLET_NUM_TIMERS];

} ObjectTimerData;

/**
 * This is a special structure for user-set state variables to track object
 * behavior over the course of an experiment. It is a convenience struct 
 * which has handles in the DSimDataLogger class for extracting experimental
 * data in the middle of simulation. When writing Droplet programs, you may use 
 * this for tracking and storing useful data.
 */
typedef struct _Object_Status
{
	uint8_t	state_changed,
			color_changed,
			msg_sent,
			msg_recv;
} ObjectStatus;

#endif
