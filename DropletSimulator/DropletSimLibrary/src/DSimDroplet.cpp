#include "DSimDroplet.h"

/* Having access to TrigArray in Droplet code is ONLY for implementing Range and Bearing.
 * This is dangerous bacause DSim is where dropletRelPos really gets defined even
 * though the variable is defined in DSimUtil.cpp. Notice that DSim.h isn't and
 * cannot be included in DSimDroplet.h but dropletRelPos is affected by it. 
 */
extern TrigArray *dropletRelPos; // Welcome to the DAINJA' ZONE!
extern std::vector<ObjectLocalizationData *> dropletPositions;

float prettyAngle(float ang){
	ang = fmodf(ang,360);
	if(ang<-180){
		ang+=360;
	}else if(ang>180){
		ang-=360;
	}
	return ang;
}


// Constructors and Destructor
DSimDroplet::DSimDroplet(ObjectPhysicsData *phyData)
{
	this->phyData = phyData;
	actData		= (ObjectActuationData *)malloc(sizeof(ObjectActuationData));
	senseData	= (ObjectSensorData *)malloc(sizeof(ObjectSensorData));
	commData	= (ObjectCommData *)malloc(sizeof(ObjectCommData));
	compData	= (ObjectPowerData *)malloc(sizeof(ObjectPowerData));
	timeData	= (ObjectTimerData *)malloc(sizeof(ObjectTimerData));
	statData	= (ObjectStatus *)malloc(sizeof(ObjectStatus));
	this->global_rx_buffer.buf = (uint8_t *)malloc(sizeof(uint8_t) * IR_BUFFER_SIZE);
}
	
DSimDroplet::~DSimDroplet()
{
	free(phyData);
	free(actData);
	free(senseData);
	free(commData);
	free(compData);
	free(statData);

	phyData = NULL;
	actData = NULL;
	senseData = NULL;
	commData = NULL;
	compData = NULL;
}

DS_RESULT DSimDroplet::_InitPhysics(
	SimPhysicsData *simPhysics, 
	std::pair<float, float> startPosition,
	float startAngle)
{
	// Set up the bullet physics object for this Droplet
	btCollisionShape *colShape = simPhysics->collisionShapes->at(phyData->colShapeIndex);

	colShape->calculateLocalInertia(phyData->mass, phyData->localInertia);
	btTransform temp;
	temp.setIdentity();
	btVector3 tmpHolder1, tmpHolder2; // Don't really need this for anything besides calling getBoundingSphere
	colShape->getAabb(temp,tmpHolder1, tmpHolder2);

	float height = 0.f - tmpHolder1.z();

	// Set the Droplet's initial position in the world. Remember that bullet uses a Y-Up coord system
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(
		btScalar(startPosition.first), 
		btScalar(startPosition.second),
		btScalar(height + 0.01f))
	);

	// Set this Droplet's angular orientation using input parameter startAngle
	btQuaternion rotationQuat; 
	rotationQuat.setRotation(btVector3(
			btScalar(0.0), 
			btScalar(0.0),
			btScalar(1.0)
		), 
		btScalar(startAngle)
	);
	startTransform.setRotation(rotationQuat);

	// Add this Droplet as a rigid body in the physics sim
	btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(
		phyData->mass, 
		motionState, 
		colShape, 
		phyData->localInertia
	);
	rbInfo.m_friction = phyData->friction;
	rbInfo.m_linearDamping = DROPLET_LINEAR_DAMPING;		// Add a damping coefficient
	rbInfo.m_angularDamping = DROPLET_ANGULAR_DAMPING;

	btRigidBody *body = new btRigidBody(rbInfo);
	body->activate(true);
	body->setActivationState(DISABLE_DEACTIVATION);

	
	// Constrain movement to XY axes and rotations along the Z axis only
	//body->setLinearFactor(btVector3(1, 1, 1));
	//body->setAngularFactor(btVector3(0, 0, 1));

	simPhysics->dynWorld->addRigidBody(		
		body, 
		SimPhysicsData::_dynObjCollisionBM,	// Droplet Collision bitmask
		SimPhysicsData::_staticObjCollisionBM | 
			SimPhysicsData::_dynObjCollisionBM); // Droplet collides with these objects

	// Set the Droplet's ID
	phyData->_worldID = simPhysics->_physicsWorldObjCounter++;
	objectID = static_cast<object_id_t>(phyData->_worldID + DROPLET_ID_START);

	return DS_SUCCESS;
}

// Droplet Subsystems Setup Functions
void DSimDroplet::init_all_systems() 
{
	reset_motors();
	reset_rgb_led();
	reset_rgb_sensor();
	reset_timers();
	for(uint8_t i = 0; i < NUM_COMM_CHANNELS; i++)
		reset_ir_sensor(i);

	// Initialize variables
	commData->sendActive = false;
	compData->capStatus = 0;
	msg_return_order = OLDEST_MSG_FIRST;
}

void droplet_reboot(void)
{
	// TODO
}

void DSimDroplet::reset_rgb_led() 
{
	actData->rOut = 250;
	actData->gOut = 250;
	actData->bOut = 250;
}


void DSimDroplet::reset_ir_sensor(uint8_t sensor_num) 
{
	if(sensor_num >= 0 && sensor_num < NUM_COMM_CHANNELS)
	{
		memset(commData->commChannels[sensor_num].inBuf, 0, IR_BUFFER_SIZE);
		memset(commData->commChannels[sensor_num].outBuf, 0, IR_BUFFER_SIZE);
		commData->commChannels[sensor_num].inMsgLength = 0;
		commData->commChannels[sensor_num].outMsgLength = 0;
		commData->commChannels[sensor_num].lastMsgInTimestamp = 0;
		commData->commChannels[sensor_num].lastMsgOutTimestamp = 0;
	}
}

void DSimDroplet::reset_rgb_sensor() 
{
	senseData->rIn = 0;
	senseData->gIn = 0;
	senseData->bIn = 0;
}

void DSimDroplet::reset_motors() 
{
	actData->currMoveDir			= MOVE_OFF;
	actData->currTurnDir			= TURN_OFF;
	actData->moveTimeRemaining		= 0;
	actData->rotateTimeRemaining	= 0;
	actData->moveStepRemaining		= 0;
	actData->rotateStepRemaining	= 0;
	actData->_oscillator			= true;
}

void DSimDroplet::reset_timers()
{
	for(int i = 0; i < DROPLET_NUM_TIMERS; i++)
	{
		timeData->timer[i] = 0;
		timeData->trigger[i] = 1;
	}
}

object_id_t DSimDroplet::get_droplet_id()
{
	return objectID;
}

uint8_t DSimDroplet::rand_byte(void)
{
	return static_cast<uint8_t>(255 * ((float)rand() / RAND_MAX));
}

// Droplet Motion Subsystem Functions
void DSimDroplet::move_duration(move_direction direction, uint16_t duration)
{
	actData->moveTimeRemaining = static_cast<float>(duration);
	actData->currMoveDir = direction;
}

void DSimDroplet::move_steps(move_direction direction, uint16_t num_steps)
{
	actData->moveStepRemaining = static_cast<float>(num_steps * WALK_STEP_TIME);
	actData->currMoveDir = direction;
}
void DSimDroplet::rotate_duration(turn_direction direction, uint16_t duration)
{
	actData->rotateTimeRemaining = static_cast<float>(duration);
	actData->currTurnDir = direction;
}

void DSimDroplet::rotate_steps(turn_direction direction, uint16_t num_steps)
{
	actData->rotateStepRemaining = static_cast<float>(num_steps * WALK_STEP_TIME);
	actData->currTurnDir = direction;
}

void DSimDroplet::rotate_degrees(int16_t deg)
{
	deg = static_cast<int16_t>(prettyAngle(static_cast<float>(deg)));
	if(deg > 0)
	{
		rotate_duration(TURN_COUNTERCLOCKWISE, ROTATE_RATE_MS_PER_DEG * deg);
	}
	else if(deg < 0)
	{
		rotate_duration(TURN_CLOCKWISE, -1 * ROTATE_RATE_MS_PER_DEG * deg);
	}
}

uint32_t DSimDroplet::cancel_move() 
{
	float tmp;
	if ( actData->moveStepRemaining > 0){
		tmp = actData->moveStepRemaining * ( 60.f / 1000.f);
		actData->moveStepRemaining = 0;
		actData->moveTimeRemaining = 0;
		return static_cast<uint32_t>(tmp);
	}
	else{
		tmp = actData->moveTimeRemaining;
		actData->moveTimeRemaining = 0;
		actData->moveStepRemaining = 0;
		return static_cast<uint32_t>(tmp);
	}
}

uint32_t DSimDroplet::cancel_rotate() 
{
	float tmp;
	if ( actData->rotateStepRemaining > 0){
		tmp = actData->rotateStepRemaining * ( 60.f / 1000.f);
		actData->rotateStepRemaining = 0;
		actData->rotateTimeRemaining = 0;
		return static_cast<uint32_t>(tmp);
	}
	else{
		tmp = actData->rotateTimeRemaining;
		actData->rotateTimeRemaining = 0;
		actData->rotateStepRemaining = 0;
		return static_cast<uint32_t>(tmp);
	}
}

uint8_t DSimDroplet::is_moving()
{
	if(actData->moveTimeRemaining <= 0.f && 
		actData->moveStepRemaining <= 0.f)
		return 0;
	else
		return actData->currMoveDir;
}

//uint8_t DSimDroplet::is_rotating()
turn_direction DSimDroplet::is_rotating() 
{
	if(actData->rotateTimeRemaining <= 0.f &&
		actData->rotateStepRemaining <= 0.f)
		return 0;
	else
		return actData->currTurnDir;
}

int8_t DSimDroplet::leg1_status()
{
	return compData->leg1Status;
}

int8_t DSimDroplet::leg2_status()
{
	return compData->leg2Status;
}

int8_t DSimDroplet::leg3_status()
{
	return compData->leg3Status;
}

int8_t DSimDroplet::leg_status(uint8_t leg)
{
	switch(leg)
	{
	case 1:
		return compData->leg1Status;
		break;

	case 2:
		return compData->leg2Status;
		break;

	case 3:
		return compData->leg3Status;
		break;

	default:
		return 2;
	}

	return 2;
}
int8_t DSimDroplet::cap_status(void)
{
	return compData->capStatus;
}

void DSimDroplet::set_rgb_led(uint8_t r, uint8_t g, uint8_t b)
{
	actData->rOut = r;
	actData->gOut = g;
	actData->bOut = b;
}

void DSimDroplet::set_red_led(uint8_t saturation)
{
	actData->rOut = saturation;
}

void DSimDroplet::set_green_led(uint8_t saturation)
{
	actData->gOut = saturation;
}

void DSimDroplet::set_blue_led(uint8_t saturation)
{
	actData->bOut = saturation;
}

uint8_t DSimDroplet::get_red_led(void)
{
	return actData->rOut;
}

uint8_t DSimDroplet::get_green_led(void)
{
	return actData->gOut;
}

uint8_t DSimDroplet::get_blue_led(void)
{
	return actData->bOut;
}

void DSimDroplet::led_off(void)
{
	reset_rgb_led();
}

void DSimDroplet::get_rgb_sensor(uint8_t *r, uint8_t *g, uint8_t *b)
{
	*r = senseData->rIn;
	*g = senseData->gIn;
	*b = senseData->bIn;
}

uint8_t DSimDroplet::get_red_sensor(void)
{
	return senseData->rIn;
}

uint8_t DSimDroplet::get_green_sensor(void)
{
	return senseData->gIn;
}

uint8_t DSimDroplet::get_blue_sensor(void)
{
	return senseData->bIn;
}

uint8_t DSimDroplet::ir_broadcast(const char *send_buf, uint8_t length)
{
	return ir_send(0, send_buf, length);
}

uint8_t DSimDroplet::ir_send(uint8_t channel, const char *send_buf, uint8_t length)
{
	// Channel 0 is reserved for broadcast
	if(channel >= NUM_COMM_CHANNELS)
		return 0;

	/* TODO : This section of code finds an empty channel to fill the send buffer in.
	 * Once directed communication is implemented, this block of code should be removed 
	 * and the channel input parameter should be used instead.
	 */
	unsigned int sendChannel;
	for(sendChannel = 0; sendChannel < NUM_COMM_CHANNELS; sendChannel++)
	{
		if(commData->commChannels[sendChannel].outMsgLength == 0)
			break;
	}

	// Reset the out comm buffer
	memset(commData->commChannels[sendChannel].outBuf, 0, IR_BUFFER_SIZE);
	//commData->commChannels[sendChannel].outMsgLength = 0;

	// Store the sending droplet's id as part of the message header
	memcpy(commData->commChannels[sendChannel].outBuf, &(compData->objectID), sizeof(object_id_t));

	// Store the rest of the message header, then body
	uint8_t msgLength = length < IR_MAX_DATA_SIZE ? length : IR_MAX_DATA_SIZE;
	memcpy(&commData->commChannels[sendChannel].outBuf[sizeof(object_id_t)], &msgLength, sizeof(uint8_t));
	memcpy(&commData->commChannels[sendChannel].outBuf[IR_MSG_HEADER], send_buf, msgLength);
	commData->commChannels[sendChannel].outMsgLength = msgLength + IR_MSG_HEADER;

	// Set Send to active
	commData->sendActive = true;
	return 1;
}

uint8_t DSimDroplet::check_for_new_messages(void)
{
	int newMsgCh = -1;
	for(int i = 0; i < NUM_COMM_CHANNELS; i++)
	{
		if(commData->commChannels[i].inMsgLength > 0)
		{
			newMsgCh = i;
			break;
			//if(newMsgCh == -1)
			//	newMsgCh = i;
			//else
			//{
			//	if(commData->commChannels[i].lastMsgInTimestamp <
			//		commData->commChannels[newMsgCh].lastMsgInTimestamp)
			//	{
			//		if(msg_return_order == OLDEST_MSG_FIRST)
			//			newMsgCh = i;
			//	}
			//	else
			//	{
			//		if(msg_return_order == NEWEST_MSG_FIRST)
			//			newMsgCh = i;
			//	}
			//}
		}
	}

	// If there is indeed a new message then we should copy it into the global_rx_buffer
	if(newMsgCh != -1)
	{
		global_rx_buffer.size = commData->commChannels[newMsgCh].inMsgLength;
		global_rx_buffer.data_len = global_rx_buffer.size - IR_MSG_HEADER;
		global_rx_buffer.message_time = commData->commChannels[newMsgCh].lastMsgInTimestamp;
		memset(global_rx_buffer.buf, 0, IR_BUFFER_SIZE);
		memcpy( 
			global_rx_buffer.buf,
			&commData->commChannels[newMsgCh].inBuf[IR_MSG_HEADER],
			global_rx_buffer.data_len);
		memcpy(
			&global_rx_buffer.sender_ID,
			commData->commChannels[newMsgCh].inBuf,
			sizeof(object_id_t));

		// Clean out this message from the in buffer
		commData->commChannels[newMsgCh].inMsgLength = 0;
	}

	return (newMsgCh == -1) ? 0 : 1;
}

uint8_t DSimDroplet::set_timer(uint16_t time, uint8_t index)
{
	if(index >= DROPLET_NUM_TIMERS) return 0;

	timeData->timer[index] = static_cast<float>(time);
	timeData->trigger[index] = 0;

	return 1;
}
 
uint8_t DSimDroplet::check_timer(uint8_t index)
{
	if(index >= DROPLET_NUM_TIMERS) return 0;

	return timeData->trigger[index];
}

uint8_t DSimDroplet::range_and_bearing(uint16_t partner_id, float *dist, float *theta, float *phi)
{
	float angle;
	unsigned int partner_world_id = static_cast<unsigned int>(partner_id - DROPLET_ID_START);
	unsigned int data_id_diff = this->phyData->_worldID - this->phyData->_dataID;
	unsigned int partner_data_id = partner_world_id - data_id_diff;

	// Get distance and relative angle
	if(dropletRelPos->GetData(
		this->phyData->_dataID, 
		partner_data_id, 
		dist, 
		&angle) != DS_SUCCESS)
	{ 
		return 0;
	}
	angle *= 180 / SIMD_PI; // Radians to Degrees

	// Find Theta
	float tauRX;
	if(dropletPositions[this->phyData->_dataID]->rotZ < 0)
		tauRX = dropletPositions[this->phyData->_dataID]->rotA * -180 / SIMD_PI;
	else
		tauRX = dropletPositions[this->phyData->_dataID]->rotA * 180 / SIMD_PI;
	*theta = prettyAngle(angle - tauRX);

	// Find Phi
	float tauTX;
	if(dropletPositions[partner_data_id]->rotZ < 0)
		tauTX = dropletPositions[partner_data_id]->rotA * -180 / SIMD_PI;
	else
		tauTX = dropletPositions[partner_data_id]->rotA * 180 / SIMD_PI;
	*phi = prettyAngle(tauTX - tauRX); 

	return 1;
}



void DSimDroplet::DropletInit(void) { return; }
void DSimDroplet::DropletMainLoop(void) { return; }
