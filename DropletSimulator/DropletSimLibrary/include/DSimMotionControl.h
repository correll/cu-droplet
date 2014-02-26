#pragma once

#include "DSimControl.h"
#include "DSimGlobals.h"
#include "DSimDataStructs.h"
#include "DSimPhysicalObject.h"

class DSimMotionControl : public DSimControl
{
public:
	DSimMotionControl();
	~DSimMotionControl();

	DS_RESULT Update(DSimPhysicalObject *pObject);
};