#pragma once

#include <wx/wxprec.h>
#include <wx/dir.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

class UserData : public wxObject
{
public:
	UserData() {};
	wxString fileName;
};