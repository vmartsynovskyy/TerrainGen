#include "ChunkInfoWorker.h"

uint32 ChunkInfoWorker::Run() {

	while (true) {
		float renderRadiusSquare = powf(GenerateRadius, 2);

		// spawn chunks
		int xStart = (playerPos.X - GenerateRadius) / ChunkSize;
		int xEnd = (playerPos.X + GenerateRadius) / ChunkSize;
		int yStart = (playerPos.Y - GenerateRadius) / ChunkSize;
		int yEnd = (playerPos.Y + GenerateRadius) / ChunkSize;
		// iterate through all chunks in (playerLocation.X - RenderRadius, playerLocation.X + RenderRadius)
		for (int x = xStart; x < xEnd; x++) {
			// iterate through all chunks in (playerLocation.Y - RenderRadius, playerLocation.Y + RenderRadius)
			for (int y = yStart; y < yEnd; y++) {
				// check that the chunk is within radius
				float distanceFromPlayerSquare = powf((x*ChunkSize - playerPos.X), 2) + powf((y*ChunkSize - playerPos.Y), 2);
				if (distanceFromPlayerSquare < renderRadiusSquare) {
					createChunkInfo(x, y);
				}
			}
		}
	}
	return 0;
}

void ChunkInfoWorker::createChunkInfo(int x, int y) {
	// can put these all on different threads too if needed
	TPair<int, int> chunkPos(x, y);
	if (!(infoMapPtr->Contains(chunkPos))) {
		ChunkInfo chunk;
		chunk.GenerateChunk(x*(heightMapLen - 1), y*(heightMapLen - 1), Resolution, ChunkSize, PtrToNoise, Curve, HeightScale);
		heightMapLen = chunk.GetDistanceTraversed();
		infoMapPtr->Add(chunkPos, chunk);
	}
}
