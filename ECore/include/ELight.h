#pragma once

#include "ECommon.h"
#include "EVector.h"

namespace E3D
{
	enum  LIGHT_TYPE
	{
		LIGHT_AMBLENT = 0,			// environment light
		LIGHT_DIRECTION = 1,		// parallel light
		LIGHT_POINT = 2,			// point light

	};


	struct ELight
	{
		EInt id;						// unique id of light
		EBool lightOn;					// the status of light

		LIGHT_TYPE lightType;			// light type

		EFloat power;					// light intensity
		EFloat shadowFactor;			// light back intensity
		EColor ambient;					// ambinent color of light
		EColor diffuse;					// diffuse of light
		EColor specular;				// specular color of light


		EVector4D position;				// light world coordinate position
		EVector4D direction;			// light direction
		EVector4D transPosition;		// world coordinate position of light camera
		EVector4D transDirection;		// world coordinate direction of light camera

		EFloat kc, kl, kq;				// attenuation factor

		EFloat spot_inner;				// inner cone angle of spotlight
		EFloat spot_outer;				// outter cone angle of spotlight
		EFloat spot_pf;					// spot light index factor

		ELight(EInt lightId, LIGHT_TYPE type);

	};
	
	// maxium number of lights
	#define MAX_LIGHTS 8

	// global light management
	extern std::vector<ELight*> *Glights;

	typedef std::vector<ELight*>::iterator LightItr;

	// get a light and if light is not exist then return null
	extern EInt CreateLight(LIGHT_TYPE lightType);
	extern EInt GetLight(EInt id);
	extern EInt GetLightSize();

	// destory all light
	extern void DestroyAllLight();

}
