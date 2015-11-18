#include "Cell.h"



void Cell::ChangeMark()
{
	if (m_type != OPENED)
	{
		int type = (static_cast<int>(m_type)+1) % 3;
		m_type = static_cast<MarkType>(type);		
	}
}
