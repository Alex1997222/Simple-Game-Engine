
#pragma once
#include "ECommon.h"
#include "EMesh.h"
#include "ELight.h"
#include "ECore.h"
#include "ECamera.h"
#include <map>

namespace E3D
{
	//Various resource management
	class ESceneManager
	{
		typedef std::map<EString, EMesh*>::iterator MeshIter;

	public:
		ESceneManager();
		~ESceneManager();

		// draw text
		void drawString(const EString &str, EInt x, EInt y, const EColor &c = EColor(255, 255, 255));

		// Create a model entity
		EMesh* createMesh(const EString &name, const EString &meshName);
		// Get the specified model entity
		EMesh* getMesh(const EString &name);
		// destroy the mesh
		void destroyMesh(const EString&name);

		void clearMesh();

		// Specify to turn on/off the lights (global)
		void enableLights(EBool enable);
		// Whether the light is currently turned on (global)
		EBool isLightsEnable() const { return mLightEnable; }
		// Create a light
		ELight* createLight(LIGHT_TYPE lightType);
		// Get a light, if the light does not exist, return NULL
		ELight*	getLight(EInt id);

		// get the camera pointer
		ECamera* getCamera(){ return mCamera;}

		// Get the number of all (visible/invisible) objects
		EInt getTotalObjectNumber() const { return mMeshs.size(); }
		// Get the number of currently visible objects
		EInt getVisibleObjectNumber() const { return mVisibleObjectNumber; }
		// Get the number of currently visible polyons
		EInt getVisiblePolyonNumber() const { return mVisiblePolyonNumber; }
		// Get the ployon number
		EInt getTotalPolyonNumber() const { return mTotalPolyonNumber; }

		EMesh* createTerrain(const EString& rawFileName, const EString &heightMapFileName, 
								EFloat uTitle = 1, EFloat vTitle = 1, EFloat blockSize = 5.0f);
		EMesh* getTerrain() { return mTerrainMesh; }

		void update();
		
	protected:
		std::map<EString, EMesh*>	mMeshs;
		std::map<EString, EMesh*>	mUsedMesh;
		ERenderList4D				*mRenderList;

		EMesh						*mTerrainMesh;

		ECamera						*mCamera;
		EBool						mLightEnable;

		EInt						mVisibleObjectNumber;
		EInt						mVisiblePolyonNumber;
		EInt						mTotalPolyonNumber;
	};
}