UnrealSceneCaptureExportPlugin
==============================

This project extends the Scene Capture functionality present within Unreal Engine v4.14.

It includes a C++ Plugin exposing two Blueprint callable functions, as well as a sample .uproject demonstrating both.

The .uproject itself contains some necessary blueprints and settings which are required to make full use of the plugin. Simply dropping the central blueprint into another project will not necessarily work, as it depends on some project settings. These are the most critical:

* Gamemode (Sample provided): Default Pawn and PlayerController set to actors without visible meshes (unless desired, such as a cutscene)
* Engine - General Settings: Project is set to use Fixed Frame Rate (necessary for any level sequence / matinee automation)
* Blueprint (Sample Provided): Helpful when rendering multiple frames in succession by providing the plugin function with some information regarding frame number.

To utilize the Plugin, it must first be built from provided source. This can be done by first opening the .uproject, and generating the necessary project files. From this point, one can open the .sln and build the plugin library directly, after which it will appear under the plugin listing within the editor (after a restart) provided it is still located within the Plugins/ directory within the project. Both of the included functions should appear under the "Rendering Plugin" header within Blueprint.

Modular 360 Degree Stereoscopic SceneCapture Exporter
=======================================================

The first and primary function (CaptureAndExport2DCaptureSet) is based around manipulating a single SceneCapture2DComponent in order to export multiple images around a central point. To do this, many RenderTarget2D objects are created on the fly as the CaptureComponent is rotated in a stepwise manner. As expected, this consumes a large amount of RAM, which is further compounded by the amount of captures requested per frame as well as the resolution of the created render targets.

The behaviour of the function is highly configurable through parameters, of which the most vital are the number of vertical and horizontal images to be captured. For example, if 4 longitudinal and 3 latitudinal segments are specified, the function will export 12 images, stopping to capture at each of the 4 cardinal directions, taking 3 images (down, straight ahead, and up) at each. If more segments are requested, the rotations become smaller, and the images will overlap more. To avoid capturing redundant information when a large number of segments are requested, the FOVAngle and Resolution parameters can be adjusted, allowing images to be more tightly focused, just as with any normal SceneCapture2D. The function also accepts an EyeDistance value which, when not equal to 0.0, will ouptut two stereoscopic images, one for each eye, offset by the provided distance. Any debug output that this function may generate is sent to the editor's console, detailing if any parameters are invalid.

In addition to this base functionality provided by the plugin, the project also includes a blueprint which assists with rendering multiple frames in succession. This blueprint allows for capturing a specific number of frames (after which the simulation will terminate), skipping a specific number of frames (only rendering the frames after this point), and scheduling intermittent garbage collection. 

Additionally, because this blueprint inherits directly from SceneCapture2D, it can be used in a similar fashion; most importantly, it can be entirely automated via a level sequence. Provided that the project being rendered is set to utilize a fixed frame rate (otherwise all movement will occur independent of framerate), this blueprint can be automatically trucked about a scene in order to render a sequence of images which can be ultimately be joined to create 360 degree stereoscopic video.

Note that the core function utilizes world coordinates to rotate, and will completely ignore any rotation applied to the parent actor, automated or otherwise.

It should also be noted that in more complex projects, all SceneCapture types (not just this one) may capture the first frame before all resources are initialized (textures, etc), and will thus produce undesireable output when ordered to export it (usually just a skybox and mesh geometry). Because SceneCaptures are primarily intended for real-time scene reflections within the Unreal Engine, this cannot be avoided unless the entire scene is made to load fully before exporting begins. For this reason, the sample capture blueprint defaults to skip the very first frame.

SceneCaptureCube Export Pipeline
================================

The second of these functions (ExportRenderTargetCube) provides a means of exporting SceneCaptureCubes similar to the method already present for SceneCapture2D within UKismetRenderingLibrary::ExportTexture2D.
The CubeRenderTargets are natively output as .hdr images in an equirectangular projection.

