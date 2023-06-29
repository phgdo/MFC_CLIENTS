#pragma once
class MyCListBox:public CListBox
{
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct, CString username);
};

