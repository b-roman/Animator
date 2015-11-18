#include <ctime>

#include <irrlicht.h>
#include "EventRecevier.h"
#include "Field.h"

#pragma comment(lib, "Irrlicht.lib")

using namespace irr;

int main()
{
	srand(time(nullptr));

	IrrlichtDevice* m_device = createDevice(video::EDT_DIRECT3D9
		, core::dimension2d<u32>(800, 600), 16, false, false, false, nullptr);
	m_device->setResizable(true);
	video::IVideoDriver* m_driver = m_device->getVideoDriver();
	scene::ISceneManager* m_smgr = m_device->getSceneManager();

	core::matrix4 ortho_matrix4;
	ortho_matrix4.buildProjectionMatrixOrthoLH(800, 600, 1, -1);

	scene::ICameraSceneNode* m_camera = m_smgr->addCameraSceneNode(nullptr, core::vector3df(0, 0, 1), core::vector3df(0, 0, 0));
	m_camera->setProjectionMatrix(ortho_matrix4);

	u32 frames = 0;

	auto* fnt = m_device->getSceneManager()->getGUIEnvironment()->getSkin()->getFont();

	auto scr_size = m_device->getVideoDriver()->getScreenSize();



	Field* game_field = new Field(m_driver, fnt);

	game_field->InitField(9, 9, 10);
	EventRecevier recv(game_field);

	m_device->setEventReceiver(&recv);

	int x_pos = 0;
	int y_pos = 0;

	while (m_device->run())
	{
		// если победа или поражение то рестарт
		if (game_field->IsClear())
			game_field->InitField(9, 9, 10);
		if (game_field->IsFault())
			game_field->InitField(9, 9, 10);

		m_driver->beginScene();

		game_field->Draw();
		fnt->draw(core::stringw(game_field->GetClosedCell()), core::rect<s32>(700, 50, 720, 60), video::SColor(0xFF00FF00));

		m_driver->endScene();

		if (++frames == 100)
		{
			core::stringw str = L"Irrlicht Engine [";
			str += m_driver->getName();
			str += L"] FPS: ";
			str += static_cast<s32>(m_driver->getFPS());
			m_device->setWindowCaption(str.c_str());
			frames = 0;
		}

	}

	m_device->drop();
	return 0;
}