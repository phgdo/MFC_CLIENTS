#include "pch.h"
#include "MyCListBox.h"

void MyCListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct, CString username) {
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	int index = lpDrawItemStruct->itemID;
	if (index >= 0) {
		CString text;
		GetText(index, text);

		COLORREF textColor = RGB(0, 0, 0);
		if (text == username) {
			textColor = RGB(255, 0, 0);
		}
		dc.SetTextColor(textColor);
		dc.DrawText(text, &lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER);
	}
	dc.Detach();
}