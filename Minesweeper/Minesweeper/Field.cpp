#include "Field.h"
#include "Cell.h"
#include <algorithm>
using namespace irr;

int RandomNumber(int i)
{
	return rand() % i;
}

int Field::GetNearMineCount(int x, int y) const
{
	int count = 0;

	for (int i = x - 1; i <= x + 1; i++)
		for (int j = y - 1; j <= y + 1; j++)
			if (!(i < 0 || i >= m_col || j < 0 || j >= m_row))
				if (m_field[j * m_col + i]->IsMine())
					count++;
				
	return count;
}

void Field::Draw()
{
	video::SColor CellColor;
	bool opened = false;

	for (int i = 0; i < m_col; ++i)
	{
		for (int j = 0; j < m_row; ++j)
		{
			opened = false;
			switch (m_field[j * m_col + i]->GetCellType())
			{
			case FLAG:
				CellColor.set(0xFFFF0000); // red
				break;
			case QUERY:
				CellColor.set(0xFFAA0000); // light red
				break;
			case CLOSED:
				CellColor.set(0xFF000088); // light blue
				break;
			case OPENED:
				CellColor.set(0xFF007700);
				opened = true;
			default:
				break;
			}
			m_driver->draw2DRectangle(CellColor, core::rect<s32>(
				i * 50 + 2,  j * 50 + 2,
				i * 50 + 48, j * 50 + 48));

			if (opened)
			{
				int count = m_field[j * m_col + i]->GetNearMineCount();
				if (count != 0)
					m_font->draw(core::stringw(count), core::rect<s32>(
					i * 50 + 20, j * 50 + 20,
					i * 50 + 48, j * 50 + 48), video::SColor(0xFFFF0000));
			}
				

		}
	}
}

bool Field::InitField(int col, int row, int mine_count)
{
	if (col >= 4 && row >= 4 && mine_count > 0 && mine_count <= col * row / 2)
	{
		m_field.clear();

		m_cleared = false;
		m_fault = false;

		m_col = col;
		m_row = row;
		m_mine_count = mine_count;
		m_closed_cell = m_col * m_row;

		for (int i = 0; i < col*row - mine_count; ++i)
			m_field.push_back(new Cell());

		for (int i = 0; i < mine_count; ++i)
			m_field.push_back(new Cell(true));

		random_shuffle(m_field.begin(), m_field.end(), RandomNumber);

		return true;
	}
	else
		return false;
}

void Field::OpenCell(int x, int y)
{
	if (m_field[y * m_col + x]->IsOpened())
		return;

	if (x >= 0 && x < m_col && y >= 0 && y < m_row)
	{
		if (m_field[y * m_col + x]->IsMine())
		{
			m_fault = true;
			return;
		}
		else
		{
			m_field[y * m_col + x]->SetOpened();

			m_closed_cell--;
			if (m_closed_cell == 0)
				m_cleared = true;

			int count = GetNearMineCount(x, y);

			if (count != 0)
				m_field[y * m_col + x]->SetNearMineCount(count);
			else
			{
				m_field[y * m_col + x]->SetNearMineCount(0);

				for (int i = x - 1; i <= x + 1; i++)
					for (int j = y - 1; j <= y + 1; j++)
						if (!(i < 0 || i >= m_col || j < 0 || j >= m_row))
							if (!m_field[j * m_col + i]->IsOpened())
								OpenCell(i, j);
					
			}
			
		}
	}
}

void Field::ChangeCellMark(int x, int y)
{
	if (x >= 0 && x < m_col && y >= 0 && y < m_row)
	{
		m_field[y * m_col + x]->ChangeMark();
		if (m_field[y * m_col + x]->GetCellType() == FLAG)
			m_closed_cell--;
		if (m_field[y * m_col + x]->GetCellType() == QUERY)
			m_closed_cell++;

	}
}

Field::Field(irr::video::IVideoDriver* video_driver, irr::gui::IGUIFont* fnt)
	: m_driver(video_driver), m_font(fnt), m_cleared(false), m_fault(false) {}

Field::~Field()
{
	for (auto c : m_field)
		delete c;
}