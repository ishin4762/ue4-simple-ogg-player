# ue4-simple-ogg-player

## Overview
This plugin plays ogg with intro in Unreal Engine 4 and Unreal Engine 5.

## Specification

* Smooth loop with intro by specifying loop start point and loop length
* As of v0.10.x, uasset files are not yet supported. Specify file path directly
* As of v0.10.x, looping by ogg built-in tags is not available

## Supported Environment

* UE4.25.x - UE4.27.x
* UE5.0.x - UE5.2.x

## Usage

1. Put Plugins/SimpleOggPlayer in the Plugins folder of your project
1. Put the .ogg file anywhere in the Content folder of your project
1. Create a Sound Cue and add a node named OggPlayer as follows
![](images/image1.png)
1. Enter parameters
    * Ogg File Path is the relative path of the ogg file from the Content folder
    * Loop Start is the number of samples at the start of the loop.
    * Loop Length is the number of samples to loop.
1. Play Cue to play ogg with loop

## License
MIT

## Update History
* 2023/08/23 Expanded supported environment to UE5.2
* 2022/04/29 Added UE4.29 and UE5.0 to supported environment
* 2022/01/08 v0.10.2 Fixed to be able to reference USoundNodeOggPlayer from C++
* 2021/02/07 v0.10.1 Fixed a bug that allowed playback even if the file path was changed to an incorrect file path.
* 2021/01/29 v0.10 released
