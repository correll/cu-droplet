#pragma once

#include "DSimControl.h"
#include "DSimGlobals.h"
#include "DSimDataStructs.h"
#include "DSimPhysicalObject.h"
#include "DSimTiming.h"

class DSimTimerControl : public DSimControl
{
public:
	DSimTimerControl();
	~DSimTimerControl();

	DS_RESULT Update(DSimPhysicalObject *pObject);
};