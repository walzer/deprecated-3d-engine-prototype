cocos3d-x
=========
cocos3d-x is  a multi-platform game framework for building 3d games,which is based cocos2d-x.


cocos3d-x is:

  * Fast
  * Free
  * Easy to use
  * Community Supported
  
  
## Supported Platforms
- [Windows]
- [iOS]
- [Android]


How to start a new game
-----------------------

1. Download the code from cocos3d download site or https://github.com/cocos2d/cocos3d-x
2. Enter `tools/project-creator`
3. Run the `create_project.py` script

Example:

    $ cd cocos3d-x/tools/project-creator
    $ ./create_project.py -project helloworld -package com.your_company.helloworld -position /projects/helloworld
    $ cd /projects/helloworld

### Build new project for android ###

    $ cd proj.android
    $ ./build_native.sh

### Build new project for ios

* Enter *proj.ios* folder, open *helloworld.xcodeproj*
* Select ios targets in scheme toolbar


### Build new project for win32 ###

* Enter *proj.win32*, open *helloworld.sln* by vs2010


Build Requirements
------------------

* Mac OS X 10.7+, Xcode 4.6+
* Windows 7+, VS 2010+


Runtime Requirements
--------------------
  * iOS 5.0+ for iPhone / iPad games
  * Android 2.3+ for Android games
  * Windows 7+ for Win games


Running Tests
--------------------

Select the test you want from Xcode Scheme chooser.

* For iOS
$ cd cocos3d-x/projects/helloworld
$ cd proj.ios
$ open helloworld.xcodeproj


* For Windows
$ cd cocos3d-x
Open the `cocos3d-win32.vc2010.sln`



* For Android
$ cd cocos3d-x/projects/helloworld
$ cd proj.android
$ ./build_native.sh


Import helloworld Android project using Eclipse(released with Android SDK). The path to be imported is `cocos3d-x/projects/helloworld/proj.android`.



Contact us
----------

   * Forum: 
   * Twitter: 
   * Weibo:
   * QQ:
  
