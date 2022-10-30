#pragma once

#include <wx/wxprec.h>
#include <wx/dir.h>
#include <wx/textfile.h>
#include <wx/filefn.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/richtext/richtextstyles.h>
#include <wx/splitter.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "GlobalState.h"
#include "Globals.h"

class DisplayPanel : public wxPanel
{
public: 
	DisplayPanel(wxSplitterWindow* parent, GlobalState* app_state);
	wxRichTextCtrl* editor;
	GlobalState* state;

	void DisplaySelectedNote();
	void SaveCurrentNote();
	void DeleteCurrentNote();
	void SaveOnChange(wxCommandEvent& event);
	void OnBold();
	void OnItalic();
	void OnH1();
	void OnH2();
};