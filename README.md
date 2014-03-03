cocos3d-x
=========
cocos3d-x is  a multi-platform game framework for building 3d games,which is based cocos2d-x.


  
## Supported Platforms
- [Windows]
- [iOS]
- [Android]

Main features
-------------
   * Math : vector,matrix,quaternion
   * Mesh
     * general mesh
     * morph mesh
     * batch mesh
   * Material
   * Model
     * skinmodel
     * skinlessmodel
     * batchmodel
   * Texture
     * 2D texture
     * 3D texture
   * FileSystem
     * filestream
     * memorystream
   * ResourceManager
     * resource
     * resourepool
     * resourcemanager
   * Scene items
     * camera
     * light
     * dynamic sprite
     * static sprite
   * ParticleSystem
     * emitter
     * render
     * affect
   * PostEffect
   * Geo ：box,plane,ray...
   * shader lib
     * texture : texture only
     * diffuse : texture + diffuse light
     * specular : texture + diffuse light + specular light
     * bumpdiffuse : texture + diffuse light + bump
     * bumpspecular : texture + diffuse light + specular light + bump
     * ......



How to start a new game
-----------------------

1. Download the code from cocos3d download site or https://github.com/cocos2d/cocos3d-x
2. Enter `tools/project-creator`
3. Run the `create_project.py` script

Example:

    $ cd cocos3d-x/tools/project-creator
    $ python ./create_project.py -project helloworld -package com.company.helloworld
    $ cd /projects/helloworld

### Build new project for android ###

    Requirements : Windows 7+ , android sdk 2.3+ , android ndk r8b+
    
    Build : 
    $ cd cocos3d-x/projects/helloworld
    $ cd proj.android
    $ sh ./build_native.sh
    
    Run:
    Import helloworld Android project using Eclipse(released with Android SDK). The path to be imported is `cocos3d-x/projects/helloworld/proj.android`.
    

### Build new project for ios
    Requirements : Mac OS X 10.7+, Xcode 4.6+

    Build :
    * Enter *proj.ios* folder
    * open *helloworld.xcodeproj*
    * Select ios targets in scheme toolbar , build
    
    Run :
    * Enter *proj.ios* folder
    * open *helloworld.xcodeproj*
    * Select ios targets in scheme toolbar , run


### Build new project for win32 ###
    Requirements : Windows 7+, VS 2010+
    
    Build :
    * enter *cocos3d* folder
    * open the `cocos3d-win32.vc2010.sln`
    * build
    
    Run :
    * Enter *cocos3d* folder
    * open the `cocos3d-win32.vc2010.sln` by vs2010
    * select the *helloworld*
    * run



Running Tests
--------------------






Contact us
----------

   * Forum: 
   * Twitter: 
   * Weibo:
   * QQ:
  
