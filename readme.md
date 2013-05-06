﻿zCraft
======

A C++ generative game engine.
Copyright 2010-2013 (c) Marc Gilleron

The project is in an early stage of development, so many things can change
on every new commit. I try to ensure that each version between pushes can
be used without too much bugs or quirks.

Note : the generative part is basically a polygon-based voxel engine (zcraft/ package), 
but it may change in the future.
It is built on top of an engine (engine/ package) that can be used for any game.

![Screenshot](http://zylannprods.fr/games/zcraft/screenshots/2013-02-19-crafted-zcraft.png)

Current main features
=====================

Voxel engine :

- Simple map generators using 2D or 3D perlin noise
- World save format based on N*N*N block files, where N=16 (not efficient, but working)
- Built-in 2D top-down world cartographer
- 3D free-flight viewer supporting viewing ranges up to 13*16 cubes without
	dropping under 60fps (on a 2009 laptop, didn't tested over that limit).

Game engine :

- Some essentials (Vector2, Vector3, Rect, AABB, Matrix4...)
- Dependence-free generic GUI (windows, buttons, wrapped text, sliders, dragging, skins...)
- OpenGL utility (Vertex/Pixel/Geometry shaders, basic direct-draws, VBO...)
- BMFont implementation
- Template entity-based game structure 
- And other things (Platform-specific things, math utilities, logging...)

Third-party :

- UTF8 conversion for text internationalization
- Simplex noise

Some planned features
================

- Frustum culling
- OpenGL 3.x (easier switching, at least)
- AABB physics
- Faster voxel archive save format
- Voxel sprites
- And many little things in the code : see TODO comments
- mapgen optimization

Downloads
=========

Nothing "playable" at the moment, but you can test the demos.

Infinite scary cave-canyon test (Windows only) :
https://dl.dropbox.com/u/60408088/INFO/zCraft/2012-12-29-zcraft-map-test.zip

Compiling
=========

A few days ago, I switched to CMake. However, I'm new to this build system,
so the CMakeLists.txt may be a bit awkward.
It is recommended to use a separate build/ directory for the building process
(notice it in .gitignore) in which you specify your environnment (paths to libs).

Compiler requirements :
- C++11, compiles fine with GCC 4.7.2 or MinGW on Windows.
(MSVC compatibility is not my priority until I really need it)

Dependencies :
- SFML 2.0 (GCC 4.7 DW2)
- OpenGL 2.1 (I should switch to 3 one day)
- Glu
- Glew (I recompiled it for MinGW)

Execute
========

The execution directory must be the same as the assets/ folder (i.e repo's root).

Folder structure
================

The project is divided in several main parts :

- zENG, namespace zn::, folder engine/ (also called zCraft Engine)

This is a set of generic tools I wrote for making games,
and may be used for other projects than zCraft.
It is heavily based on SFML, but some parts are independent.

- zENGui, namespace zn::ui::, folder engine/gui/

This is a generic gui that only depends on the core/ and utf8/ parts of the engine.
It is currently developped as part of the project.
All you have to do to use it is to provide your renderer and input.
Note : some renderers and inputs are already programmed.

- zCraft, namespace zcraft::, folder zcraft/

This is the voxel engine, based on zENG.

- Experimental, namespace experimental::

experimental undocumented stuff based on the other packages.
its contents may be future features or tests. I added this to the repo to have a
cloud copy of it (I don't trust my HD) and to ease sharing.

