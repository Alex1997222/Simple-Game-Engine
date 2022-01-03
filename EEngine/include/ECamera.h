
#pragma once

#include "EFrustum.h"
#include "EMath.h"

namespace E3D
{
	enum RenderMode
	{
		RENDER_WIRE,	// Wireframe rendering
		RENDER_SOILD,	// Solid rendering
	};

	class ECamera
	{
	public:
		~ECamera();

		void update();

		// Set the distance of the near clipping plane
		void setZNear(EFloat znear);
		// Set the distance of the far clipping plane
		void setZFar(EFloat zfar);

		// Set camera world coordinates
		void setPosition(const EVector3D &pos);
		// Set the camera observation point, when lockTarget is true, lock the observation point
		void setTarget(const EVector3D &target, EBool lockTarget = false);
		void releaseTarget() { mLockTarget = false; }

		// Move based on world matrix
		void move(const EVector3D &mov);
		// Move based on the camera's own coordinate system
		void moveRelative(const EVector3D &mov);

		// Rotate around Y axis
		void yaw(EFloat yDegree);
		// Rotate around X axis
		void pitch(EFloat pDegree);

		// Set the rendering mode, wireframe or entity
		void setRenderMode(RenderMode mode) { mRenderMode = mode; }
		RenderMode getRenderMode() const { return mRenderMode; }

		EFrustum *getFrustum() const { return mFrustum; }
	
	protected:
		friend class ESceneManager;
		ECamera();
		EFrustum	*mFrustum;

		EBool		mLockTarget;
		EBool		mNeedUpdate;

		RenderMode	mRenderMode;
	};
}