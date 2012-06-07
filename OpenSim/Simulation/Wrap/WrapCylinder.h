#ifndef __WrapCylinder_h__
#define __WrapCylinder_h__

// WrapCylinder.h
// Author: Peter Loan
/*
 * Copyright (c)  2006, Stanford University. All rights reserved. 
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


// INCLUDE
#include <iostream>
#include <string>
#include <OpenSim/Simulation/osimSimulationDLL.h>
#include <OpenSim/Common/Object.h>
#include <OpenSim/Common/VisibleObject.h>
#include <OpenSim/Common/PropertyDbl.h>
#include "WrapObject.h"

namespace OpenSim {

class VisibleObject;
class Body;
class Model;
class PathPoint;
class PathWrap;
class WrapResult;

//=============================================================================
//=============================================================================
/**
 * A class implementing a cylinder for muscle wrapping.
 *
 * @author Peter Loan
 * @version 1.0
 */
class OSIMSIMULATION_API WrapCylinder : public WrapObject {
OpenSim_DECLARE_CONCRETE_OBJECT(WrapCylinder, WrapObject);

//=============================================================================
// DATA
//=============================================================================

	PropertyDbl _radiusProp;
	double& _radius;

	PropertyDbl _lengthProp;
	double& _length;

//=============================================================================
// METHODS
//=============================================================================
	//--------------------------------------------------------------------------
	// CONSTRUCTION
	//--------------------------------------------------------------------------
public:
	WrapCylinder();
	WrapCylinder(const WrapCylinder& aWrapCylinder);
	virtual ~WrapCylinder();

#ifndef SWIG
	WrapCylinder& operator=(const WrapCylinder& aWrapCylinder);
#endif
   void copyData(const WrapCylinder& aWrapCylinder);

	double getRadius() const { return _radius; }
	void setRadius(double aRadius) { _radius = aRadius; }
	double getLength() const { return _length; }
	void setLength(double aLength) { _length = aLength; }

	virtual const char* getWrapTypeName() const;
	virtual std::string getDimensionsString() const;
	virtual void scale(const SimTK::Vec3& aScaleFactors);

	virtual void connectToModelAndBody(Model& aModel, OpenSim::Body& aBody);
#ifndef SWIG
	virtual int wrapLine(const SimTK::State& s, SimTK::Vec3& aPoint1, SimTK::Vec3& aPoint2,
		const PathWrap& aPathWrap, WrapResult& aWrapResult, bool& aFlag) const;
#endif
protected:
	void setupProperties();

private:
	void setNull();
	void _make_spiral_path(SimTK::Vec3& aPoint1, SimTK::Vec3& aPoint2,
												 bool far_side_wrap,WrapResult& aWrapResult) const;
	void _calc_spiral_wrap_point(const SimTK::Vec3& r1a,
														 const SimTK::Vec3& axial_vec,
														 double m[4][4],
														 const SimTK::Vec3& axis,
														 double sense,
														 double t,
														 double theta,
														 SimTK::Vec3& wrap_pt) const;


	bool _adjust_tangent_point(SimTK::Vec3& pt1,
													  SimTK::Vec3& dn,
													  SimTK::Vec3& r1,
													  SimTK::Vec3& w1) const;

//=============================================================================
};	// END of class WrapCylinder
//=============================================================================
//=============================================================================

} // end of namespace OpenSim

#endif // __WrapCylinder_h__


