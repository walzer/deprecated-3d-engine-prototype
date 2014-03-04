<img src="http://b277.photo.store.qq.com/psb?/V149io673qGalT/W6cXcz9sTIkPXptO4G5cIUM2r88IoXP8jIhqI4LpW7M!/b/dJ8QJKWVFQAA&bo=IAPgAQAAAAADAOc!" width=800 height = 480>
cocos3d-x
=========
cocos3d-x is a multi-platform game framework for building 3d games,which is based on the cocos2d-x engine.

  
### Supported Platforms
- [Windows]
- [iOS]
- [Android]

### Main features

   * Math : vector,matrix,quaternion
   * Mesh ：
     * general mesh
         supporting various vertex formats and 1 or more MeshPart to define how the vertices are connected.
     * morph mesh
         supporting morph deform
     * batch mesh
         rendering multiple mesh into a single draw call
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



### Create project
    * Download the code from cocos3d download site or https://github.com/cocos2d/cocos3d-x
    * Enter `tools/project-creator`
    * Run the `create_project.py` script

    Example:
       $ cd cocos3d-x/tools/project-creator
       $ python ./create_project.py -project helloworld -package com.company.helloworld
       $ cd /projects/helloworld
       
### Build and Run project
    for android

        Requirements : Windows 7+ , android sdk 2.3+ , android ndk r8b+
        
        Build : 
        $ cd cocos3d-x/projects/helloworld
        $ cd proj.android
        $ sh ./build_native.sh
        
        Run:
        * Import helloworld Android project using Eclipse(released with Android SDK). 
        * The path to be imported is `cocos3d-x/projects/helloworld/proj.android`.
    

    for ios
        Requirements : Mac OS X 10.7+, Xcode 4.6+
    
        Build :
        * Enter *proj.ios* folder
        * open *helloworld.xcodeproj*
        * Select ios targets in scheme toolbar , build
        
        Run :
        * Enter *proj.ios* folder
        * open *helloworld.xcodeproj*
        * Select ios targets in scheme toolbar , run


    for win32
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



### Contact us
   * Forum: 
   * Twitter: 
   * Weibo:
   * QQ:
   
### some useful web resource about 3d

   * matrix transform
     * http://blog.sina.com.cn/s/blog_4a657c5a0100090b.html
     * http://hi.baidu.com/xoxoxo/item/db76cd2988fa0385ae48f5c0
   * fbx sdk : http://blog.sina.com.cn/s/blog_4a657c5a01014da9.html
   * soft shadow : http://blog.sina.com.cn/s/blog_4a657c5a0100w8il.html   * 
   * opengl render optimize : http://blog.sina.com.cn/s/blog_4a657c5a01015tyo.html   * 
   * Gimbal Lock ： http://blog.sina.com.cn/s/blog_4a657c5a0100w6sb.html
