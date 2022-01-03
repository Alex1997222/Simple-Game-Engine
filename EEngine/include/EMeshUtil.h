
#pragma once

#include "ECommon.h"
#include "EOgreMeshInfo.h"
#include "ECore.h"
#include <fstream>

namespace E3D
{
	
	// read string from stream
	// size indicates the number of strings to be read, if size==-1, then read a line of strings
	EString ReadString(std::ifstream &in, EInt size = -1);

	
	// Used when reading Ogre mesh files
	// Read the next identifier from the stream, corresponding to OGREMESHID
	EUShort ReadChunk(std::ifstream &in);

	// Read a bool type value from the stream
	EBool ReadBool(std::ifstream &in);

	// Read an unsined int type value from the stream
	EUInt ReadInt(std::ifstream &in);

	// Read an unsigned short value from strean
	EUShort ReadShort(std::ifstream& in);

	// Read a set of data of type T from the stream
	// size indicates the size of the array
	template<typename T>
	std::vector<T> ReadArray(std::ifstream &in, EInt size)
	{
		std::vector<T> v(size, 0);
		in.read((char*)(&v[0]),sizeof(T) * size);
		return v;
	}

	EObject4D* LoadOgreMesh(const EString& meshName, const EVector4D &worldPos = EVector4D::ZERO, 
							const EVector4D &scale = EVector4D(1, 1, 1));
}