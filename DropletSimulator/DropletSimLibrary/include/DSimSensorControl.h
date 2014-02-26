#pragma once

#include "DSimGlobals.h"
#include "DSimControl.h"
#include "DSimDataStructs.h"
#include "DSimPhysicalObject.h"
#include "DSimProjection.h"

class DSimSensorControl : public DSimControl
{
public:
	DSimSensorControl();
	~DSimSensorControl();

	DS_RESULT Update(DSimPhysicalObject *pObject);
};