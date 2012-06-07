// ActuatorWorkMeter.cpp
// Author: Frank C. Anderson, Ajay Seth
/*
 * Copyright (c)  2006-12, Stanford University. All rights reserved. 
* Use of the OpenSim software in source form is permitted provided that the following
* conditions are met:
* 	1. The software is used only for non-commercial research and education. It may not
*     be used in relation to any commercial activity.
* 	2. The software is not distributed or redistributed.  Software distribution is allowed 
*     only through https://simtk.org/home/opensim.
* 	3. Use of the OpenSim software or derivatives must be acknowledged in all publications,
*      presentations, or documents describing work in which OpenSim or derivatives are used.
* 	4. Credits to developers may not be removed from executables
*     created from modifications of the source.
* 	5. Modifications of source code must retain the above copyright notice, this list of
*     conditions and the following disclaimer. 
* 
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
*  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
*  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
*  SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
*  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR BUSINESS INTERRUPTION) OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*  WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//=============================================================================
// INCLUDES
//=============================================================================
#include <OpenSim/Simulation/Model/Model.h>
#include <OpenSim/Simulation/Model/BodySet.h>

#include "ActuatorWorkMeter.h"

//=============================================================================
// STATICS
//=============================================================================
using namespace std;
using namespace OpenSim;

static const string WORK_STATE_NAME = "work";

//=============================================================================
// CONSTRUCTORS
//=============================================================================
//_____________________________________________________________________________
/**
 * Default constructor.
 */
ActuatorWorkMeter::ActuatorWorkMeter() 
{
	setNull();
	constructProperties();
}

//_____________________________________________________________________________
/**
 * Convenience constructor.
 */
ActuatorWorkMeter::ActuatorWorkMeter(const Actuator& actuator, 
                                     double initialWork)
{
	setNull();
	constructProperties();

	set_actuator_name(actuator.getName());
	set_initial_actuator_work(initialWork);
}


//=============================================================================
// CONSTRUCTION METHODS
//=============================================================================
//_____________________________________________________________________________
// Set the data members of this ActuatorWorkMeter to their null values.
void ActuatorWorkMeter::setNull(void)
{
    // No data members.
}

//_____________________________________________________________________________
// Connect properties to local pointers.
void ActuatorWorkMeter::constructProperties(void)
{
	constructProperty_actuator_name("Unassigned");
	constructProperty_initial_actuator_work(0.0);
}

//_____________________________________________________________________________
/**
 * Perform some set up functions that happen after the
 * object has been deserialized or copied.
 *
 * @param aModel OpenSim model containing this ActuatorWorkMeter.
 */
void ActuatorWorkMeter::connectToModel(Model& aModel)
{
	Super::connectToModel(aModel);

	const string& actName = get_actuator_name();
	int k = _model->getActuators().getIndex(actName);
	if( k >=0 )
		_actuator = &_model->getActuators().get(k);
	else{
		string errorMessage = "ActuatorWorkMeter: Invalid actuator '" 
                              + actName + "' specified in Actuator.";
		throw (Exception(errorMessage.c_str()));
	}
}

//=============================================================================
// Create the underlying system component(s)
//=============================================================================
void ActuatorWorkMeter::addToSystem(SimTK::MultibodySystem& system) const
{
	Super::addToSystem(system);

	// Assign a name to the state variable to access the work value stored in the state
	string stateName = _actuator->getName()+"."+WORK_STATE_NAME;

	// Add state variables to the underlying system
	addStateVariable(stateName);
}


//=============================================================================
// The state variable derivative (power) to be integrated
//=============================================================================
SimTK::Vector ActuatorWorkMeter::
computeStateVariableDerivatives(const SimTK::State& s) const
{
	SimTK::Vector derivs(1, _actuator->getPower(s));
	double force = _actuator->getForce(s);
	double speed = _actuator->getSpeed(s);
	double power = derivs[0];

	return derivs;
}

 void ActuatorWorkMeter::initStateFromProperties(SimTK::State& s) const
{
	setStateVariable(s, getStateVariableNames()[0], 
        get_initial_actuator_work());
}

void ActuatorWorkMeter::setPropertiesFromState(const SimTK::State& state)
{
    set_initial_actuator_work(getWork(state));
}


//=============================================================================
// GET AND SET
//=============================================================================
//
// Computed work is part of the state
double ActuatorWorkMeter::getWork(const SimTK::State& state) const
{
	return getStateVariable(state, _actuator->getName()+"."+WORK_STATE_NAME);
}