/**
 * \file	cu-object\DropletSimulator\DropletSimLibrary\include\DSimControl.h
 *
 * \brief	Abstract base class that with "update()" function that all simulation
 * control classes must implment.
 */
#pragma once

#include "DSimPhysicalObject.h"
#include "DSimGlobals.h"

class DSimControl
{
public:

	DSimControl();
	virtual ~DSimControl();

	virtual DS_RESULT Update(DSimPhysicalObject *pObject) = 0;
};
