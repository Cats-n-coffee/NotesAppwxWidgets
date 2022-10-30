#pragma once

#include <wx/wxprec.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/fswatcher.h>
#include <wx/splitter.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "DisplayPanel.h"
#include "SidePanel.h"
#include "GlobalState.h"

class MainFrame : public wxFrame
{
public:
    MainFrame();
    wxSplitterWindow* main_parent;
    DisplayPanel* display_parent;
    SidePanel* side_parent;
    GlobalState* app_state;

    bool CreateWatcherIfNecessary();
    void AddEntry(wxFSWPathType type, wxString filename = wxString());
   
private:
    void OnExit(wxCommandEvent& event);
    void AddNewNote(wxCommandEvent& event);
    void SelectNote(wxCommandEvent& event);
    void SaveNote(wxCommandEvent& event);
    void DeleteNote(wxCommandEvent& event);
    void TriggerFileListRefresh(wxFileSystemWatcherEvent& event);
    void RefreshMainFrame(wxCommandEvent& event);
    void OnBoldClicked(wxCommandEvent& event);
    void OnItalicClicked(wxCommandEvent& event);
    void OnH1Clicked(wxCommandEvent& event);
    void OnH2Clicked(wxCommandEvent& event);
    wxFileSystemWatcher* file_watcher;
};