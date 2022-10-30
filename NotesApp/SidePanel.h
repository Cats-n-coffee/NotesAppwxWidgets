#pragma once

#include <wx/wxprec.h>
#include <wx/dir.h>
#include <wx/splitter.h>
#include <wx/filefn.h>
#include <wx/textfile.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "GlobalState.h"
#include "UserData.h"
#include "FileTraverser.h"

wxDECLARE_EVENT(EVT_NOTIFY_DISPLAY_PANEL, wxCommandEvent);
wxDECLARE_EVENT(EVT_REFRESH_MAIN_FRAME, wxCommandEvent);

class SidePanel : public wxPanel
{
public:
    SidePanel(wxSplitterWindow* parent, GlobalState* app_state);
    wxSplitterWindow* main_parent;
    GlobalState* state;

    void FetchAllNotes();

private:
    struct FileStructure
    {
        time_t lastModified;
        wxString filename;
        wxString fileTitleDisplay;

        bool operator > (const FileStructure& file) const
        {
            return (lastModified > file.lastModified);
        }
    };

    void SelectNote(wxCommandEvent& event);
    void SendDisplayPanelEvent();
    void SendMainFrameRefreshEvent();
};