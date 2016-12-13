UnrealSceneCaptureExportPlugin
==============================

This project was created to extend the scene capture functionality added in the Unreal Engine v4.13.

Specifically, it defines a plugin which exposes a Blueprint callable node that allows for the exporting of scene capture cube renders into .HDR image files. To function, it must be provided with a valid TextureRenderTargetCube reference along with a file path and file name. A sample level is provided in this repository to demonstrate the application of the plugin.

For the plugin to function, it must be located in the Plugins/ folder inside of a Unreal Project or added directly to the Engine's Plugins/ folder. Once the plugin has been installed, the static 'Export Render Target Cube' function should become available within Blueprint. It is recommended that the function be executed sparingly, as Scene Capture Cubes are expensive to render, even at low resolutions. For this reason, the sample Blueprint executes the function only once on level start, though it is possible to execute it every tick. The quality of the resultant .HDR image is tied to the Render Target Cube object rather than the Scene Capture object itself, and in the sample Blueprint, is set to produce a 4000x2000 image.

The source of the plugin resembles the implementation of the UKismetRenderingLibrary::ExportTexture2D function already present in the engine, although adapted to support the exporting of Scene Capture Cubes.