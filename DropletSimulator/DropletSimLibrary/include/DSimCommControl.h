#pragma once

#include "DSimGlobals.h"
#include "DSimDataStructs.h"
#include "DSimControl.h"
#include "DSimPhysicalObject.h"

class DSimCommControl : public DSimControl
{
public:
	DSimCommControl();
	~DSimCommControl();

	DS_RESULT Update(DSimPhysicalObject *pObject);
};