#pragma once

#include <Windows.h>
#include <wx/wxprec.h>
#include <wx/dir.h>
#include <wx/richtext/richtextstyles.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "MainFrame.h"

class MyApp : public wxApp
{
public:
	virtual bool OnInit();

	void CreateStyles();

	wxRichTextStyleSheet* GetStyleSheet() const { return main_styleSheet; }
	wxRichTextStyleSheet* main_styleSheet;

	virtual void OnEventLoopEnter(wxEventLoopBase* WXUNUSED(loop)) wxOVERRIDE
	{
		if (frame->CreateWatcherIfNecessary())
		{
			if (!dirToWatch.empty())
				frame->AddEntry(wxFSWPath_Dir, dirToWatch);
		}
	}

private:
	MainFrame* frame;
	wxString dirToWatch = "C:\\tempNotes";
};

DECLARE_APP(MyApp);