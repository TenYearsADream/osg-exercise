// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: 在此处引用程序需要的其他头文件
#include <osg/Node> //节点类  
#include <osg/Geode> //是个几何节点，可以说是一个几何Group节点，一般的可绘制几何体都是通过它来传向root进行渲染，是OSG几何绘制的最高管理节点  
#include <osg/Group> //对节点起到组织作用，一般作为父节点或者根节点出现  
#include <osg/PositionAttitudeTransform> //位置变换节点类，提供模型的位置变换、大小缩放、原点位置的设置、坐标系的变换  

#include <osgDB/ReadFile>  
#include <osgDB/WriteFile>   

#include <osgViewer/Viewer>  
#include <osgViewer/ViewerEventHandlers> //事件监听  

#include <osgGA/StateSetManipulator> //事件响应类，对渲染状态进行控制  

#include <osgUtil/Optimizer> //优化器  
#include <osgUtil/Simplifier> //简化几何体