
#include "ECommon.h"
#include "EVector.h"
#include <list>

#pragma once
namespace E3D
{

	#define EPOLY_ATTR_2SIDE				0x0001	// double-sided
	#define EPOLY_ATTR_TRANSPARENT			0x0002	// transparent

	#define EPOLY_ATTR_SHADE_MODE_PURE      0x0004
	#define EPOLY_ATTR_SHADE_MODE_CONSTANT  0x0004 // (alias)
	#define EPOLY_ATTR_SHADE_MODE_FLAT      0x0008
	#define EPOLY_ATTR_SHADE_MODE_GOURAUD   0x0010
	#define EPOLY_ATTR_SHADE_MODE_PHONG     0x0020
	#define EPOLY_ATTR_SHADE_MODE_FASTPHONG 0x0020 // (alias)
	#define EPOLY_ATTR_SHADE_MODE_TEXTURE   0x0040 

	#define EPOLY_ATTR_VERTEX_POSITION		0x1000
	#define EPOLY_ATTR_VERTEX_NORMAL		0x2000
	#define EPOLY_ATTR_VERTEX_UV			0x4000

	#define EPOLY_STATE_ACTIVE				0x0100	// active state
	#define EPOLY_STATE_CLIPPED				0x0200	// clipped state
	#define EPOLY_STATE_BACKFACE			0x0400	// Back face

	#define EOBJECT_STATE_ACTIVE			0x0001
	#define EOBJECT_STATE_VISIBLE			0x0002 
	#define EOBJECT_STATE_CULLED			0x0004

	// Object culling type
	enum CULL_TYPE
	{
		CULL_BOX	= 0,	// Object bounding box AABB culling
		CULL_SPHERE	= 1,	// Object ball removal
	};

	struct EVertex4D;
	typedef std::vector<EVertex4D> EVertex4DArray;
	typedef std::vector<EVector4D> EVector4DArray;
	typedef std::vector<EPolyon4D> EPolyon4DArray;

	// Vertex object, including vertices, normals, uv
	struct EVertex4D
	{
		EFloat x, y, z, w;
	
		EFloat u, v;

		EVertex4D();
		EVertex4D(const EVector4D &v);
		EVertex4D(EFloat xf, EFloat yf, EFloat zf);
		EVertex4D operator *(const EVector4D &right);
		EVertex4D operator -(const EVertex4D &right);
		EVertex4D& operator =(const EVertex4D &right);
		EVertex4D operator +(const EVector4D &right);
		EVertex4D operator +(const EVertex4D &right);
		EVertex4D operator /(EFloat factor);
		
		EVector4D toVector4D() const;
	};

	// face
	struct EPlane3D
	{
		EVector4D	point;	// point in sphere
		EVector4D	normal;	// point in line

		EPlane3D(){}
		EPlane3D(const EVector4D &p, const EVector4D &nor);
		EPlane3D& operator=(const EPlane3D &right);
	};

	// Poly4D1_TYPE
	// This polygon structure is a data structure based on vertex indexing
	struct EPolyon4D
	{
		EInt state;			// state information
		EInt attribute;		// attribute information
		EInt color;			// color information

		
		EVertex4DArray	*verList;		// transformList
		EInt			verIndex[3];	// index of vertex

		EPolyon4D();
	};

	// POLYF4DV1_TYP
	// This polygon structure is a data structure based on vertices
	struct EPolyonF4D
	{
		EInt state;			// state information
		EInt attribute;		// physic attribute
		EInt color;			// Polygon light intensity color

		EVertex4D localList[3];			// location of projects
		EVertex4D transformList[3];		// transform list

		struct EMaterial *material;

		EPolyonF4D();
	};

	struct CompEPolyonF4D
	{
		EBool operator()(const EPolyonF4D &polyA, const EPolyonF4D &polyB)
		{			
			EFloat zA = Max(polyA.transformList[0].z, Max(polyA.transformList[1].z, polyA.transformList[2].z));
			EFloat zB = Max(polyB.transformList[0].z, Max(polyB.transformList[1].z, polyB.transformList[2].z));
			// Written here as <will crash in debug mode
			if (Abs(zA - zB) < 0.005f)
			{
				zA = (polyA.transformList[0].z + polyA.transformList[1].z + polyA.transformList[2].z) / 3.0f;
				zB = (polyB.transformList[0].z + polyB.transformList[1].z + polyB.transformList[2].z) / 3.0f;
				
				if (zA <= zB)
					return false;
				else
					return true;
			}
			else if (zA < zB)
				return false;
			else
				return true;
		}
	};

	// Objects based on fixed-point lists and polygonal lists
	struct EObject4D
	{
		EString			name;			// object name
		EInt			state;			// object state
		EInt			attribute;		// object attribute
		EFloat			avgRadius;		// The average radius of the object, used for collision detection
		EFloat			maxRadius;		// the maximum radium

		EBool			needCull;		// Whether to perform a culling operation

		EString			materiaName;	

		EVector4D		minBoundingBox;	// The minimum point of the bounding box of the model cube
		EVector4D		maxBoundingBox;	// The maximum point of the bounding box of the model cube

		// Regardless of whether it is used for review or scaling, the parameters of the first model shall prevail.
		EVector4D		worldPosition;	// The worldposition of vector
		EVector4D		scale;			// The current zoom value of the object

		EVector4D		direction;		// The direction vector of the object, each component records the angle of rotation along each axis
	

		EInt			vertexNumber;	// The vertex number

		EVertex4DArray	localList;		// Object local coordinate array before object transformation
		EVertex4DArray	transformList;	// The coordinates of the object after transformation

		EInt			polyonNumber;	// The number of polygons contained in the object
		EPolyon4DArray	polyonList;		// An array of polygons

		//find the next object
		EObject4D		*nextObject;

		EObject4D();
	};

	// Render List
	struct ERenderList4D
	{
		EInt						state;		// Obeject State
		EInt						attribute;	// Object attribute

		std::list<EPolyonF4D>		polyData;	// Specific polygon data

		typedef std::list<EPolyonF4D>::iterator Itr;
	};

	// Vertex transform control flag
	enum OBJ_TRANSFORM_TYPE
	{
		TRANSFORM_LOCAL = 0,			// Transform only the local vertex list
		TRANSFORM_TRANS = 1,			// Transform only the transformed vertex list
		TRANSFORM_LOCAL_TO_TRANS = 2,	// Transform the local fixed-point list and store the result in the transformed vertex list
	};

	// Transform EObject, you can perform displacement transformation and rotation transformation
	// tansformType specifies which fixed-point list to transform
	// transformBasis specifies whether to transform the orientation vector
	void Tranform_Object4D(	EObject4D* obj, 
							const EMatrix44 &mat, 
							OBJ_TRANSFORM_TYPE tansformType 
							/*,EBool transformBasis = true*/);

	
	// Transform ERenderList4D, you can perform displacement transformation and rotation transformation
	// tansformType specifies which fixed-point list to transform
	void Transform_Renderlist4D(ERenderList4D* renderList, 
								const EMatrix44 &mat, 
								OBJ_TRANSFORM_TYPE transformType);

	// Insert Object into RenderList, where LocalList stores the original coordinates, and TransformList stores the passed
	// Pan and zoom coordinates
	void Inert_Object4D_To_RenderList4D(ERenderList4D *renderList, EObject4D *obj, 
									OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS,
									bool transformScaleAndPosition = false);

	// Transform the local coordinate fixed list to the world coordinate
	void Local_To_World_Object4D(	EObject4D* obj, 
									OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);
	// Transform the local coordinate fixed list to the world coordinate
	void Local_To_World_RenderList4D( ERenderList4D *renderList,
									const EVector4D &pos,
									OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);

	// Culling objects
	bool Cull_Object4D(EObject4D *object, EFrustum *camera, CULL_TYPE cullType= CULL_SPHERE);

	// Backside elimination
	bool Remove_Backface(EPolyon4D *poly, EFrustum *camera);
	bool Remove_Backface(EPolyonF4D *poly, EFrustum *camera);
	void Remove_Backface_Object4D(EObject4D *object, EFrustum *camera);
	// light Whether to illuminate the polygon
	void Remove_Backface_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// Lighting calculation
	void Light_PolyonF4D(EPolyonF4D *poly, EFrustum *camera);
	void Light_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// Transform world coordinates into camera coordinate system
	void World_To_Camera_Object4D(EObject4D *obj, EFrustum *camera);
	void World_To_Camera_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// Depth sorting of the polygons of the RenderList, using the painter algorithm
	void Sort_RenderList4D(ERenderList4D *renderList);

	// Convert camera coordinate system to perspective coordinate system
	void Camera_To_Perspective_Object4D(EObject4D *obj, EFrustum *camera);
	void Camera_To_Perspective_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// The perspective coordinate system is converted to the screen coordinate system
	void Perspective_To_Screen_Object4D(EObject4D *obj, EFrustum *camera);
	void Perspective_To_Screen_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// Perform cropping operations on RenderList4D
	void Clip_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// Transform world coordinates to screen coordinates
	void World_To_Screen_RenderList4D(ERenderList4D *renderList, EFrustum *camera);


	// Render the wireframe model
	void Draw_Object4D_Wire(EObject4D *obj);
	void Draw_RenderList4D_Wire(ERenderList4D *renderList);

	// Render the solid model
	void Draw_Object4D_Solid(EObject4D *object);
	void Draw_RenderList4D_Solid(ERenderList4D *renderList);

	// Draw a textured renderlist
	void Draw_RenderList4D_Teture_Solid(ERenderList4D * renderList);
;}