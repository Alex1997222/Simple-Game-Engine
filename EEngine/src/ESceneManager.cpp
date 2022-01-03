
#include "ESceneManager.h"
#include "EGraphics.h"
#include "EMaterial.h"
#include "EUtil.h"

namespace E3D
{
	

	ESceneManager::ESceneManager() : mLightEnable(true), mVisibleObjectNumber(0), 
		mVisiblePolyonNumber(0), mTotalPolyonNumber(0), mTerrainMesh(NULL)
	{
		mCamera		= new ECamera();
		mRenderList = new ERenderList4D;
	}

	

	ESceneManager::~ESceneManager()
	{
		for(MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); ++iter)
			SafeDelete(iter->second);
		for(MeshIter iter = mUsedMesh.begin(); iter != mUsedMesh.end(); ++iter)
			SafeDelete(iter->second);

		mMeshs.clear();
		DestoryAllLights();
		SafeDelete(mCamera);
		SafeDelete(mRenderList);
	}

	

	void ESceneManager::drawString(const EString &str, EInt x, EInt y, const EColor &c)
	{
		EGraphics::drawString(str, x, y, c);
	}

	

	EMesh* ESceneManager::createMesh(const EString &name, const EString &meshName)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter == mMeshs.end())
		{
			EMesh *mesh = NULL;

			MeshIter meshItr = mUsedMesh.find(meshName);
			if (meshItr != mUsedMesh.end())
			{
				mesh = meshItr->second->clone();
				mesh->mName = name;
			}
			else
			{
				mesh = new EMesh(name, meshName);
				mUsedMesh.insert(std::make_pair(meshName, mesh->clone()));
			}

			mMeshs.insert(std::make_pair(name, mesh));
			return mesh;
		}

		return iter->second;
	}

	void ESceneManager::clearMesh()
	{
		for(MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); ++iter)
			SafeDelete(iter->second);

		mMeshs.clear();
	}

	

	EMesh* ESceneManager::getMesh(const EString &name)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter == mMeshs.end())
		{
			return iter->second;
		}
		return NULL;
	}

	

	void ESceneManager::destroyMesh(const EString&name)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter != mMeshs.end())
			SafeDelete(iter->second);
		mMeshs.erase(name);
	}

	

	void ESceneManager::enableLights(EBool enable)
	{
		mLightEnable = enable;
		for (int i = 0; i < GetLightSize(); i++)
			getLight(i)->lightOn = enable;
	}

	

	ELight* ESceneManager::createLight(LIGHT_TYPE lightType)
	{
		EInt light = CreateLight(lightType);
		return getLight(light);
	}

	

	ELight*	ESceneManager::getLight(EInt id)
	{
		return GetLight(id);
	}

	

	void ESceneManager::update()
	{
		//camera->update();

		//Inert_Object4D_To_RenderList4D(renderList, object1->getObject(), TRANSFORM_TRANS, true);
		//Inert_Object4D_To_RenderList4D(renderList, object->getObject(), TRANSFORM_TRANS, true);

		//Local_To_World_RenderList4D(renderList, EVector4D::ZERO, TRANSFORM_TRANS);
		//World_To_Screen_RenderList4D(renderList, camera->getFrustum());

		mVisibleObjectNumber	= 0;
		mTotalPolyonNumber		= 0;

		mCamera->update();

		for(MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); ++iter)
		{
			mTotalPolyonNumber += iter->second->getPolyonNumber();

			if (iter->second->isVisible() && !Cull_Object4D(iter->second->getObject(), mCamera->getFrustum(), CULL_BOX))
			{
				mVisibleObjectNumber++;
				Inert_Object4D_To_RenderList4D(	mRenderList, iter->second->getObject(), 
					TRANSFORM_TRANS, true);
			}
		}

	
		Local_To_World_RenderList4D(mRenderList, EVector4D::ZERO, TRANSFORM_TRANS);
		
		World_To_Screen_RenderList4D(mRenderList, mCamera->getFrustum());

		mVisiblePolyonNumber = mRenderList->polyData.size();

		if (mCamera->getRenderMode() == RENDER_WIRE)
			Draw_RenderList4D_Wire(mRenderList);
		else
			Draw_RenderList4D_Teture_Solid(mRenderList);
	}

	


	EMesh* ESceneManager::createTerrain(const EString& heightMapFileName, 
		const EString &textureFileName, EFloat uTitle, EFloat vTitle, EFloat blockSize)
	{
		if (mTerrainMesh)
			SafeDelete(mTerrainMesh);

		Log("Loading Terrain HeightMap : #%s Texture : ...", heightMapFileName.c_str(), textureFileName.c_str());

		EBitmap *heigthMap		= new EBitmap(heightMapFileName);

		if (!heigthMap->isValid() || 
			heigthMap->getWidth() % 2 == 0 || 
			heigthMap->getHeight() % 2 == 0)
		{
			SafeDelete(heigthMap);
			return NULL;
		}

		EMaterial *mat			= new EMaterial();
		mat->bitmap				= new EBitmap(textureFileName);
		SetMaterial("Terrain", mat);

		EInt row				= heigthMap->getHeight();
		EInt col				= heigthMap->getWidth();

		// Y = 0.299R+0.587G+0.114B
		mTerrainMesh			= new EMesh();
		EObject4D *obj			= new EObject4D();
		mTerrainMesh->mObject	= obj;

		obj->name				= "Terrain";
		obj->materiaName		= "Terrain";
		obj->state				= EOBJECT_STATE_ACTIVE | EOBJECT_STATE_VISIBLE;
		obj->scale				= EVector4D(1,1,1);
		obj->worldPosition		= EVector4D::ZERO;
		obj->vertexNumber		= row * col;
		obj->polyonNumber		= (row - 1) * (col - 1) * 2;

		mTerrainMesh->mVertexNumber = obj->vertexNumber;
		mTerrainMesh->mPolyonNumber = obj->polyonNumber;

		obj->localList.reserve(obj->vertexNumber);
		obj->transformList.reserve(obj->vertexNumber);

		obj->polyonList.reserve(obj->polyonNumber);

		EFloat offsetX = (col - 1) * blockSize / 2;
		EFloat offsetZ = (row - 1) * blockSize / 2;

		EFloat minY = 0, maxY = 0;
		EFloat u = uTitle / (col - 1);
		EFloat v = vTitle / (row - 1);
		for (EInt r = 0; r < row; r++)
		{
			for (EInt c = 0; c < col; c++)
			{
				EVertex4D vex;
				EColor color = heigthMap->getPixel(c, r);
				vex.x	= c * blockSize - offsetX;
				vex.y	= 0.1f * (0.299f * color.r + 0.587f * color.g + 0.114f * color.b);
				vex.z	= r * blockSize - offsetZ;

				if (vex.y < minY)
					minY = vex.y;
				if (vex.y > maxY)
					maxY = vex.y;

				vex.u	= c * u;
				vex.v	= r * v;

				obj->localList.push_back(vex);
				obj->transformList.push_back(vex);

				if (r < row - 1 && c < col - 1)
				{
					EPolyon4D poly;
					poly.state		= EPOLY_STATE_ACTIVE;
					poly.attribute	= EPOLY_ATTR_VERTEX_POSITION | EPOLY_ATTR_VERTEX_UV;

					poly.verList	= &obj->transformList;

					poly.verIndex[0] = r * col + c;
					poly.verIndex[1] = (r + 1) * col + c;
					poly.verIndex[2] = r * col + c + 1;
					obj->polyonList.push_back(poly);

					poly.verIndex[0] = r * col + c + 1;
					poly.verIndex[1] = (r + 1) * col + c;
					poly.verIndex[2] = (r + 1) * col + c + 1;
					obj->polyonList.push_back(poly);
				}
			}
		}

		obj->maxBoundingBox		= EVector4D(offsetX, maxY, offsetZ);
		obj->minBoundingBox		= EVector4D(-offsetX, minY, -offsetZ);
		obj->maxRadius			= std::sqrt(offsetX * offsetX + offsetZ * offsetZ);

		SafeDelete(heigthMap);
		mMeshs.insert(std::make_pair("Terrain", mTerrainMesh));

		mTerrainMesh->setCullFlag(false);

		Log("Terrain Load Sucessed!");
		return mTerrainMesh;
	}

	
}