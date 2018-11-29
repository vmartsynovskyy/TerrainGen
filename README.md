# TerrainGen

A procedural terrain generator for Unreal Engine 4.

![Short gif of TerrainGen](https://media.giphy.com/media/2uIcDbw2Lib8WEmZtW/giphy.gif)

(This gif is in terrible quality because I had to compress it down to 5 MB)

[//]: # (TODO: include a description of how it works)

## Features

- [x] Infinite terrain generation by only spawning visible terrain
- [x] Support for editing of terrain parameters through Unreal Engine's editor
- [x] Support for different resolutions of terrain
- [ ] Support for lower LOD chunks further from player
- [ ] Better parallelization of mesh generation (different thread/task for each chunk, not just one seperate thread for all chunks being generated)
- [ ] Don't keep vertex info for already spawned chunks that are in the render radius in memory
- [ ] Use something better than a simple curve to add some interesting features to the terrain. Potentially investigate techniques from [this paper](https://pdfs.semanticscholar.org/5961/c577478f21707dad53905362e0ec4e6ec644.pdf)
- [ ] Support for different textures/colors based on terrain height
- [ ] Place object such as bushes, grass, and trees using [Poisson disk sampling](https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf)
- [ ] (in the far future) GPU accelerated heightmap -> vertices, normals, and triangles conversion

## Acknowledgements

* [Unreal Engine](https://www.unrealengine.com)
* [FastNoise](https://github.com/Auburns/FastNoise)
* [Sebastion Lague on YouTube](https://www.youtube.com/channel/UCmtyQOKKmrMVaKuRXz02jbQ) compared my implementation to his and got idea for features from here
