#pragma once

#include <wx/wxprec.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filename.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

class GlobalState
{
public:
	GlobalState();
	wxString GetNoteInView();
	void SetNoteInView(wxString noteName);

private:
	wxString noteInView;
};