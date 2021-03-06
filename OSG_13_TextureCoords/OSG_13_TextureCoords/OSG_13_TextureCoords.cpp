// OSG_13_TextureCoords.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"


/*
 *只有少数的曲面（如圆锥、圆柱）可以在不产生扭曲的情况下映射到平面上
 *其他的曲面在映射表面时都会产生一定程度的扭曲
 *曲面表面的曲率越大，纹理所需要的扭曲度就越大
 *因而直接指定纹理坐标是非常不方便的（如曲面纹理坐标）
 *
 *通过一个继承自osg::NodeVisitor访问器的纹理生成器遍历模型的所有顶点及法线
 *根据顶点、法线及一定的比例来确定纹理坐标
*/

//纹理坐标生成器，继承自NodeVisitor
class TexCoordGenerator:public osg::NodeVisitor
{
public:
	//遍历所有子节点
	TexCoordGenerator() :NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{
		//
	}

	//通过包围盒来确定合适的比例
	void apply(osg::Geode &geode)
	{
		const osg::BoundingSphere &bsphere = geode.getBound();

		float scale = 10;

		if (bsphere.radius() != 0)
		{
			scale = 5 / bsphere.radius();
		}

		//遍历所有几何体，并设置纹理坐标
		for (unsigned i = 0; i < geode.getNumDrawables(); ++i)
		{
			osg::Geometry *geo = dynamic_cast<osg::Geometry *>(geode.getDrawable(i));

			if (geo)
			{
				osg::Vec2Array *tc = generate_coords(geo->getVertexArray(), geo->getNormalArray(), scale);
				geo->setTexCoordArray(0, tc);
			}
		}
		NodeVisitor::apply(geode);
	}
protected:
	//计算纹理坐标
	osg::Vec2Array *generate_coords(osg::Array *vx, osg::Array *nx, float scale)
	{
		osg::Vec2Array *v2a = dynamic_cast<osg::Vec2Array *>(vx);
		osg::Vec3Array *v3a = dynamic_cast<osg::Vec3Array *>(vx);
		osg::Vec4Array *v4a = dynamic_cast<osg::Vec4Array *>(vx);
		osg::Vec2Array *n2a = dynamic_cast<osg::Vec2Array *>(vx);
		osg::Vec3Array *n3a = dynamic_cast<osg::Vec3Array *>(vx);
		osg::Vec4Array *n4a = dynamic_cast<osg::Vec4Array *>(vx);

		osg::ref_ptr<osg::Vec2Array> tc = new osg::Vec2Array();
		for (unsigned i = 0; i < vx->getNumElements(); ++i)
		{
			osg::Vec3 P;
			if (v2a)
				P.set((*v2a)[i].x(), (*v2a)[i].y(), 0);
			if (v3a)
				P.set((*v3a)[i].x(), (*v3a)[i].y(), (*v3a)[i].z());
			if (v4a)
				P.set((*v4a)[i].x(), (*v4a)[i].y(), (*v4a)[i].z());
			
			osg::Vec3 N(0, 0, 1);
			if (n2a)
				N.set((*n2a)[i].x(), (*n2a)[i].y(), 0);
			if (n3a)
				N.set((*n3a)[i].x(), (*n3a)[i].y(), (*n3a)[i].z());
			if (n4a)
				N.set((*n4a)[i].x(), (*n4a)[i].y(), (*n4a)[i].z());

			int axis = 0;
			if (N.y() > N.x() && N.y() > N.z())
				axis = 1;
			if (-N.y() > N.x() && -N.y() > N.z())
				axis = 1;
			if (N.z() > N.x() && N.z() > N.y())
				axis = 2;
			if (-N.z() > N.x() && -N.z() > N.y())
				axis = 2;
			
			
			osg::Vec2 uv;

			switch (axis)
			{
				case 0: uv.set(P.y(), P.z());
					break;
				case 1: uv.set(P.x(), P.z());
					break;
				case 2: uv.set(P.x(), P.y());
					break;
				default:;
			}

			tc->push_back(uv * scale);
		}
		return tc.release();
	}
};

//创建二维纹理状态对象
osg::ref_ptr<osg::StateSet> createTexture2DState(osg::ref_ptr<osg::Image> image)
{
	//创建二维纹理对象
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
	texture->setDataVariance(osg::Object::DYNAMIC);
	//设置贴图
	texture->setImage(image.get());
	//设置滤波
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	//设置环绕模式
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

	//创建状态集对象
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();
	stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);

	return stateset.release();
}

int main()
{
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("Images/primitives.gif");
	osg::ref_ptr<osg::StateSet> stateset = createTexture2DState(image.get());

	TexCoordGenerator tcg;
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("dumptruck.osg");
	node->accept(tcg);
	node->setStateSet(stateset.get());

	osg::ref_ptr<osg::Group> root = new osg::Group();
	//加入天空盒
	root->addChild(node.get());

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	viewer->setSceneData(root.get());
	viewer->realize();

	return viewer->run();
}