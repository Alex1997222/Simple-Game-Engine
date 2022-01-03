
#pragma once
#include "ECommon.h"
#include "EVector.h"
#include "EMatrix.h"
#include "ECore.h"

namespace E3D
{

	enum CAMERA_TYPE
	{
		CAMERA_MODEL_ELUA	= 0x0001,	// elua camera model
		CAMERA_MODEL_UVN	= 0x0002,	// uvn camera model
	};

	struct EFrustum
	{
		EInt		state;		// camera state
		EInt		attribute;	// camera attribute
		CAMERA_TYPE	camMode;

		EVector4D	position;	// camera position in real world
		EVector4D	direction;	// camera direction in real world

		// use of uvn model,used to define uvn camera 
		EVector4D	camUp;		// The heading up vector of the UVN camera model
		EVector4D	camRight;	// The right vector of the UVN camera model 
		EVector4D	camLook;	// The gaze direction of the UVN camera model
		EVector4D	camTarget;		// The gaze point of the UVN camera model

		EFloat		view_dist;	// The distance from the viewpoint to the projection surface

		EFloat		fov;			// Horizontal and vertical field of view

		EFloat		clip_z_near;	// Near cut side
		EFloat		clip_z_far;		// Far cut side

		EPlane3D	clip_plane_R;	// Right cut side
		EPlane3D	clip_plane_L;	// Left cut side
		EPlane3D	clip_plane_T;	// Upper cut surface
		EPlane3D	clip_plane_B;	// Bottom cut surface

		EFloat		viewplane_width;		// viewplane width
		EFloat		viewplane_height;		// viewplane height

		EFloat		viewport_width;			// Projection surface width and height
		EFloat		viewport_height;
		EFloat		viewport_center_X;		// Screen center coordinates
		EFloat		viewprot_center_Y;

		EFloat		aspect_ratio;			// Screen aspect ratio

		EMatrix44	mWorldToCamera;				// World coordinate to camera coordinate transformation matrix
		EMatrix44	mCameraToPerspective;	// Camera coordinate to perspective coordinate system transformation matrix
		EMatrix44	mPerspectiveToScreen;	// Perspective coordinate system to screen coordinate system transformation matrix

		EFrustum(CAMERA_TYPE mode, const EVector4D &pos, const EVector4D &dir,
				const EVector4D& target, EFloat nearZ, EFloat farZ, EFloat ffov, 
				EFloat viewportWidth, EFloat viewportHeight);
	};
}