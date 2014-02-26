#include "DSimDataLogger.h"
#include "DSim.h"
#include "DSimDroplet.h"

extern std::vector<ObjectLocalizationData *> dropletPositions;

DS_RESULT DSimDataLogger::GetDropletPositions(std::vector<ObjectLocalizationData *> *outPosData, DSim& simulator)
{

	std::vector<ObjectLocalizationData *>::iterator in_it, p_it;
	p_it = /*simulator.*/dropletPositions.begin();

	for(in_it = outPosData->begin(); in_it < outPosData->end(); in_it++)
	{
		ObjectLocalizationData *outDat, *dat;
		outDat = (ObjectLocalizationData *)*in_it;
		dat = (ObjectLocalizationData *)*p_it;
		outDat->posX = dat->posX;
		outDat->posY = dat->posY;
		outDat->posZ = dat->posZ;
		outDat->rotA = dat->rotA;
		outDat->rotX = dat->rotX;
		outDat->rotY = dat->rotY;
		outDat->rotZ = dat->rotZ;

		if(p_it < /*simulator.*/dropletPositions.end())
			p_it++;
	}

	return DS_SUCCESS;
}

DS_RESULT DSimDataLogger::GetObjectPositions(std::vector<ObjectLocalizationData *> *outPosData, DSim& simulator)
{

	std::vector<ObjectLocalizationData *>::iterator in_it, p_it;
	p_it = simulator.objectPositions.begin();

	for(in_it = outPosData->begin(); in_it < outPosData->end(); in_it++)
	{
		ObjectLocalizationData *outDat, *dat;
		outDat = (ObjectLocalizationData *)*in_it;
		dat = (ObjectLocalizationData *)*p_it;
		outDat->posX = dat->posX;
		outDat->posY = dat->posY;
		outDat->posZ = dat->posZ;
		outDat->rotA = dat->rotA;
		outDat->rotX = dat->rotX;
		outDat->rotY = dat->rotY;
		outDat->rotZ = dat->rotZ;

		if(p_it < simulator.objectPositions.end())
			p_it++;
	}

	return DS_SUCCESS;
}

DS_RESULT DSimDataLogger::GetDropletColors(std::vector<uint8_t *> *colors, DSim& simulator)
{
	std::vector<DSimDroplet *>::iterator d_it;
	std::vector<uint8_t *>::iterator c_it;
	c_it = colors->begin();

	for(d_it = simulator.droplets.begin(); d_it < simulator.droplets.end(); d_it++)
	{
		DSimDroplet *pDroplet = *d_it;
		ObjectActuationData *actData;
		AccessActuatorData(pDroplet, &actData);

		uint8_t *outColor = (uint8_t *)*c_it;
		outColor[0] = actData->rOut;
		outColor[1] = actData->gOut;
		outColor[2] = actData->bOut;

		if(c_it < colors->end())
			c_it++;
	}

	return DS_SUCCESS;
}

DS_RESULT DSimDataLogger::GetRemainingMotionTimes(std::vector<float *> *times, DSim& simulator)
{
	std::vector<DSimDroplet *>::iterator d_it;
	std::vector<float *>::iterator c_it;
	c_it = times->begin();

	for(d_it = simulator.droplets.begin(); d_it < simulator.droplets.end(); d_it++)
	{
		DSimDroplet *pDroplet = *d_it;
		ObjectActuationData *actData;
		AccessActuatorData(pDroplet, &actData);

		float *outTime = (float *)*c_it;
		outTime[0] = actData->moveTimeRemaining;
		outTime[1] = actData->rotateTimeRemaining;

		if(c_it < times->end())
			c_it++;
	}

	return DS_SUCCESS;
}

DS_RESULT DSimDataLogger::GetMotionDirections(std::vector<DirInfo *> *directions, DSim& simulator)
{
	std::vector<DSimDroplet *>::iterator d_it;
	std::vector<DirInfo *>::iterator c_it;
	c_it = directions->begin();

	for(d_it = simulator.droplets.begin(); d_it < simulator.droplets.end(); d_it++)
	{
		DSimDroplet *pDroplet = *d_it;
		ObjectActuationData *actData;
		AccessActuatorData(pDroplet, &actData);

		DirInfo *outDir = (DirInfo *)*c_it;
		outDir->currMoveDir = actData->currMoveDir;
		outDir->currTurnDir = actData->currTurnDir;

		if(c_it < directions->end())
			c_it++;
	}

	return DS_SUCCESS;
}

DS_RESULT DSimDataLogger::GetSensorColors(std::vector<uint8_t *> *colors, DSim& simulator)
{
	std::vector<DSimDroplet *>::iterator d_it;
	std::vector<uint8_t *>::iterator c_it;
	c_it = colors->begin();

	for(d_it = simulator.droplets.begin(); d_it < simulator.droplets.end(); d_it++)
	{
		DSimDroplet *pDroplet = *d_it;
		ObjectSensorData *senseData;
		AccessSensorData(pDroplet, &senseData);

		uint8_t *outColor = (uint8_t *)*c_it;
		outColor[0] = senseData->rIn;
		outColor[1] = senseData->gIn;
		outColor[2] = senseData->bIn;

		if(c_it < colors->end())
			c_it++;
	}

	return DS_SUCCESS;
}

DS_RESULT DSimDataLogger::GetPhysData(std::vector<ObjectPhysicsData *> *phys, DSim& simulator)
{
	std::vector<DSimDroplet *>::iterator d_it;
	std::vector<ObjectPhysicsData *>::iterator c_it;
	c_it = phys->begin();

	for(d_it = simulator.droplets.begin(); d_it < simulator.droplets.end(); d_it++)
	{
		DSimDroplet *pDroplet = *d_it;
		ObjectPhysicsData *physData;
		AccessPhysicsData(pDroplet, &physData);

		ObjectPhysicsData *outPhys = (ObjectPhysicsData *)*c_it;
		outPhys->mass = physData->mass;
		outPhys->friction = physData->friction;
		outPhys->localInertia = physData->localInertia;
		outPhys->colShapeIndex = physData->colShapeIndex;
		outPhys->_worldID = physData->_worldID;

		if(c_it < phys->end())
			c_it++;
	}

	return DS_SUCCESS;
}

DS_RESULT DSimDataLogger::GetCommData(std::vector<ObjectCommData *> *comm, DSim& simulator)
{
	std::vector<DSimDroplet *>::iterator d_it;
	std::vector<ObjectCommData *>::iterator c_it;
	c_it = comm->begin();

	for(d_it = simulator.droplets.begin(); d_it < simulator.droplets.end(); d_it++)
	{
		DSimDroplet *pDroplet = *d_it;
		ObjectCommData *commData;
		AccessCommData(pDroplet, &commData);

		ObjectCommData *outComm = (ObjectCommData *)*c_it;
		outComm->sendActive = commData->sendActive;
		int i;
		for (i = 0; i < 6; i++)
		{
			outComm->commChannels[i].lastMsgInTimestamp = 
				commData->commChannels[i].lastMsgInTimestamp;
			outComm->commChannels[i].inMsgLength =
				commData->commChannels[i].inMsgLength;
			outComm->commChannels[i].lastMsgOutTimestamp =
				commData->commChannels[i].lastMsgOutTimestamp;
			outComm->commChannels[i].outMsgLength =
				commData->commChannels[i].outMsgLength;
			memcpy(outComm->commChannels[i].inBuf,
				commData->commChannels[i].inBuf, IR_BUFFER_SIZE);
			memcpy(outComm->commChannels[i].outBuf,
				commData->commChannels[i].outBuf, IR_BUFFER_SIZE);
		}
		if(c_it < comm->end())
			c_it++;
	}
	
	return DS_SUCCESS;
}

DS_RESULT DSimDataLogger::GetCompData(std::vector<ObjectPowerData *> *comp, DSim& simulator)
{
	std::vector<DSimDroplet *>::iterator d_it;
	std::vector<ObjectPowerData *>::iterator c_it;
	c_it = comp->begin();

	for(d_it = simulator.droplets.begin(); d_it < simulator.droplets.end(); d_it++)
	{
		DSimDroplet *pDroplet = *d_it;
		ObjectPowerData *compData;
		AccessCompData(pDroplet, &compData);

		ObjectPowerData *outComp = (ObjectPowerData *)*c_it;
		outComp->leg1Status = compData->leg1Status;
		outComp->leg2Status = compData->leg2Status;
		outComp->leg3Status = compData->leg3Status;
		outComp->capStatus = compData->capStatus;
		outComp->objectID = compData->objectID;

		if(c_it < comp->end())
			c_it++;
	}

	return DS_SUCCESS;
}

DS_RESULT DSimDataLogger::GetActuationData(std::vector<ObjectActuationData *> *act, DSim &simulator)
{
	std::vector<DSimDroplet *>::iterator d_it;
	std::vector<ObjectActuationData *>::iterator c_it;
	c_it = act->begin();

	for(d_it = simulator.droplets.begin(); d_it < simulator.droplets.end(); d_it++)
	{
		DSimDroplet *pDroplet = *d_it;
		ObjectActuationData *actData;
		AccessActuatorData(pDroplet, &actData);

		ObjectActuationData *outAct = (ObjectActuationData *)*c_it;
		outAct->rOut = actData->rOut;
		outAct->gOut = actData->gOut;
		outAct->bOut = actData->bOut;
		outAct->currMoveDir = actData->currMoveDir;
		outAct->currTurnDir = actData->currTurnDir;
		outAct->moveTimeRemaining = actData->moveTimeRemaining;
		outAct->rotateTimeRemaining = actData->rotateTimeRemaining;

		if(c_it < act->end())
			c_it++;
	}

	return DS_SUCCESS;
}

uint64_t DSimDataLogger::GetNumCollisions(DSim &simulator)
{
	int numManifolds = simulator.simPhysics->dynWorld->getDispatcher()->getNumManifolds();
	uint64_t numColls = 0;
	for(int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contactManifold = 
			simulator.simPhysics->dynWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		if(obA->isStaticObject() || obB->isStaticObject())
			continue;

		int numContacts = contactManifold->getNumContacts();
		for(int j = 0; j < numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);

			if(pt.getDistance() <= 0.f)
			{
				numColls++;
				break;
			}
		}
	}

	return numColls;
}

double DSimDataLogger::GetTotalRT(DSim& simulator) {
	return simulator.timer.getTotalRT();
}

double DSimDataLogger::GetTotalST(DSim& simulator) {
	return simulator.timer.getTotalST();
}

double DSimDataLogger::GetStepRT(DSim& simulator) {
	return simulator.timer.getStepRT();
}

double DSimDataLogger::GetTotalDiff(DSim& simulator) {
	return simulator.timer.getTotalDiff();
}

double DSimDataLogger::GetTimeRatio(DSim& simulator) {
	return simulator.timer.getTimeRatio();
}
