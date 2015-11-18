#ifndef _FIELD_
#define _FIELD_

#include <vector>

#include <IVideoDriver.h>
#include "IGUIFont.h"

class Cell;

class Field
{

	irr::video::IVideoDriver* m_driver;
	irr::gui::IGUIFont* m_font;

	std::vector<Cell*> m_field;

	int m_col;
	int m_row;
	int m_mine_count;
	int m_closed_cell;

	bool m_cleared;
	bool m_fault;
	
	int GetNearMineCount(int x, int y) const;

public:

	void Draw();

	bool InitField(int field_col, int field_row, int mine_count);

	bool IsClear() const { return m_cleared; }
	bool IsFault() const { return m_fault; }

	void OpenCell(int x, int y);
	void ChangeCellMark(int x, int y);
	int GetClosedCell() const { return m_closed_cell; }

	Field(irr::video::IVideoDriver* video_driver, irr::gui::IGUIFont* fnt);
	~Field();

};

#endif