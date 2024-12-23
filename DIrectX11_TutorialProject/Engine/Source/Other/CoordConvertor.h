#pragma once

#include <iostream>
#include <fstream>

using namespace std;

typedef struct
{
	float x, y, z;
}VertexType;

typedef struct
{
	int v1, v2, v3;
	int t1, t2, t3;
	int n1, n2, n3;
}FaceType;

void GetModelFilename(char*);
bool ReadFileCounts(char*, int&, int&, int&, int&);
bool LoadDataStructures(char*, int, int, int, int);

bool Execute()
{
	bool result;
	char fileName[256];
	int vertexCount, textureCount, normalCount, faceCount;

	GetModelFilename(fileName);

	result = ReadFileCounts(fileName, vertexCount, textureCount, normalCount, faceCount);
	if (!result) return false;

	result = LoadDataStructures(fileName, vertexCount, textureCount, normalCount, faceCount);
	if (!result) return false;

	return true;
}


void GetModelFilename(char* fileName)
{
	ifstream fin;

	fileName = "Engine/Model/cube.obj";

	fin.open(fileName);

	if (!fin.good()) fin.clear();
}

bool ReadFileCounts(char* fileName,
	int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;

	fileName = "Engine/Model/cube.obj";

	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	fin.open(fileName);

	if (fin.fail()) return false;

	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { ++vertexCount; }
			if (input == 't') { ++textureCount; }
			if (input == 'n') { ++normalCount; }
		}

		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { ++faceCount; }
		}

		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	return true;
}

bool LoadDataStructures(char* fileName, int vertextCount, int textureCount, int normalCount, int faceCount)
{
	VertexType* vertices = 0, * texcoords = 0, * normals = 0;
	FaceType* faces;
	ifstream fin;
	int vertexIndex = 0, texcoordIndex = 0, normalIndex = 0, faceIndex = 0, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	fileName = "Engine/Model/cube.obj";

	vertices = new VertexType[vertextCount];
	if (!vertices) return false;

	texcoords = new VertexType[textureCount];
	if (!texcoords) return false;

	normals = new VertexType[normalCount];
	if (!normals) return false;

	faces = new FaceType[faceCount];
	if (!faces) return false;

	fin.open(fileName);
	if (fin.fail()) return false;

	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				++vertexIndex;
			}

			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				++texcoordIndex;
			}

			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				++normalIndex;
			}
		}

		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				fin >> faces[faceIndex].v3 >> input2 >> faces[faceIndex].t3 >> input2 >> faces[faceIndex].n3
					>> faces[faceIndex].v2 >> input2 >> faces[faceIndex].t2 >> input2 >> faces[faceIndex].n2
					>> faces[faceIndex].v1 >> input2 >> faces[faceIndex].t1 >> input2 >> faces[faceIndex].n1;
				++faceIndex;
			}
		}

		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	fout.open("model.txt");

	fout << "Vertex Count: " << (faceCount * 3) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;
	for (int i = 0; i < faceIndex; ++i)
	{
		vIndex = faces[i].v1 - 1;
		tIndex = faces[i].t1 - 1;
		nIndex = faces[i].n1 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].v2 - 1;
		tIndex = faces[i].t2 - 1;
		nIndex = faces[i].n2 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].v3 - 1;
		tIndex = faces[i].t3 - 1;
		nIndex = faces[i].n3 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

	}
	
	fout.close();
	
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}
	if (normals)
	{
		delete[] normals;
		normals = 0;
	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}
	
	return true;
}