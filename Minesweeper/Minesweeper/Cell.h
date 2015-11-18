#ifndef _CELL_
#define _CELL_

enum MarkType
{
	FLAG = 0,
	QUERY,
	CLOSED,
	OPENED = 10
};

class Cell
{
	MarkType m_type;	
	wchar_t m_count;
	bool m_opened;
	bool m_mine;

public:

	Cell(const Cell&) = delete;
	Cell& operator = (const Cell&) = delete;

	bool IsMine() const { return m_mine; }
	bool IsOpened() const { return m_opened; }
	void SetOpened() { m_opened = true; m_type = OPENED; }

	void ChangeMark();
	MarkType GetCellType() const { return m_type; }
	void SetNearMineCount(int count) { m_count = count; /*static_cast<short>(count) << 8 + 0x3000; */ }
	wchar_t GetNearMineCount() const { return m_count; }
	Cell(bool is_mine = false)
		: m_type(CLOSED), m_opened(false), m_mine(is_mine) {}

};


#endif