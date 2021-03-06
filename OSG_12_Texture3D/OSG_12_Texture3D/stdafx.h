// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: 在此处引用程序需要的其他头文件
#include <osg/Node>
#include <osg/Geode>  
#include <osg/Group>

#include <osg/Image> 
#include <osg/StateSet>

#include <osg/TexGen> //指定用于自动生成纹理坐标的函数，可以设置纹理的计算方式是以物体坐标空间还是相机坐标空间来进行不同的计算
#include <osg/TexEnv>
#include <osg/Texture3D>  //继承自osg::Texture类，但它不支持立方图纹理

#include <osgViewer/Viewer>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>