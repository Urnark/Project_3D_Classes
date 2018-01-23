#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <iostream>
#include <fstream>
#include <DirectXMath.h>
using namespace std;

class ModelLoader
{
private:
	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

	bool fileIsCompatible(string filename);
	bool ReadFileCounts(string, int&, int&, int&, int&);
	bool LoadDataStructures(string, int, int, int, int);

public:
	bool LoadModel(string filename);

};





#endif
