# X-Ray Tracer

## What is this?
A simple renderer to take x-rays instead of showing the surface of objects.

This is a very basic implementation, just enough code to get the job done, of [ray tracing](https://en.wikipedia.org/wiki/Ray_tracing_(graphics)).
But with a twist: what if, instead of bouncing rays over the objects, we let them go all the way trough?

We can produce an x-ray picture.

![Image mimiking the "Homer Simpson small brain" meme](https://github.com/stefanos-86/X-RayTracer/blob/main/sample_result.png)
[...if you don't know the meme](https://duckduckgo.com/?t=ffab&q=Homer+Simpson+brain+xray&iax=images&ia=images)

## *Why* is this?
Because it is easier that creating a real ray tracing renderer, but the "meat" of the algorithm is
 still the same.

I wanted to create a ray tracing renderer for practice, for a long time.
But I don't have too much time to spend to play around with code.

[Suddenly](https://en.wikipedia.org/wiki/Serendipity) I remembered
 that I alredy had an implemetation of the key ray-mesh collision algorithm in [an old project](https://github.com/stefanos-86/CatapultGame).

* I can handle any mesh already. *
 
 I used [text based 3D graphics](https://en.wikipedia.org/wiki/Wavefront_.obj_file) before, [Blender](https://www.blender.org/) can export it.
 And I know there is a [text-based graphic format](https://en.wikipedia.org/wiki/Netpbm#File_formats) I came across many years ago. It is extremey simple and [GIMP](https://www.gimp.org/) can open it.

 * I can do computer graphics using just * `cin` * and * `cout`.
 
 Finally, I realized that faking x-rays allows to cut a lot of big corners: no colors or complex materials, no reflections. If I don't mind a slow run time, no fancy optimizations either.

 * All the parts slotted into place. *

 ## Technicalities
 This project has no dependencies but the C++ standard library.

 It should be possible to build it out of the box with [CMake](https://cmake.org): `cmake . && make`.

It is also very rough, and not intended for any real use.
The rendering parameters are hardcoded right in the main function (...did I mention that I don't have time to play around, yet?).

The code structure mirrors the high-level idea of ray tracing, simulating the light movement
in a scene. XRayMachine class is the entry point, and does just that: it emits the rays, check
their collisions with one or more Meshes, marks the color on a spot of the Film.

To compute the x-ray attenuation trough the material, just multiply the thickness for its
attenuation power. It is not physically accurate, but the images look "good enough".

That is all that there is to it.

## How to make the models.
To save even more programming time, I cheated on how to create the objects to scan in the x-ray machine. 
- Each mesh can have only one material, with any name.
- The .mtl file associated to the .obj file is completely ignored. The shielding strenght of the material is hardcoded in the Mesh class. You will have to add more values in the `materialsLib` map.
- The format has to report the material, the points, the faces. No edges. See the [Blender export settings](https://github.com/stefanos-86/X-RayTracer/blob/main/BlenderExportSettings.png).
- To make a multi-material complex object, just export a different .obj file for every part that has that material. This is the reason why the XRayMachine class accepts multiple objects.
- The meshes need not be closed, but be careful with holes. The code expects the ray to hit the surface of the object first, even if it has internal holes. If you "cut" the mesh and the ray enters from the "fake" hole, it meets an interal surface first. The code thinks that the ray just hit the object, but instead it entered already - there was no representation of the surface at the point of entry.
- There are few parameters to position the emitter and the projection plane. You will have to model the object in a matching position to "compose" the scene and be sure it is between the emitter and the film.

Don't forget that you need to draw the volume of the model.
The usual meshes done in 3D graphics are for surfaces. But if the object has an internal
structure, you have to model that too. The sample head has a big hole inside, otherwise it would
block the rays completely, as a dense block of material would. The thickness
of the elements is important.


