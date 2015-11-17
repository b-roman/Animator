#include <irrlicht.h>

#pragma comment(lib, "Irrlicht.lib")

using namespace irr;

int main()
{
	IrrlichtDevice* m_device = createDevice(video::EDT_DIRECT3D9
		, core::dimension2d<u32>(800, 600), 16, false, false, false, nullptr);

	video::IVideoDriver* m_driver = m_device->getVideoDriver();
	scene::ISceneManager* m_smgr = m_device->getSceneManager();

	core::matrix4 ortho_matrix4;
	ortho_matrix4.buildProjectionMatrixOrthoLH(800, 600, 1, -1);

	scene::ICameraSceneNode* m_camera = m_smgr->addCameraSceneNode(0, core::vector3df(0, 0, 1), core::vector3df(0, 0, 0));
	m_camera->setProjectionMatrix(ortho_matrix4);

	u32 frames = 0;
	
	while (m_device->run())
	{
		m_driver->beginScene();

		auto* fnt = m_device->getSceneManager()->getGUIEnvironment()->getSkin()->getFont();

		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				m_driver->draw2DRectangle(video::SColor(255, (i + j * 2) * 100 + 100 % 255, 0, 0)
					, core::rect<s32>(i * 50 + 2, j * 50 + 2, i * 50 + 48, j * 50 + 48));
				//fnt->draw(core::stringw(i * 5 + j), core::rect<s32>(i * 50 + 20, j * 50 + 20, i * 50 + 50, j * 50 + 50)
				//	, video::SColor(255, 0, 255, 0));
				fnt->draw(core::stringw(L"#"), core::rect<s32>(i * 50 + 20, j * 50 + 20, i * 50 + 50, j * 50 + 50)
					, video::SColor(255, 0, 255, 0));
			}
		}
		
		
		//m_smgr->drawAll();

		m_driver->endScene();




		if (++frames == 100)
		{
			core::stringw str = L"Irrlicht Engine [";
			str += m_driver->getName();
			str += L"] FPS: ";
			str += (s32)m_driver->getFPS();
			m_device->setWindowCaption(str.c_str());
			frames = 0;
		}

	}

	m_device->drop();
	return 0;
}