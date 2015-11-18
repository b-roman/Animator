#ifndef _EVENT_RECEVIER_
#define _EVENT_RECEVIER_

#include <irrlicht.h>
#include "Field.h"

using namespace irr;

class EventRecevier : public IEventReceiver
{
	Field* m_field;

public:
	void SetNewField(Field* f) { m_field = f; }
	virtual bool OnEvent(const irr::SEvent& event) override
	{
		int cell_x;
		int cell_y;

		if (m_field == nullptr)
			return false;

		if (event.EventType == EET_MOUSE_INPUT_EVENT)
		{
			// узнаем координаты €чейки по которой кликнули
			cell_x = event.MouseInput.X / 50;
			//if (event.MouseInput.X % 50)
			//	cell_x++;

			cell_y = event.MouseInput.Y / 50;
			//if (event.MouseInput.Y % 50)
			//	cell_y++;


			switch (event.MouseInput.Event)
			{

			case EMIE_LMOUSE_LEFT_UP:
				m_field->OpenCell(cell_x, cell_y);
				break;

			case EMIE_RMOUSE_LEFT_UP:
				m_field->ChangeCellMark(cell_x, cell_y);
				break;

			default:
				break;
			}
		}

		return false;
	}

	EventRecevier(Field* field)
		: m_field(field) {}

};

#endif
