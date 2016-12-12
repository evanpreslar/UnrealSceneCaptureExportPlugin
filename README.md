UnrealSceneCaptureExportPlugin
==============================

This project was created to extend the scene capture functionality added in the Unreal Engine v4.13.

Specifically, it defines a plugin which exposes a Blueprint callable node that allows for the exporting of scene capture cube renders into .HDR image files. To function, it must be provided with a valid TextureRenderTargetCube reference along with a file path and file name. A sample level is provided in this repository to demonstrate the application of the plugin.

The source of the plugin resembles the implementation of the UKismetRenderingLibrary::ExportTexture2D function already present in the engine, although adapted to support the exporting of Scene Capture Cubes.