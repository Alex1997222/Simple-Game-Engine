#include "ECore.h"
#include "EFrustum.h"
#include "EGraphics.h"
#include "ELight.h"
#include "EMaterial.h"

namespace E3D
{
	

	EVertex4D::EVertex4D() : x(0.0f), y(0.0f), z(0.0f), w(1.0f), 
							u(0.0f), v(0.0f){}

	

	EVertex4D::EVertex4D(const EVector4D &v):x(v.x), y(v.y), z(v.z), w(v.w),

							u(0.0f), v(0.0f){}

	

	EVertex4D::EVertex4D(EFloat xf, EFloat yf, EFloat zf):x(xf), y(yf), z(zf), w(1),
							u(0.0f), v(0.0f){}

	

	EVertex4D EVertex4D::operator *(const EVector4D &right)
	{
		EVertex4D temp = *this;
		temp.x *= right.x, temp.y *= right.y, temp.z *= right.z;
		return temp;
	}

	

	EVertex4D EVertex4D::operator -(const EVertex4D &right)
	{
		EVertex4D temp = *this;
		temp.x -= right.x, temp.y -= right.y, temp.z -= right.z;
		return temp;
	}

	


	EVertex4D EVertex4D::operator +(const EVector4D &right)
	{
		EVertex4D temp = *this;
		temp.x += right.x, temp.y += right.y, temp.z += right.z;
		return temp;
	}

	

	EVertex4D EVertex4D::operator +(const EVertex4D &right)
	{
		EVertex4D temp = *this;
		temp.x += right.x, temp.y += right.y, temp.z += right.z;
		return temp;
	}

	EVertex4D EVertex4D::operator /(EFloat factor)
	{
		EVertex4D temp = *this;
		temp.x /= factor, temp.y /= factor, temp.z /= factor;
		return temp;
	}

	

	EVertex4D& EVertex4D::operator =(const EVertex4D &right)
	{
		x	= right.x;	y	= right.y;	z	= right.z;	w = right.w;
		u	= right.u;	v	=right.v;
		return *this;
	}

	

	EVector4D EVertex4D::toVector4D() const
	{
		return EVector4D(x, y, z, w);
	}

	

	EPlane3D::EPlane3D(const EVector4D &p, const EVector4D &nor) : point(p)
	{
		normal = nor;
		normal.normalize();
	}

	

	EPlane3D& EPlane3D::operator=(const EPlane3D &right)
	{
		point	= right.point;
		normal	= right.normal;
		return *this;
	}

	

	EPolyon4D::EPolyon4D() : state(0), attribute(0), color(0xffffff), verList(NULL){}

	

	EPolyonF4D::EPolyonF4D() : state(0), attribute(0), color(0xffffff){}

	

	EObject4D::EObject4D() : nextObject(NULL), materiaName(DEFAULT_NAME), 
							scale(1, 1, 1),direction(EVector4D::UNIT_X), needCull(true)
							{}


	void Tranform_Object4D(	EObject4D* object, const EMatrix44 &mat, 
							OBJ_TRANSFORM_TYPE tansformType)
	{
		EObject4D* obj = object;
		switch (tansformType)
		{
		case TRANSFORM_LOCAL:
			{
				while (obj)
				{
					for (EInt i = 0; i < obj->vertexNumber; i++)
					{
						GetVertex4DMulMatrix44(obj->localList[i], mat, obj->localList[i]);
					}
					obj = obj->nextObject;
				}
			}
			break;
		case TRANSFORM_TRANS:
			{
				while (obj)
				{
					for (EInt i = 0; i < obj->vertexNumber; i++)
					{
						GetVertex4DMulMatrix44(obj->transformList[i], mat, obj->transformList[i]);
					}
					obj = obj->nextObject;
				}
			}
			break;
		case TRANSFORM_LOCAL_TO_TRANS:
			{
				while (obj)
				{
					for (EInt i = 0; i < obj->vertexNumber; i++)
					{
						GetVertex4DMulMatrix44(obj->localList[i], mat, obj->transformList[i]);
					}

					obj = obj->nextObject;
				}
			}
			break;
		}
	}

	void Transform_Renderlist4D(ERenderList4D* renderList, 
								const EMatrix44 &mat, OBJ_TRANSFORM_TYPE transformType)
	{
		EPolyonF4D *poly = NULL;
		switch(transformType)
		{
		case TRANSFORM_LOCAL:
			{
				for (ERenderList4D::Itr itr = renderList->polyData.begin();
					itr != renderList->polyData.end(); ++itr)
				{
					poly = &(*itr);

					GetVertex4DMulMatrix44(poly->localList[0], mat, poly->localList[0]);
					GetVertex4DMulMatrix44(poly->localList[1], mat, poly->localList[1]);
					GetVertex4DMulMatrix44(poly->localList[2], mat, poly->localList[2]);
				}
			}
			break;
		case TRANSFORM_TRANS:
			{
				for (ERenderList4D::Itr itr = renderList->polyData.begin();
					itr != renderList->polyData.end(); ++itr)
				{
					poly = &(*itr);
					GetVertex4DMulMatrix44(poly->transformList[0], mat, poly->transformList[0]);
					GetVertex4DMulMatrix44(poly->transformList[1], mat, poly->transformList[1]);
					GetVertex4DMulMatrix44(poly->transformList[2], mat, poly->transformList[2]);
				}
			}
			break;
		case TRANSFORM_LOCAL_TO_TRANS:
			{
				for (ERenderList4D::Itr itr = renderList->polyData.begin();
					itr != renderList->polyData.end(); ++itr)
				{
					poly = &(*itr);
					GetVertex4DMulMatrix44(poly->localList[0], mat, poly->transformList[0]);
					GetVertex4DMulMatrix44(poly->localList[1], mat, poly->transformList[1]);
					GetVertex4DMulMatrix44(poly->localList[2], mat, poly->transformList[2]);
				}
			}
			break;
		}
	}


	void Inert_Object4D_To_RenderList4D(ERenderList4D *renderList, EObject4D *object, 
					OBJ_TRANSFORM_TYPE transformType, bool transformScaleAndPosition)
	{
		if (!object)
			return;

		EVector4D scale(1,1,1);
		EVector4D pos(0,0,0);

		if (transformScaleAndPosition)
		{
			scale = object->scale;
			pos   = object->worldPosition;
		}

		ELight *light = NULL;
		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr)
		{
			light = *itr;
			light->transPosition = light->position;
			light->transDirection = light->direction;
		}

		EObject4D *obj = object;
		EPolyon4D *base;
		EPolyonF4D poly;
		switch(transformType)
		{
		case TRANSFORM_LOCAL:
			while (obj)
			{
				for (EInt i = 0; i < obj->polyonNumber; i++)
				{
					base = &obj->polyonList[i];

					poly.attribute		= base->attribute;
					poly.state			= EPOLY_STATE_ACTIVE;
					poly.color	= base->color;
					poly.localList[0]	= obj->localList[base->verIndex[0]];
					poly.localList[1]	= obj->localList[base->verIndex[1]];
					poly.localList[2]	= obj->localList[base->verIndex[2]];

					poly.material		= GetMaterial(obj->materiaName);

					renderList->polyData.push_back(poly);
				}

				obj = obj->nextObject;
			}
			break;
		case TRANSFORM_LOCAL_TO_TRANS:
			while (obj)
			{
				for (EInt i = 0; i < obj->polyonNumber; i++)
				{
					base = &obj->polyonList[i];

					poly.attribute			= base->attribute;
					poly.state				= EPOLY_STATE_ACTIVE;
					poly.color		= base->color;

					poly.transformList[0]	= obj->localList[base->verIndex[0]] * scale + pos;
					poly.transformList[1]	= obj->localList[base->verIndex[1]] * scale + pos;
					poly.transformList[2]	= obj->localList[base->verIndex[2]] * scale + pos;

					poly.material		= GetMaterial(obj->materiaName);

					renderList->polyData.push_back(poly);
				}

				obj = obj->nextObject;
			}
			break;
		case TRANSFORM_TRANS:
			while (obj)
			{
				for (EInt i = 0; i < obj->polyonNumber; i++)
				{
					base = &obj->polyonList[i];

					poly.attribute			= base->attribute;
					poly.state				= EPOLY_STATE_ACTIVE;
					poly.color		= base->color;

					poly.transformList[0]	= obj->transformList[base->verIndex[0]] * scale + pos;
					poly.transformList[1]	= obj->transformList[base->verIndex[1]] * scale + pos;
					poly.transformList[2]	= obj->transformList[base->verIndex[2]] * scale + pos;

					poly.material		= GetMaterial(obj->materiaName);

					renderList->polyData.push_back(poly);
				}

				obj = obj->nextObject;
			}
			break;
		}


	}

	void Local_To_World_Object4D(EObject4D* object, OBJ_TRANSFORM_TYPE transformType)
	{
		if (!object)
			return;

		EObject4D* obj = object;
		EVector4D pos  = obj->worldPosition;
		if (TRANSFORM_LOCAL_TO_TRANS == transformType)
		{
			while(obj)
			{
				for (EInt i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DAddVector4D(obj->localList[i] * obj->scale , pos, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}

		}
		else if (TRANSFORM_TRANS == transformType)
		{
			while(obj)
			{
				for (EInt i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DAddVector4D(obj->transformList[i], pos, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
		}
	}


	void Local_To_World_RenderList4D( ERenderList4D *renderList,
										const EVector4D &pos, OBJ_TRANSFORM_TYPE transformType)
	{
		EPolyonF4D *poly = NULL;
		switch(transformType)
		{
		case TRANSFORM_LOCAL:
			{
				for(ERenderList4D::Itr itr = renderList->polyData.begin();
					itr != renderList->polyData.end(); ++itr)
				{
					poly = &(*itr);

					GetVertex4DAddVector4D(poly->localList[0], pos, poly->localList[0]);
					GetVertex4DAddVector4D(poly->localList[1], pos, poly->localList[1]);
					GetVertex4DAddVector4D(poly->localList[2], pos, poly->localList[2]);
				}
			}
			break;
		case TRANSFORM_LOCAL_TO_TRANS:
			{
				for(ERenderList4D::Itr itr = renderList->polyData.begin();
					itr != renderList->polyData.end(); ++itr)
				{
					poly = &(*itr);
					GetVertex4DAddVector4D(poly->localList[0], pos, poly->transformList[0]);
					GetVertex4DAddVector4D(poly->localList[1], pos, poly->transformList[1]);
					GetVertex4DAddVector4D(poly->localList[2], pos, poly->transformList[2]);
				}
			}
			break;
		case TRANSFORM_TRANS:
			{
				for(ERenderList4D::Itr itr = renderList->polyData.begin();
					itr != renderList->polyData.end(); ++itr)
				{
					poly = &(*itr);
					GetVertex4DAddVector4D(poly->transformList[0], pos, poly->transformList[0]);
					GetVertex4DAddVector4D(poly->transformList[1], pos, poly->transformList[1]);
					GetVertex4DAddVector4D(poly->transformList[2], pos, poly->transformList[2]);
				}
			}
			break;
		}
	}

	bool Cull_Object4D(EObject4D *object, EFrustum *camera, CULL_TYPE cullType)
	{
		if (!object)
			return true;

		if (!object->needCull)
			return false;

		EVector4D centerPos;
		GetVector4DMulMatrix44(object->worldPosition, camera->mWorldToCamera, centerPos);
		EFloat scaleRadiusZ;
		EFloat scaleRadiusX;
		EFloat scaleRadiusY;

		if (cullType == CULL_SPHERE)
		{
			scaleRadiusZ= object->maxRadius * object->scale.z;
			scaleRadiusX = object->maxRadius * object->scale.x;
			scaleRadiusY = object->maxRadius * object->scale.y;
		}
		else if(cullType == CULL_BOX)
		{
			scaleRadiusZ = object->maxBoundingBox.z * object->scale.z;
			scaleRadiusX = object->maxBoundingBox.x * object->scale.x;
			scaleRadiusY = object->maxBoundingBox.y * object->scale.y;
		}
		else
			return true;

		if ((centerPos.z + scaleRadiusZ) < camera->clip_z_near || 
			(centerPos.z - scaleRadiusZ) > camera->clip_z_far)
			return true;

		EFloat x_test = 0.5f * camera->viewplane_width * centerPos.z / camera->view_dist;
		if ((centerPos.x - scaleRadiusX) > x_test || 
			(centerPos.x + scaleRadiusX) < -x_test)
			return true;

		EFloat y_test = 0.5f * camera->viewplane_height * centerPos.z / camera->view_dist;
		if ((centerPos.y - scaleRadiusY) > y_test ||
			(centerPos.y + scaleRadiusY) < -y_test)
			return true;

		return false;
	}


	void World_To_Camera_Object4D(EObject4D *object, EFrustum *camera)
	{
		EObject4D *obj = object;
		while (obj)
		{
			for (EInt i = 0; i < obj->vertexNumber; i++)
			{
				GetVertex4DMulMatrix44(obj->transformList[i], camera->mWorldToCamera, obj->transformList[i]);
			}
			obj = obj->nextObject;
		}
	}


	void World_To_Camera_RenderList4D(ERenderList4D *renderList, EFrustum *camera)
	{
		EMatrix44 mat = camera->mWorldToCamera;

		mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0;

		ELight *light = NULL;
		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr)
		{
			light = *itr;
			GetVector4DMulMatrix44(light->position, mat,light->transPosition);
			GetVector4DMulMatrix44(light->direction, mat,light->transDirection);
		}

		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr itr = renderList->polyData.begin();
				itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);


			GetVertex4DMulMatrix44(poly->transformList[0], camera->mWorldToCamera, poly->transformList[0]);
			GetVertex4DMulMatrix44(poly->transformList[1], camera->mWorldToCamera, poly->transformList[1]);
			GetVertex4DMulMatrix44(poly->transformList[2], camera->mWorldToCamera, poly->transformList[2]);
		}
	}

	

	void Sort_RenderList4D(ERenderList4D *renderList)
	{
		renderList->polyData.sort(CompEPolyonF4D());
	
	}


	bool Remove_Backface(EPolyon4D *poly, EFrustum *camera)
	{
	

		EInt index0 = poly->verIndex[0];
		EInt index1 = poly->verIndex[1];
		EInt index2 = poly->verIndex[2];
		EVector4D u = (poly->verList->at(index0) - poly->verList->at(index1)).toVector4D();
		EVector4D v = (poly->verList->at(index0) - poly->verList->at(index2)).toVector4D();

		EVector4D n = u.crossProduct(v);
		EVector4D view = camera->position - poly->verList->at(index0).toVector4D();

		EFloat dot = n.dotProduct(view);
		if (dot < 0.0f)
		{
			poly->state |= EPOLY_STATE_BACKFACE;
			return true;
		}

		return false;
	}

	

	bool Remove_Backface(EPolyonF4D *poly, EFrustum *camera)
	{
		if (!(poly->state & EPOLY_STATE_ACTIVE) ||
			(poly->state & EPOLY_STATE_BACKFACE) ||
			(poly->state & EPOLY_STATE_CLIPPED))
			return true;
		EVector4D u = (poly->transformList[0] - poly->transformList[1]).toVector4D();
		EVector4D v = (poly->transformList[0] - poly->transformList[2]).toVector4D();

		EVector4D n = u.crossProduct(v);
		EVector4D view = camera->position - poly->transformList[0].toVector4D();

		EFloat dot = n.dotProduct(view);
		if (dot < 0)
		{
			poly->state |= EPOLY_STATE_BACKFACE;
			return true;
		}

		return false;
	}

	

	void Remove_Backface_Object4D(EObject4D *object, EFrustum *camera)
	{
		EObject4D *obj = object;
		EPolyon4D *poly = NULL;
		while(obj)
		{
			if (!(obj->state & EOBJECT_STATE_CULLED))
			{
				for (EInt i = 0; i < obj->polyonNumber; i++)
				{
					poly = &obj->polyonList[i];
					Remove_Backface(poly, camera);
					
				}
			}
				
			obj = obj->nextObject;
		}
	}

	

	void Remove_Backface_RenderList4D(ERenderList4D *renderList, EFrustum *camera)
	{
		EPolyonF4D *poly = NULL;

		for (ERenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end();)
		{
			poly = &(*itr);

			if(Remove_Backface(poly, camera))
				renderList->polyData.erase(itr++);
			else
			{
				++itr;
			}
			
		}
	}
	
	void Light_PolyonF4D(EPolyonF4D *poly, EFrustum *camera)
	{
		

		EBool lightOn = false;
		EColor baseColor(poly->color);
		EInt sumColorR = 0 ,sumColorG = 0, sumColorB = 0;	

		EVector4D u = (poly->transformList[0] - poly->transformList[1]).toVector4D();
		EVector4D v = (poly->transformList[0] - poly->transformList[2]).toVector4D();
		EVector4D n = u.crossProduct(v);

		EFloat length = n.length();

		ELight *light = NULL;
		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr)
		{
			light = *itr;
			if (!light->lightOn)
				continue;

			lightOn = true;
			if (light->lightType == LIGHT_AMBIENT)
			{
				sumColorR += light->ambient.r * baseColor.r >> 8;
				sumColorG += light->ambient.g * baseColor.g >> 8;
				sumColorB += light->ambient.b * baseColor.b >> 8;
			}
			else if (light->lightType == LIGHT_DIRECTION)
			{
				
				EFloat dot = n.dotProduct(-light->transDirection);
				if (dot > 0.0f)
				{
					EFloat dirLeng = light->transDirection.length();
					
					EFloat temp = light->power * dot / (length * dirLeng * 256);
					sumColorR += temp * light->diffuse.r * baseColor.r;
					sumColorG += temp * light->diffuse.g * baseColor.g;
					sumColorB += temp * light->diffuse.b * baseColor.b;
				}
				else
				{
					
	
					sumColorR += baseColor.r * light->shadowFactor;
					sumColorG += baseColor.g * light->shadowFactor;
					sumColorB += baseColor.b * light->shadowFactor;
				}
			}
			else if (light->lightType == LIGHT_POINT)
			{
				EVector4D dir = light->transPosition - poly->transformList[0].toVector4D();
				EFloat dot = n.dotProduct(dir);

				if (dot > 0.0f)
				{
					
					EFloat dist = dir.length();
					EFloat atten = light->kc + light->kl * dist + light->kq * dist * dist;
					EFloat temp = dot / (length * dist * atten * 256);

					sumColorR += light->diffuse.r * baseColor.r * temp;
					sumColorG += light->diffuse.g * baseColor.g * temp;
					sumColorB += light->diffuse.b * baseColor.b * temp;
				}
				else
				{
					
					
					sumColorR += baseColor.r * light->shadowFactor;
					sumColorG += baseColor.g * light->shadowFactor;
					sumColorB += baseColor.b * light->shadowFactor;
				}
			}
		
		}

		if (lightOn)
		{
			if (sumColorR > 255) sumColorR = 255;
			if (sumColorG > 255) sumColorG = 255;
			if (sumColorB > 255) sumColorB = 255;

			poly->color = ECOLOR_16BIT(sumColorR, sumColorG, sumColorB);
		}
	}

	void Light_RenderList4D(ERenderList4D *renderList, EFrustum *camera)
	{
		EPolyonF4D *poly = NULL;

		for (ERenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);
			Light_PolyonF4D(poly, camera);
		}
	}

	

	void Camera_To_Perspective_Object4D(EObject4D *object, EFrustum *camera)
	{
		if (!object)
			return;

		EObject4D *obj = object;
		EFloat d = camera->view_dist;
		EFloat temp = 0;

		while (obj)
		{
			for (EInt i = 0; i < obj->vertexNumber; i++)
			{
				temp = d / obj->transformList[i].z;
				
				obj->transformList[i].x = temp * obj->transformList[i].x;
				obj->transformList[i].y = camera->aspect_ratio * temp * obj->transformList[i].y;
			}
			obj = obj->nextObject;
		}
	}


	void Camera_To_Perspective_RenderList4D(ERenderList4D *renderList, EFrustum *camera)
	{
		EFloat d = camera->view_dist;
		EFloat temp = 0;
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr itr = renderList->polyData.begin();
				itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);
			

			for (EInt i = 0; i < 3; i++)
			{
				temp = d / poly->transformList[i].z;
				poly->transformList[i].x = temp * poly->transformList[i].x;
				poly->transformList[i].y = camera->aspect_ratio * temp * poly->transformList[i].y;
			}
		}
	}

	
	void Perspective_To_Screen_Object4D(EObject4D *object, EFrustum *camera)
	{
		if (!object)
			return;

		EFloat XOffset = 0.5f * (camera->viewport_width - 1);
		EFloat YOffset = 0.5f * (camera->viewport_height - 1);
		
		EObject4D *obj = object;
		while (obj)
		{
			for (EInt i = 0; i < obj->vertexNumber; i++)
			{
				obj->transformList[i].x = XOffset *( 1 + obj->transformList[i].x);
				obj->transformList[i].y = YOffset *( 1 - obj->transformList[i].y);
			}
			obj = obj->nextObject;
		}
	}


	void Perspective_To_Screen_RenderList4D(ERenderList4D *renderList, EFrustum *camera)
	{

		EFloat YOffset = 0.5f * (camera->viewport_height - 1);

		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);


			for (EInt i = 0; i < 3; i++)
			{
				poly->transformList[i].y = YOffset *(1 - poly->transformList[i].y);
			}
		}
	}

	

	EBool isClipped(EPolyonF4D *poly, const EPlane3D &plane)
	{
		EInt flag = 0;
		for (EInt i = 0; i < 3; i++)
		{
			EFloat temp = plane.normal.dotProduct(poly->transformList[i].toVector4D() - plane.point);
			if (temp < 0)
				flag++;
		}

		if (flag == 3)
			return true;
		else
			return false;
	}

	
	EBool isClipped(EPolyonF4D *point, EFloat znear, EFloat zfar)
	{
		EInt num = 0;

		for (int i = 0; i < 3; i++)
		{
			if (point->transformList[i].z < znear || point->transformList[i].z > zfar)
				num++;
		}

		if (num == 3)
			return true;
		return false;
	}

	
	EBool isClipped(EPolyonF4D *poly, EFrustum *camera)
	{
		EVertex4D v0, v1, v2;
		GetVertex4DMulMatrix44(poly->transformList[0], camera->mWorldToCamera, v0);
		GetVertex4DMulMatrix44(poly->transformList[1], camera->mWorldToCamera, v1);
		GetVertex4DMulMatrix44(poly->transformList[2], camera->mWorldToCamera, v2);

		if (camera->clip_z_near > v0.z && camera->clip_z_near > v1.z && camera->clip_z_near > v2.z ||		
			camera->clip_z_far < v0.z && camera->clip_z_far < v1.z && camera->clip_z_far < v2.z ||			
			camera->clip_plane_L.normal.dotProduct(v0.toVector4D() - camera->clip_plane_L.point) < 0 &&		
			camera->clip_plane_L.normal.dotProduct(v1.toVector4D() - camera->clip_plane_L.point) < 0 &&
			camera->clip_plane_L.normal.dotProduct(v2.toVector4D() - camera->clip_plane_L.point) < 0 ||

			camera->clip_plane_R.normal.dotProduct(v0.toVector4D() - camera->clip_plane_R.point) < 0 &&		
			camera->clip_plane_R.normal.dotProduct(v1.toVector4D() - camera->clip_plane_R.point) < 0 &&
			camera->clip_plane_R.normal.dotProduct(v2.toVector4D() - camera->clip_plane_R.point) < 0 ||

			camera->clip_plane_T.normal.dotProduct(v0.toVector4D() - camera->clip_plane_T.point) < 0 &&		
			camera->clip_plane_T.normal.dotProduct(v1.toVector4D() - camera->clip_plane_T.point) < 0 &&
			camera->clip_plane_T.normal.dotProduct(v2.toVector4D() - camera->clip_plane_T.point) < 0 ||

			camera->clip_plane_B.normal.dotProduct(v0.toVector4D() - camera->clip_plane_B.point) < 0 &&		
			camera->clip_plane_B.normal.dotProduct(v1.toVector4D() - camera->clip_plane_B.point) < 0 &&
			camera->clip_plane_B.normal.dotProduct(v2.toVector4D() - camera->clip_plane_B.point) < 0)
			return true;

		return false;
	}


	void Clip_RenderList4D(ERenderList4D *renderList, EFrustum *camera)
	{
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end();)
		{
			poly = &(*itr);
			if (isClipped(poly, camera->clip_plane_L) || 
				isClipped(poly, camera->clip_plane_R) ||
				isClipped(poly, camera->clip_plane_B) ||
				isClipped(poly, camera->clip_plane_T))
			{
				renderList->polyData.erase(itr++);
				continue;
			}

			++itr;
		}
	}



	void World_To_Screen_RenderList4D(ERenderList4D *renderList, EFrustum *camera)
	{
		EFloat XOffset = 0.5f * (camera->viewport_width - 1);
		EFloat YOffset = 0.5f * (camera->viewport_height - 1);
		EFloat dist = camera->view_dist;
		EFloat temp = 0;

	
		EMatrix44 mat = camera->mWorldToCamera;
		
		mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0.0f; mat.m[3][3] = 1.0f;

		ELight *light = NULL;
		for (LightItr itr = GLights->begin(); itr != GLights->end(); ++itr)
		{
			light = *itr;
			GetVector4DMulMatrix44(light->position, mat,light->transPosition);
			GetVector4DMulMatrix44(light->direction, mat,light->transDirection);
		}

		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end();)
		{
			poly = &(*itr);

			if (Remove_Backface(poly, camera))
			{
				renderList->polyData.erase(itr++);
				continue;
			}

			GetVertex4DMulMatrix44(poly->transformList[0], camera->mWorldToCamera, poly->transformList[0]);
			GetVertex4DMulMatrix44(poly->transformList[1], camera->mWorldToCamera, poly->transformList[1]);
			GetVertex4DMulMatrix44(poly->transformList[2], camera->mWorldToCamera, poly->transformList[2]);

			if(	isClipped(poly, camera->clip_z_near, camera->clip_z_far) ||
				isClipped(poly, camera->clip_plane_L) ||
				isClipped(poly, camera->clip_plane_R) ||
				isClipped(poly, camera->clip_plane_T) ||
				isClipped(poly, camera->clip_plane_B))
			{
				renderList->polyData.erase(itr++);
				continue;
			}

			Light_PolyonF4D(poly, camera);

			for (EInt i = 0; i < 3; i++)
			{

				temp = dist / poly->transformList[i].z;
				poly->transformList[i].x = temp * poly->transformList[i].x;
				poly->transformList[i].y = camera->aspect_ratio * temp * poly->transformList[i].y;

				poly->transformList[i].x = XOffset *(1 + poly->transformList[i].x);
				poly->transformList[i].y = YOffset *(1 - poly->transformList[i].y);
			}

			++itr;
		}
		Sort_RenderList4D(renderList);
	}

	

	void Draw_Object4D_Wire(EObject4D *object)
	{
		if (!object)
			return;

		EObject4D *obj = object;
		while (obj)
		{
			for (EInt i = 0; i < obj->polyonNumber; i++)
			{
				if ((obj->polyonList[i].state & EPOLY_STATE_ACTIVE) && 
					!(obj->polyonList[i].state & EPOLY_STATE_CLIPPED) &&
					!(obj->polyonList[i].state & EPOLY_STATE_BACKFACE))
				{
					EInt index0 = obj->polyonList[i].verIndex[0];
					EInt index1 = obj->polyonList[i].verIndex[1];
					EInt index2 = obj->polyonList[i].verIndex[2];

					EGraphics::drawLine(obj->transformList[index0].x, obj->transformList[index0].y, 
							 obj->transformList[index1].x, obj->transformList[index1].y, 
							 EColor(obj->polyonList[i].color));
					EGraphics::drawLine(obj->transformList[index1].x, obj->transformList[index1].y, 
							 obj->transformList[index2].x, obj->transformList[index2].y,
							 EColor(obj->polyonList[i].color));
					EGraphics::drawLine(obj->transformList[index0].x, obj->transformList[index0].y, 
							 obj->transformList[index2].x, obj->transformList[index2].y,
							 EColor(obj->polyonList[i].color));
				}

				obj->polyonList[i].state = EPOLY_STATE_ACTIVE;
			}

			obj->state = EOBJECT_STATE_ACTIVE;
			obj = obj->nextObject;
		}
	}

	

	void Draw_RenderList4D_Wire(ERenderList4D *renderList)
	{
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);

			{
				EGraphics::drawLine(poly->transformList[0].x,poly->transformList[0].y,
						 poly->transformList[1].x,poly->transformList[1].y, EColor(poly->color));
				EGraphics::drawLine(poly->transformList[1].x,poly->transformList[1].y,
						 poly->transformList[2].x,poly->transformList[2].y, EColor(poly->color));
				EGraphics::drawLine(poly->transformList[0].x,poly->transformList[0].y,
						 poly->transformList[2].x,poly->transformList[2].y, EColor(poly->color));
			}
			poly->state = EPOLY_STATE_ACTIVE;
		}

		renderList->polyData.clear();
	}

	void Draw_Object4D_Solid(EObject4D *object)
	{
		if (!object)
			return;

		EObject4D *obj = object;
		while (obj)
		{
			for (EInt i = 0; i < obj->polyonNumber; i++)
			{
				if ((obj->polyonList[i].state & EPOLY_STATE_ACTIVE) && 
					!(obj->polyonList[i].state & EPOLY_STATE_CLIPPED) &&
					!(obj->polyonList[i].state & EPOLY_STATE_BACKFACE))
				{
					EInt index0 = obj->polyonList[i].verIndex[0];
					EInt index1 = obj->polyonList[i].verIndex[1];
					EInt index2 = obj->polyonList[i].verIndex[2];

					EGraphics::fillTriangle(obj->transformList[index0].x, obj->transformList[index0].y, 
								 obj->transformList[index1].x, obj->transformList[index1].y,
								 obj->transformList[index2].x, obj->transformList[index2].y, 
								 EColor(obj->polyonList[i].color));
				}
				obj->polyonList[i].state = EPOLY_STATE_ACTIVE;
			}

			obj->state = EOBJECT_STATE_ACTIVE;
			obj = obj->nextObject;
		}
	}

	

	void Draw_RenderList4D_Solid(ERenderList4D *renderList)
	{
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);

			{
				EGraphics::fillTriangle(poly->transformList[0].x,poly->transformList[0].y,
							 poly->transformList[1].x,poly->transformList[1].y,
							 poly->transformList[2].x,poly->transformList[2].y,
							 EColor(poly->color));
			}

			poly->state = EPOLY_STATE_ACTIVE;
		}

		renderList->polyData.clear();
	}

	
	void Draw_Top_Trangle(	EFloat x0, EFloat y0, EFloat z0,
							EFloat u0, EFloat v0,
							EFloat x1, EFloat y1, EFloat z1,
							EFloat u1, EFloat v1,
							EFloat x2, EFloat y2, EFloat z2,
							EFloat u2, EFloat v2,
							EMaterial *mat, const EColor &color)
	{
		if (y2 < 0 || y0 > SCREEN_HEIGHT - 1)
			return;

		EFloat dxdl = (x0 - x1) / (y0 - y1);
		EFloat dxdr = (x0 - x2) / (y0 - y2);

		EFloat dzdl = (z0 - z1) / (y0 - y1);
		EFloat dzdr = (z0 - z2) / (y0 - y2);

		EFloat startL = x0;
		EFloat startR = x0;

		EFloat begX = 0;
		EFloat endX = 0;

		if (!mat || !mat->useTexture())
		{
			if (y0 < 0)
				y0 = 0;
			if (y2 > SCREEN_HEIGHT - 1)
				y2 = SCREEN_HEIGHT - 1;
			
			
			for (EFloat y = y0; y < y2; y++)
			{
				
				begX = startL;
				endX = startR;
				if (endX < begX) std::swap(begX, endX);

				if (begX < 0)
					begX = 0;
				if (endX > SCREEN_WIDTH - 1)
					endX = SCREEN_WIDTH - 1;

				for (EFloat x = begX; x < endX; x++)
				{
					if(EGraphics::checkZ(x, y, z0))
						EGraphics::setPixel(x, y, /*z0, */color);
				}

				startL += dxdl;
				startR += dxdr;
			}

			return;
		}

		EFloat dudl = (u0 - u1) / (y0 - y1);
		EFloat dvdl = (v0 - v1) / (y0 - y1);
		EFloat dudr = (u0 - u2) / (y0 - y2);
		EFloat dvdr = (v0 - v2) / (y0 - y2);

		EFloat startLU = u0;
		EFloat startLV = v0;
		EFloat startRU = u0;
		EFloat startRV = v0;

		EFloat begU = 0;
		EFloat endU = 0;
		EFloat begV = 0;
		EFloat endV = 0;

		EFloat dx = 0;
		EFloat ui = 0;
		EFloat vi = 0;

		EFloat zl = 0;
		EFloat zr = 0;
		EFloat z  = 0;

		for (EFloat y = y0; y < y2; y++)
		{
	
			begX = (EInt)startL;
			endX = (EInt)startR;
			begU = startLU;
			endU = startRU;
			begV = startLV;
			endV = startRV;

			zl = z0 + (y - y0) * dzdl;
			zr = z0 + (y - y0) * dzdr;

			if (endX < begX) 
			{
				std::swap(begX, endX);
				std::swap(begU, endU);
				std::swap(begV, endV);

				std::swap(zl, zr);
			}

			if (begX < 0)
			{
				begU = begU - begX * (startLU - startRU) / (startL - startR);
				begX = 0;
			}
			if (endX > SCREEN_WIDTH - 1)
			{
				endU = endU - (endX - SCREEN_WIDTH) * (startLU - startRU) / (startL - startR);
				endX = SCREEN_WIDTH - 1;
			}

			dx = endX - begX;
			ui = (endU - begU) / dx;
			vi = (endV - begV) / dx;

			z = zl;
			EFloat zd = (zl - zr) / (begX - endX);
			for (EFloat x = begX; x <= endX; x++)
			{
				if (y > -1 && y < SCREEN_HEIGHT && EGraphics::checkZ(x, y, z))
				{
					const EColor tex = mat->getPixel(begU, begV);
					EGraphics::setPixel(x, y,/* z, */color * tex);
				}

				begU += ui;
				begV += vi;
				z	 += zd;
			}

			startL += dxdl;
			startR += dxdr;

			startLU += dudl;
			startLV += dvdl;
			startRU += dudr;
			startRV += dvdr;
		}
	}

	
	void Draw_Botton_Trangle(	EFloat x0, EFloat y0, EFloat z0,
								EFloat u0, EFloat v0,
								EFloat x1, EFloat y1, EFloat z1,
								EFloat u1, EFloat v1,
								EFloat x2, EFloat y2, EFloat z2,
								EFloat u2, EFloat v2,
								EMaterial *mat, const EColor &color)
	{
		if (y2 < 0 || y0 > SCREEN_HEIGHT - 1)
			return;

		EFloat dxdl = (x1 - x2) / (y1 - y2);
		EFloat dxdr = (x0 - x2) / (y0 - y2);

		EFloat dzdl = (z1 - z2) / (y1 - y2);
		EFloat dzdr = (z0 - z2) / (y0 - y2);

		EFloat startL = x1;
		EFloat startR = x0;

		EFloat begX = 0;
		EFloat endX = 0;

		if (!mat || !mat->useTexture())
		{
			if (y0 < 0)
				y0 = 0;
			if (y2 > SCREEN_HEIGHT - 1)
				y2 = SCREEN_HEIGHT - 1;

			for (EFloat y = y0; y < y2; y++)
			{
	
				begX = startL;
				endX = startR;
				if (endX < begX) std::swap(begX, endX);

				if (begX < 0)
					begX = 0;
				if (endX > SCREEN_WIDTH - 1)
					endX = SCREEN_WIDTH - 1;

				for (EFloat x = begX; x < endX; x++)
				{
					if (EGraphics::checkZ(x, y, z0))
						EGraphics::setPixel(x, y,/* z0, */color);
				}
				
				startL += dxdl;
				startR += dxdr;
			}

			return;
		}

		EFloat dudl = (u1 - u2) / (y1 - y2);
		EFloat dvdl = (v1 - v2) / (y1 - y2);
		EFloat dudr = (u0 - u2) / (y0 - y2);
		EFloat dvdr = (v0 - v2) / (y0 - y2);

		EFloat startLU = u1;
		EFloat startLV = v1;
		EFloat startRU = u0;
		EFloat startRV = v0;

		EFloat begU = 0;
		EFloat endU = 0;
		EFloat begV = 0;
		EFloat endV = 0;

		EFloat dx = 0;
		EFloat ui = 0;
		EFloat vi = 0;

		EFloat zl = 0;
		EFloat zr = 0;
		EFloat z  = 0;
		for (EFloat y = y0; y < y2; y++)
		{

			begX = (EInt)startL;
			endX = (EInt)startR;
			begU = startLU;
			endU = startRU;
			begV = startLV;
			endV = startRV;

		
			zl = z1 + (y - y0) * dzdl;
			zr = z0 + (y - y0) * dzdr;

			if (endX < begX) 
			{
				std::swap(begX, endX);
				std::swap(begU, endU);
				std::swap(begV, endV);

				
				std::swap(zl, zr);
			}

			if (begX < 0)
			{
				begU = begU - begX * (startLU - startRU) / (startL - startR);
				begX = 0;
			}
			if (endX > SCREEN_WIDTH - 1)
			{
				endU = endU - (endX - SCREEN_WIDTH) * (startLU - startRU) / (startL - startR);
				endX = SCREEN_WIDTH - 1;
			}

			dx = endX - begX;
			ui = (endU - begU) / dx;
			vi = (endV - begV) / dx;

			z = zl;
			EFloat zd = (zl - zr) / (begX - endX);
			for (EFloat x = begX; x <= endX; x++)
			{
				if (y > -1 && y < SCREEN_HEIGHT && EGraphics::checkZ(x, y, z))
				{
					const EColor tex = mat->getPixel(begU, begV);
					EGraphics::setPixel(x, y,/* z, */color * tex);
				}

				begU += ui;
				begV += vi;
				z	 += zd;
			}

			startL += dxdl;
			startR += dxdr;

			startLU += dudl;
			startLV += dvdl;
			startRU += dudr;
			startRV += dvdr;
		}
	}

	void Draw_Trangle(EPolyonF4D *poly)
	{
		EFloat x0 = poly->transformList[0].x, y0 = poly->transformList[0].y, z0 = poly->transformList[0].z;
		EFloat u0 = poly->transformList[0].u, v0 = poly->transformList[0].v;
		EFloat x1 = poly->transformList[1].x, y1 = poly->transformList[1].y, z1 = poly->transformList[1].z;
		EFloat u1 = poly->transformList[1].u, v1 = poly->transformList[1].v;
		EFloat x2 = poly->transformList[2].x, y2 = poly->transformList[2].y, z2 = poly->transformList[2].z;
		EFloat u2 = poly->transformList[2].u, v2 = poly->transformList[2].v;

		if (y1 < y0)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
			std::swap(z0, z1);
			std::swap(u0, u1);
			std::swap(v0, v1);
		}

		if (y2 < y0)
		{
			std::swap(x0, x2);
			std::swap(y0, y2);
			std::swap(z0, z2);
			std::swap(u0, u2);
			std::swap(v0, v2);
		}

		if (y2 < y1)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
			std::swap(z1, z2);
			std::swap(u1, u2);
			std::swap(v1, v2);
		}

		EFloat factor = (y1- y0) / (y2 - y0);
		EFloat newX = x0 + factor * (x2 - x0);
		EFloat newZ = z0 + factor * (z2 - z0);
		EFloat newU = u0 + factor * (u2 - u0);

		EFloat newV = v0 + factor * (v2 - v0);
		Draw_Top_Trangle(x0, y0, z0, u0, v0, x1, y1, z1, u1, v1, newX, y1, newZ, newU, newV, poly->material, EColor(poly->color));
		Draw_Botton_Trangle(newX, y1, newZ, newU, newV, x1, y1, z1, u1, v1, x2, y2, z2, u2, v2, poly->material, EColor(poly->color));
		//}
	}
	void Draw_RenderList4D_Teture_Solid(ERenderList4D * renderList)
	{
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr itr = renderList->polyData.begin();
			itr != renderList->polyData.end(); ++itr)
		{
			poly = &(*itr);

			Draw_Trangle(poly);

			poly->state = EPOLY_STATE_ACTIVE;
		}

		renderList->polyData.clear();
	}
}