#include "ChunkInfoWorker.h"

uint32 ChunkInfoWorker::Run() {
	while (KeepGenerating) {
		float renderRadiusSquare = powf(GenerateRadius, 2);

		// spawn chunks
		int xStart = (playerPos.X - GenerateRadius) / Params.Size;
		int xEnd = (playerPos.X + GenerateRadius) / Params.Size;
		int yStart = (playerPos.Y - GenerateRadius) / Params.Size;
		int yEnd = (playerPos.Y + GenerateRadius) / Params.Size;
		// iterate through all chunks in (playerLocation.X - RenderRadius, playerLocation.X + RenderRadius)
		for (int x = xStart; x < xEnd && KeepGenerating; x++) {
			// iterate through all chunks in (playerLocation.Y - RenderRadius, playerLocation.Y + RenderRadius)
			for (int y = yStart; y < yEnd && KeepGenerating; y++) {
				// check that the chunk is within radius
				float distanceFromPlayerSquare = powf((x*Params.Size - playerPos.X), 2) + powf((y*Params.Size - playerPos.Y), 2);
				if (distanceFromPlayerSquare < renderRadiusSquare) {
					createChunkInfo(x, y);
				}
			}
		}

		// cull chunkinfos
		TArray<TPair<int, int>> chunksToRemove;
		for (auto& Elem : *infoMapPtr) {
			// pythagoras
			float distanceFromPlayerSquare = powf((Elem.Key.Key*Params.Size - playerPos.X), 2) + powf((Elem.Key.Value*Params.Size - playerPos.Y), 2);
			// destroy chunk if its further than the render radius
			if ((distanceFromPlayerSquare + 10000.0f) > renderRadiusSquare) {
				chunksToRemove.Add(TPair<int, int>(Elem.Key.Key, Elem.Key.Value));
			}
		}

		{
			FScopeLock LockWhileRemoving(ChunkDeletion);
			for (auto chunk : chunksToRemove) {
				destroyChunkInfo(chunk.Key, chunk.Value);
			}
		}
	}
	return 0;
}

void ChunkInfoWorker::Stop() {
	KeepGenerating = false;
}

ChunkInfoWorker::ChunkInfoWorker() {
}

void ChunkInfoWorker::createChunkInfo(int x, int y) {
	// can put these all on different threads too if needed
	TPair<int, int> chunkPos(x, y);
	if (!(infoMapPtr->Contains(chunkPos))) {
		ChunkInfo chunk;
		chunk.GenerateChunk(x*(heightMapLen - 1), y*(heightMapLen - 1), Params);
		heightMapLen = chunk.GetDistanceTraversed();
		infoMapPtr->Add(chunkPos, chunk);
	}
}

void ChunkInfoWorker::destroyChunkInfo(int x, int y) {
	infoMapPtr->Remove(TPair<int, int>(x, y));
}
