// OSG_1_NodeTransform.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"


/*
Description:使用3种空间变换节点的应用
根据用户视点自动进行变换的AutoTransform节点，直接使用变换矩阵进行变换的MatrixTransform节点，使用位移、旋转和缩放值进行变换的PositionAttitudeTransform节点
Author:Epulari T
Date:2018.5.17
*/


/*创建自动变换节点对象AutoTransform，使其始终面对屏幕，即用户视点*/
osg::Transform *createAutoTransform(double posX, osg::Node *model)
{
	//在osg智能指针中进行定义
	osg::ref_ptr<osg::AutoTransform> at = new osg::AutoTransform;
	at->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_SCREEN);
	at->setPosition(osg::Vec3(posX, 0.0, 0.0));
	at->addChild(model);
	//新建对象作为函数结果返回时，应该返回release()，并尽快引入到别的场景中，否则发生内存泄露
	return at.release();
}

/*创建空间变换矩阵节点对象MatrixTransform，并使其沿Z轴逆时针旋转45度*/
osg::Transform *createMatrixTransform(double posX, double rotateZ, osg::Node *model)
{
	//在osg智能指针中进行定义
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	//设置空间变换矩阵的内容，格式都为setMatrix(osg::Matrix::translate(x, y, z))，其中osg::Matrix::Scale和osg::Matrix::rotate也可以被用在其中，并且可以相乘叠加
	//设置mat矩阵后把node加入到mat中，再把mat加入到Group中，在这个例子中model在main函数中就已加入到Group中了
	mt->setMatrix(osg::Matrix::rotate(rotateZ, osg::Z_AXIS)* osg::Matrix::translate(posX, 0.0, 0.0));
	mt->addChild(model);
	//新建对象作为函数结果返回时，应该返回release()，并尽快引入到别的场景中，否则发生内存泄露
	return mt.release();
}

/*创建位置姿态节点对象PositionAttitudeTransform，使其沿Z轴顺时针旋转45度*/
osg::Transform *createPositionAttitudeTransform(double posX, double rotateZ, osg::Node *model)
{
	//在osg智能指针中进行定义
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
	pat->setPosition(osg::Vec3(posX, 0.0, 0.0));
	pat->setAttitude(osg::Quat(rotateZ, osg::Z_AXIS));
	pat->addChild(model);
	//新建对象作为函数结果返回时，应该返回release()，并尽快引入到别的场景中，否则发生内存泄露
	return pat.release();
}

int main(int argc, char **argv)
{
	//读取main函数参数以及外部参数的类，会自动识别外部参数正确与否
	//它是一个单独的类
	//ArgumentParser(int *argc, char **argv)构造函数，传入参数为main的两个参数，前者代表命令行参数个数，后者代表参数的具体值
	osg::ArgumentParser arguments(&argc, argv);
	//如果外部参数正确，则使用osgDB::readNodeFile读取该模型，否则读取本地坐标系模型文件axes.osgt
	osg::Node *model = osgDB::readNodeFiles(arguments);
	if (!model)
		model = osgDB::readNodeFile("axes.osgt");

	//将3个节点加入到场景根节点中，同时它们的子节点均为一个预设的模型节点model，因此实现了场景中叶节点的共享
	osg::ref_ptr<osg::Group> root = new osg::Group;
	//把新创建的节点加入到Group中
	root->addChild(createMatrixTransform(-5.0, osg::PI / 4, model));
	root->addChild(createAutoTransform(0.0, model));
	root->addChild(createPositionAttitudeTransform(5.0, -osg::PI / 4, model));

	//实例化显示图像的类osgViewer::Viewer
	osgViewer::Viewer viewer;
	//设置viewer显示的场景
	viewer.setSceneData(root.get());
	//开始执行渲染操作，返回值一般为1，程序退出或出错返回0
	return viewer.run();
}

