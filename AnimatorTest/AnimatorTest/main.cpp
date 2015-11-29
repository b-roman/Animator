#include <irrlicht.h>
#include "ISceneNodeAnimator.h"
#include "MyAnimator.h"

#pragma comment(lib, "irrlicht.lib")

using namespace irr;

int main()
{
	IrrlichtDevice* m_device = createDevice(video::EDT_DIRECT3D9, core::dimension2d<u32>(800, 600));
	scene::ISceneManager* m_smgr = m_device->getSceneManager();

	scene::ICameraSceneNode* camera = m_device->getSceneManager()->addCameraSceneNodeFPS(/*nullptr, core::vector3df(0, 400, -300)*/);
	


	auto* m_cmgr = m_smgr->getSceneCollisionManager();
	
	auto* font = m_device->getGUIEnvironment()->getSkin()->getFont();

	core::array<core::vector3df> spline;

	spline.push_back(core::vector3d<float>(0, 0, 0));
	spline.push_back(core::vector3d<float>(-300, 0, 0));
	spline.push_back(core::vector3d<float>(100, 0, 100));
	spline.push_back(core::vector3d<float>(200, 0, 200));
	spline.push_back(core::vector3d<float>(400, 0, 100));
	spline.push_back(core::vector3d<float>(0, 0, 0));

	scene::ISceneNodeAnimator* manim = new MyAnimator(0, spline, 50.0);
	m_device->getCursorControl()->setVisible(false);
	
	
	scene::IAnimatedMeshSceneNode* plane_node = m_smgr->addAnimatedMeshSceneNode(m_smgr->getMesh("plane.x"));
	plane_node->setVisible(false);

	/*
	plane_node->setMaterialTexture(0, m_device->getVideoDriver()->getTexture("field.jpg"));
	plane_node->setMaterialFlag(video::EMF_LIGHTING, false);
	plane_node->setMaterialFlag(video::EMF_USE_MIP_MAPS, false);

	plane_node->setScale(core::vector3df(50, 50, 50));
	*/

	auto* water = m_smgr->addWaterSurfaceSceneNode(
		plane_node->getMesh(), 4.0, 700.0, 40.0, nullptr, -1
		, core::vector3df(0.0, 10.0, 0.0)
		, core::vector3df(0, 1, 0)
		, core::vector3df(1, 1, 1));

	water->setMaterialFlag(video::EMF_LIGHTING, false);
	water->setMaterialTexture(0, m_device->getVideoDriver()->getTexture("water.jpg"));
	//water->setMaterialTexture(1, m_device->getVideoDriver()->getTexture("field.jpg"));

	water->setMaterialType(video::EMT_REFLECTION_2_LAYER);

	scene::IAnimatedMeshSceneNode* snode = m_smgr->addAnimatedMeshSceneNode(m_smgr->getMesh("knight.x"));
	snode->setScale(core::vector3df(15, 15, 15));
	
	snode->setMaterialFlag(video::EMF_LIGHTING, false);
	snode->addAnimator(manim);

	auto* selector = m_smgr->createTriangleSelector(snode);
	snode->setTriangleSelector(selector);
	selector = m_smgr->createTriangleSelector(plane_node);
	plane_node->setTriangleSelector(selector);


	scene::IParticleSystemSceneNode* ps =
		m_smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32> (-7, 0, -7, 7, 1, 7), // размер эмиттера(куба)
		core::vector3df(0.0f, 0.06f, 0.0f), // начальное направление
		80, 100, // частота испускания (мин,макс)
		video::SColor(0, 255, 255, 255), // самый темный цвет
		video::SColor(0, 255, 255, 255), // самый яркий цвет
		800, 2000, 0, // време жизни (мин,макс), угол
		core::dimension2df(10.f, 10.f), // минимальный размер частиц
		core::dimension2df(20.f, 20.f)); // максимальный размер частиц

	ps->setEmitter(em); // отдаем эмиттер системе
	em->drop(); // а лично нам он не нужен


	//selector = m_smgr->createOctreeTriangleSelector(cube->getMesh(), cube, 4);
	//cube->setTriangleSelector(selector);

	if (selector)
	{
		scene::ISceneNodeAnimator* anim = m_smgr->createCollisionResponseAnimator(
			selector, camera, core::vector3df(30, 50, 30),
			core::vector3df(0, 0, 0), core::vector3df(0, 30, 0));
		selector->drop(); // вот теперь лично нам селектор больше не нужен
		camera->addAnimator(anim);
		anim->drop(); // аниматор столкновений тоже дропаем,
		// оставляя на попечение камере
	}

	selector->drop();

	while (m_device->run())
	{
		m_device->getVideoDriver()->beginScene();
		//camera->setPosition(snode->getPosition() + core::vector3df(0.0, 55.0, 0.0));
		//camera->setTarget(snode->getRotation());
		m_device->getSceneManager()->drawAll();
		
		core::line3df ray;
		ray.start = camera->getPosition();
		ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;

		// переменная под хранение точки пересещения
		core::vector3df intersection;
		// переменная под хранение треугольника с которым пересекся луч
		core::triangle3df hitTriangle;
		scene::ISceneNode * selectedSceneNode =
			m_cmgr->getSceneNodeAndCollisionPointFromRay(
			ray,
			intersection, // точка столкновения
			hitTriangle, // полигон(треугольник) в котором
			// точка столкновения
			-1, // определять столкновения только для
			//нод с идентификатором IDFlag_IsPickable
			0);
		if (selectedSceneNode)
		{
			font->draw(
				core::stringw(intersection.X) + " " +
				core::stringw(intersection.Y) + " " +
				core::stringw(intersection.Z) + " ", core::rect<s32>(10, 40, 150, 60)
				, video::SColor(255, 255, 0, 0));
			font->draw(
				core::stringw(selectedSceneNode->getPosition().X) + " " +
				core::stringw(selectedSceneNode->getPosition().Y) + " " +
				core::stringw(selectedSceneNode->getPosition().Z) + " ", core::rect<s32>(10, 10, 150, 30)
				, video::SColor(255, 255, 0, 0));
		}
		//font->draw(L"123", core::rect<s32>(10, 10, 150, 30), video::SColor(255, 255, 0, 0));

		m_device->getVideoDriver()->endScene();
	}

	delete manim;
	return 0;
}