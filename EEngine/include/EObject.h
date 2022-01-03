
#pragma once
#include "ECommon.h"

namespace E3D
{
	class EObject
	{
	public:
		virtual ~EObject(){}

		virtual EString getName() const { return mName; }
		virtual void update() {}

		// judge whether the target is alive
		virtual EBool isAlive() const { return mCurrentLive < mMaxLive; }
	
	protected:
		EObject() : mMaxLive(1), mCurrentLive(0){}

		EString	mName;

		EInt			mMaxLive;		// the max value of life
		EInt			mCurrentLive;	// current life
	};
}