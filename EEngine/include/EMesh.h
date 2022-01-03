
#pragma once

#include "ECommon.h"
#include "ECore.h"
#include "EMaterial.h"
#include "EObject.h"
//Refer to the .mesh format of the orge open source engine
namespace E3D
{
	enum COLLSION_TYPE
	{
		COLLSION_COLLSION,
		COLLSION_IGNORE,
	};

	class EMesh : public EObject
	{
	public:
		~EMesh();

		void setScale(const EVector3D &scale);
		void setPosition(const EVector3D &pos);
		EVector3D getPosition() const;
		void move(const EVector3D &mov);

		// Whether the given point is within the bounding box of Mesh, if so, return true, otherwise return false
		EBool intersect(const EVector3D &point);

		void setVisible(EBool visible) { mIsVisible = visible;}
		EBool isVisible() const { return mIsVisible; }

		// Do you need to perform a culling operation
		void setCullFlag(EBool needCull) {mObject->needCull = needCull; }

		// Rotate around Y axis
		void yaw(EFloat yDegree);
		// Rotate around Z axis
		void roll(EFloat rDegree);
		// Rotate around X axis
		void pitch(EFloat pDegree);

		void setCollsionType(COLLSION_TYPE type) { mCollsionType = type; }
		COLLSION_TYPE getCollsionType() const { return mCollsionType; }

		EMaterial *getMaterial(EInt index = 0);
		void setMateria(EInt index, EString matName);

		EObject4D* getObject() const { return mObject; }
		EInt getPolyonNumber() const { return mPolyonNumber; }
		EInt getVertexNumber() const { return mVertexNumber; }
		EInt getSubmeshNumber() const { return mSubmeshNumber; }

		// Get the rotation component of each axis, expressed in angle
		EVector3D getRotateVec() const { return mRotate; }

		// Clone this model to avoid reading the model again
		EMesh *clone();

	protected:
		friend class ESceneManager;
		EMesh();
		EMesh(const EString &name, const EString &meshName);

		EObject4D		*mObject;

		EInt			mPolyonNumber;	// Number of model polygons
		EInt			mVertexNumber;	// The number of model vertices

		EInt			mSubmeshNumber;	// Number of model submodels

		EVector3D		mRotate;		// The rotation component of each axis of the model

		EBool			mIsVisible;

		COLLSION_TYPE	mCollsionType;
	};
}