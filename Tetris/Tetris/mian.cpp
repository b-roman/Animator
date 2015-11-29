#include <irrlicht.h>

#pragma comment (lib, "irrlicht.lib")

#define CUBE_SIZE 20
#define CUBE_PADDING 1

#include <map>


using namespace irr;

void Rotate(char* src)
{
	char tmp[16];

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; j++)
			tmp[j * 4 + i] = src[j + i * 4];

	memcpy(src, tmp, 16);
}


int main()
{
	IrrlichtDevice* m_device = createDevice(video::EDT_DIRECT3D9, core::dimension2d<u32>(200, 450));

	auto* m_smgr = m_device->getSceneManager();
	auto* m_driver = m_device->getVideoDriver();

	auto* camera = m_smgr->addCameraSceneNode();

	char tetramono[][16] =
	{ 
		{
			0, 0, 0, 0,
			0, 1, 1, 1,
			0, 0, 1, 0,
			0, 0, 0, 0
		},
		{
			0, 0, 1, 0,
			0, 1, 1, 0,
			0, 0, 1, 0,
			0, 0, 0, 0
		},
		{
			0, 0, 1, 0,
			0, 1, 1, 1,
			0, 0, 0, 0,
			0, 0, 0, 0
		},
		{
			0, 0, 1, 0,
			0, 0, 1, 1,
			0, 0, 1, 0,
			0, 0, 0, 0
		}
	
	};

	int x = 0;
	int y = 0;



	while (m_device->run())
	{
		m_driver->beginScene();

		
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; j++)
			{
				if (tetramono[3][j * 4 + i] != 0)
					m_driver->draw2DRectangle(video::SColor(255, 255, 0, 0),
					core::rect<s32>( i * CUBE_SIZE + CUBE_PADDING, j * CUBE_SIZE + CUBE_PADDING
					,  i * CUBE_SIZE + CUBE_SIZE - CUBE_PADDING,j * CUBE_SIZE + CUBE_SIZE - CUBE_PADDING));
			}
		}
		/*
		x += 100;
		y += 100;
		
		Rotate(tetramono);

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; j++)
			{
				if (tetramono[i * 4 + j] != 0)
					m_driver->draw2DRectangle(video::SColor(255, 255, 0, 0),
					core::rect<s32>(x + i * CUBE_SIZE + CUBE_PADDING, y + j * CUBE_SIZE + CUBE_PADDING
					,x + i * CUBE_SIZE + CUBE_SIZE - CUBE_PADDING, y + j * CUBE_SIZE + CUBE_SIZE - CUBE_PADDING));
			}
		}
		*/
		m_smgr->drawAll();

		m_driver->endScene();
	}

	m_device->drop();
	return 0;
}