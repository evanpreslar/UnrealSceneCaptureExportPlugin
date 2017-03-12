UnrealSceneCaptureExportPlugin
==============================

This project was extends the Scene Capture functionality present within Unreal Engine v4.14.

The Project includes a C++ Plugin exposing two Blueprint callable functions, as well as a sample .uproject demonstrating both.

The .uproject itself contains some necessary blueprints and settings which are required to make full use of the plugin, meaning simply dropping the central blueprint into another project will not necessarily work. The most important changes are as follows:

* Gamemode (Sample provided): Default Pawn and PlayerController set to actors without visible meshes (unless desired)
* Engine: General Settings: Project is set to use Fixed Frame Rate (necessary for any level sequence / matinee automation)
* Blueprint (Sample Provided): Helpful when rendering multiple frames in succession by providing plugin function with some information regarding frame number.

To utilize the Plugin, it must first be built from provided source, after which it will appear under the plugin listing within the editor (after a restart) provided it is located within the Plugins/ directory within the project.

SceneCaptureCube Export Pipeline
================================

The first of these functions (ExportRenderTargetCube) provides a means of exporting SceneCaptureCubes similar to the method already present for SceneCapture2D within UKismetRenderingLibrary::ExportTexture2D.
The CubeRenderTargets are natively output as .hdr images in an equirectangular projection.

SceneCapture2D Modular 360 Degree Stereoscopic Exporter
=======================================================

The second function (CaptureAndExport2DCaptureSet) is based around manipulating a single SceneCapture2DComponent in order to export a specific number of images around a central point. To do this, many RenderTarget2D objects are created on the fly as the CaptureComponent is rotated. As expected, this consumes a large amount of RAM, which is only compounded by the amount of captures requested per frame as well as the resolutionof the created render targets.

The behaviour of the function is highly configurable through parameters, of which the most critical are the number of vertical and horizontal images to be captured. For instance, if 4 longitudinal and 3 latitudinal segments are specified, the function will export 12 images, stopping to capture at each of the 4 cardinal directions, taking 3 images (down, straight ahead, and up) at each. If more segments are requested, the rotations become smaller, and the images will overlap more. To avoid capturing redundant information when a large number of segments are requested, the FOVAngle and Resolution parameters can be adjusted, allowing images to be more tightly focused, just as with any normal SceneCapture2D. Finally, the function will ouptut two stereoscopic images, one for each eye, unless the EyeDistance parameter is set to 0.0. Debug output is sent to the editor's console detailing if any parameters are invalid.

In addition to this base functionality provided by the plugin, this project also includes a blueprint which assists with rendering multiple frames in succession. This blueprint allows for capturing a specific number of frames (after which the simulation will exit), skipping a specific number of frames (only rendering the frames after this point), and scheduling intermittent garbage collection. Because this blueprint inherits directly from SceneCapture2D, it can be used in a similar fashion; most importantly, it can be entirely automated via a level sequence. Provided that the project being rendered is set to utilize a fixed frame rate (otherwise all movement will occur independent of framerate), this blueprint can be used to render a sequence of images which can be ultimately used to create 360 degree stereoscopic video.

It should also be noted that in more complex projects, the first frame may be captured before all resources are initialized (textures, etc), and will thus produce undesireable output. For this reason, the sample capture blueprint defaults to skip the very first frame.

