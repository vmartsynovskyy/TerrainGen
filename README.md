# TerrainGen

A procedural terrain generator for Unreal Engine 4.

[//]: # (TODO: Put a video here)

[//]: # (TODO: include a description of how it works)

## Features

### Current

* Infinite terrain generation by only spawning visible terrain
* Support for editing of terrain parameters through Unreal Engine's editor
* Support for different resolutions of terrain

### Future

* Support for lower LOD chunks further from player
* Better parallelization of mesh generation (different thread/task for each chunk, not just one seperate thread for all chunks being generated)
* Don't keep vertex info for already spawned chunks that are in the render radius in memory
* Use something better than a simple curve to add some interesting features to the terrain. Potentially investigate techniques from [this paper](https://pdfs.semanticscholar.org/5961/c577478f21707dad53905362e0ec4e6ec644.pdf)
* Support for different textures/colors based on terrain height

### Very Future

* GPU accelerated heightmap -> vertices, normals, and triangles conversion

## Acknowledgements

* [Unreal Engine](https://www.unrealengine.com)
* [FastNoise](https://github.com/Auburns/FastNoise)