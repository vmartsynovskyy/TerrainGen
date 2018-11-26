#include "ChunkInfo.h"

void ChunkInfo::GenerateChunk(int xStart, int yStart, ChunkGenParams params) {
	int heightMapLength = 4096 / params.Resolution + 1;
	float vertexDistance = params.Size / (heightMapLength - 1);

	noiseGen = params.PtrToNoise;

	TArray<TArray<float>> heightmap;
	heightmap = generateHeightmap(xStart-1, yStart-1, params.Resolution, heightMapLength, params.TerrainCurve, params.HeightScale);
	vertices = generateVertices(heightmap, vertexDistance);
	triangles = generateTriangles(heightMapLength, heightMapLength);
	normals = generateNormals(vertices, heightmap, heightMapLength, heightMapLength, vertexDistance);
	uvMap = generateUV(heightMapLength, heightMapLength);
}

TArray<FVector> ChunkInfo::GetVertices() {
	return vertices;
}

TArray<int32> ChunkInfo::GetTriangles() {
	return triangles;
}

TArray<FVector> ChunkInfo::GetNormals() {
	return normals;
}

TArray<FVector2D> ChunkInfo::GetUVMap() {
	return uvMap;
}

TArray<FLinearColor> ChunkInfo::GetColors() {
	return colors;
}

TArray<FProcMeshTangent> ChunkInfo::GetTangents() {
	return tangents;
}


int ChunkInfo::GetDistanceTraversed() {
	return heightmapDistanceTraversed;
}

/**
*   \brief Generates a heightmap
*
*   Generates a heightMap with the given resolution factor and length, starting at (xStart, yStart)
*
*   Generates two more points in each direction than actually needed so that we have correct normals and correctly sewn together chunks.
*
*	\param curve is a function that can modify the heights (for example you can apply e^x to the whole heightMap to make the higher values even higher)
*
*   \return The heightmap for the chunk at (xStart, yStart)
*
**/
TArray<TArray<float>> ChunkInfo::generateHeightmap(int xStart, int yStart, int resolutionFactor, int heightMapLength, UCurveFloat* curve, float heightScale) {
	TArray<TArray<float>> xArray;

	int lastHmap = 0;
	for (int x = xStart; x < xStart + (heightMapLength + 2) * resolutionFactor; x += resolutionFactor) {
		TArray<float> yArray;
		for (int y = yStart; y < yStart + (heightMapLength + 2) * resolutionFactor; y += resolutionFactor) {
			lastHmap = x;
			yArray.Add(curve->GetFloatValue(noiseGen->GetNoise(x, y))*heightScale);
		}
		xArray.Add(yArray);
	}

	heightmapDistanceTraversed = lastHmap - xStart - 2*resolutionFactor;
	return xArray;
}

/**
*   \brief Generates vertices given a heightmap.
*
*   Given a rectangular heightmap, generates a flattened array
*   of vectors representation. Flattened top-down then left-right.
*
*   \param heightmap A rectangular array representing a heightmap
*   \param vertexDistance The distance between each vertex
*   \return A flat array of vertices generated from the heightmap
**/
TArray<FVector> ChunkInfo::generateVertices(TArray<TArray<float>> heightmap, float vertexDistance) {
	// TArray<TArray<float>> heightmap must be a rectangle
	TArray<FVector> vertices;
	for (int x = 1; x < heightmap.Num() - 1; x++) {
		for (int y = 1; y < heightmap[0].Num() - 1; y++) {
			vertices.Add(FVector((x - 1)*vertexDistance, (y - 1)*vertexDistance, heightmap[x][y]));
		}
	}
	return vertices;
}

/**
*   \brief Generates triangles given the dimensions of a rectangular heightmap.
*
*   Generates triangles given the dimensions of a rectangular heightmap,
*   assuming a flattened list of vectors generated from heightmap top-down then left-right.
*
*   \param width The width of the rectangular heightmap.
*   \param height The height of the rectangular heightmap.
*   \return A list of int32 representing the triangles generated from the rectangular heightmap.
*
**/
TArray<int32> ChunkInfo::generateTriangles(int width, int height) {
	// TArray<TArray<float>> heightmap must be a rectangle
	TArray<int32> triangles;
	for (int x = 0; x < width - 1; x++) {
		for (int y = 0; y < height - 1; y++) {
			int topLeft = x * height + y;
			int bottomLeft = x * height + y + 1;
			int bottomRight = (x + 1) * height + y + 1;
			int topRight = (x + 1) * height + y;

			// Bottom-left triangle
			triangles.Add(topLeft);
			triangles.Add(bottomLeft);
			triangles.Add(bottomRight);

			// Top-right triangle
			triangles.Add(topLeft);
			triangles.Add(bottomRight);
			triangles.Add(topRight);
		}
	}
	return triangles;
}

/**
*   \brief Generates normals used for shading.
*
*   \param verts A rectangular array representing a heightmap
*   \param hMap A rectangular array representing a heightmap
*   \param width The width of the heightMap
*   \param height The height of the heightMap
*   \param vertexDistance The distance between each vertex
*   \return A flat array of normals for each vertex
**/
TArray<FVector> ChunkInfo::generateNormals(TArray<FVector> verts, TArray<TArray<float>> hMap, int width, int height, float vertexDistance) {
	/// \todo{ Get height and width from the heightmap instead of passing it }
	TArray<FVector> normals;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++) {
			// indices for each vertex around the current one
			// based on triangle ordering above
			int centerInd = x * height + y;
			int leftInd = centerInd - height;
			int rightInd = centerInd + height;
			int topRightInd = rightInd - 1;
			int bottomLeftInd = leftInd + 1;
			int topInd = centerInd - 1;
			int bottomInd = centerInd + 1;

			FVector topLeftCross;
			FVector topRightUpperCross;
			FVector topRightLowerCross;
			FVector bottomRightCross;
			FVector bottomLeftLowerCross;
			FVector bottomLeftUpperCross;

			// we must check if the index is within bounds otherwise, to have seamless edges, we must generate new vertices from the heightmap
			// if it's in bounds then we can just use the vertices we already generated
			FVector centerVert = verts[centerInd];
			FVector leftVert = (x - 1 > 0 && x - 1 < width) ? verts[leftInd] : FVector((x - 1)*vertexDistance, (y)*vertexDistance, hMap[x][y + 1]);
			FVector rightVert = (x + 1 > 0 && x + 1 < width) ? verts[rightInd] : FVector((x + 1)*vertexDistance, (y)*vertexDistance, hMap[x + 2][y + 1]);
			FVector topRightVert = (x + 1 > 0 && x + 1 < width && y - 1 > 0 && y - 1 < height) ? verts[topRightInd] : FVector((x + 1)*vertexDistance, (y - 1)*vertexDistance, hMap[x + 2][y]);
			FVector bottomLeftVert = (x - 1 > 0 && x - 1 < width && y + 1 > 0 && y + 1 < height) ? verts[bottomLeftInd] : FVector((x - 1)*vertexDistance, (y + 1)*vertexDistance, hMap[x][y + 2]);
			FVector topVert = (y - 1 > 0 && y - 1 < height) ? verts[topInd] : FVector((x)*vertexDistance, (y - 1)*vertexDistance, hMap[x + 1][y]);
			FVector bottomVert = (y + 1 > 0 && y + 1 < height) ? verts[bottomInd] : FVector((x)*vertexDistance, (y + 1)*vertexDistance, hMap[x + 1][y + 2]);


			// we calculate the normals of each side, sum them, and normalize to get normal vectors for a vertex
			topLeftCross = FVector::CrossProduct(topVert - centerVert, leftVert - centerVert);
			bottomLeftLowerCross = FVector::CrossProduct(centerVert - bottomVert, bottomVert - bottomLeftVert);
			bottomLeftUpperCross = FVector::CrossProduct(leftVert - bottomLeftVert, centerVert - leftVert);
			topRightUpperCross = FVector::CrossProduct(topVert - centerVert, topRightVert - centerVert);
			topRightLowerCross = FVector::CrossProduct(topRightVert - rightVert, rightVert - centerVert);
			bottomRightCross = FVector::CrossProduct(centerVert - bottomVert, rightVert - centerVert);

			FVector normal = topLeftCross + topRightUpperCross + topRightLowerCross + bottomRightCross + bottomLeftLowerCross + bottomLeftUpperCross;
			normal.Normalize();
			normals.Add(normal);
		}
	}

	return normals;
}

/**
*   \brief Generates the UV map used for mapping textures to vertices
*
*   \param width The width of the vertices of the chunk
*   \param height The height of the vertices of the chunk
*   \return The UV array for the chunk
**/
TArray<FVector2D> ChunkInfo::generateUV(int width, int height) {
	TArray<FVector2D> uv;
	for (int32 x = 0; x < width; x++)
	{
		for (int32 y = 0; y < height; y++) {
			FVector2D uvVert = FVector2D(x, y);
			uv.Add(uvVert);
		}
	}

	return uv;
}
