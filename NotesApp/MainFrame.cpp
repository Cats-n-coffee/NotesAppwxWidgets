#include "MainFrame.h"
#include "Globals.h"
#include "SidePanel.h"
#include "DisplayPanel.h"
#include "GlobalState.h"


MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "The Notes App", wxDefaultPosition, wxSize(800, 600))
{
    this->SetMinSize(wxSize(450, 500));

    wxMenu* menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");

    SetMenuBar(menuBar);
    CreateStatusBar();

    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);

    // Toolbar start
    wxImage::AddHandler(new wxPNGHandler);

    wxBitmap addItemIcon(wxT("plus-resized.png"), wxBITMAP_TYPE_PNG);
    wxBitmap saveIcon(wxT("save.png"), wxBITMAP_TYPE_PNG);
    wxBitmap deleteIcon(wxT("delete.png"), wxBITMAP_TYPE_PNG);
    wxBitmap boldIcon(wxT("bold.png"), wxBITMAP_TYPE_PNG);
    wxBitmap italicIcon(wxT("italic.png"), wxBITMAP_TYPE_PNG);
    wxBitmap h1Icon(wxT("h1.png"), wxBITMAP_TYPE_PNG);
    wxBitmap h2Icon(wxT("h2.png"), wxBITMAP_TYPE_PNG);

    wxToolBar* toolbar = CreateToolBar();
    toolbar->AddTool(ID_ADD_ITEM, wxT("Add New Note"), addItemIcon);
    toolbar->AddTool(ID_SAVE_ITEM, wxT("Save note"), saveIcon);
    toolbar->AddTool(ID_DELETE_ITEM, wxT("Delete note"), deleteIcon);
    toolbar->AddCheckTool(ID_BOLD_FORMAT, wxT("Apply bold style"), boldIcon);
    toolbar->AddCheckTool(ID_ITALIC_FORMAT, wxT("Apply italic style"), italicIcon);
    toolbar->AddCheckTool(ID_H1_FORMAT, wxT("Heading level 1"), h1Icon);
    toolbar->AddCheckTool(ID_H2_FORMAT, wxT("Heading level 2"), h2Icon);
    toolbar->Realize();

    Bind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::AddNewNote, this, ID_ADD_ITEM);
    Bind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::SaveNote, this, ID_SAVE_ITEM);
    Bind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::DeleteNote, this, ID_DELETE_ITEM);
    Bind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnBoldClicked, this, ID_BOLD_FORMAT);
    Bind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnItalicClicked, this, ID_ITALIC_FORMAT);
    Bind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnH1Clicked, this, ID_H1_FORMAT);
    Bind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnH2Clicked, this, ID_H2_FORMAT);
    // Toolbar end

    // Side panel (note list) + display panel start
    main_parent = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER | wxSP_LIVE_UPDATE);

    app_state = new GlobalState();

    side_parent = new SidePanel(main_parent, app_state);
    side_parent->SetBackgroundColour(wxColour(175, 195, 250));
    side_parent->Bind(EVT_NOTIFY_DISPLAY_PANEL, &MainFrame::SelectNote, this);
    side_parent->Bind(EVT_REFRESH_MAIN_FRAME, &MainFrame::RefreshMainFrame, this);
    wxBoxSizer* sidePanelSizer = new wxBoxSizer(wxHORIZONTAL);
    sidePanelSizer->Add(side_parent, 1, wxEXPAND | wxALL, 5);

    display_parent = new DisplayPanel(main_parent, app_state);
    display_parent->SetBackgroundColour(*wxRED);
    wxBoxSizer* displayPanelSizer = new wxBoxSizer(wxHORIZONTAL);
    displayPanelSizer->Add(display_parent, 1, wxEXPAND | wxALL, 5);

    main_parent->SplitVertically(side_parent, display_parent);
    main_parent->SetMinimumPaneSize(200);
    // Side panel + display panel end

    this->Center();
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainFrame::SelectNote(wxCommandEvent& event) {
    display_parent->DisplaySelectedNote();
    side_parent->DestroyChildren();
    side_parent->FetchAllNotes();
}

void MainFrame::AddNewNote(wxCommandEvent& WXUNUSED(event))
{
    int randomNumber;
    srand(time(NULL));
    randomNumber = rand() % 100;
    wxString intToStr = wxString::Format(wxT("%i"), randomNumber);

    wxFile newFile;
    wxString newFilePath(TEXT("C:\\tempNotes\\newnote.txt"));
    wxString fileToSetInState;

    if (wxFileName::FileExists(newFilePath)) {
        wxString newNotePath("C:\\tempNotes\\newnote" + intToStr + ".txt");
        fileToSetInState = newNotePath;

        if (!newFile.Create(newNotePath, false)) {
            bool b = newFile.Open(newNotePath);

            if (!b) {
                wxLogError(TEXT("could not open the note" + newNotePath));
            }
        }
    }
    else {
        if (!newFile.Create(newFilePath, false)) {
            bool b = newFile.Open(newFilePath);

            if (!b) {
                wxLogError(TEXT("could not open file"));
            }
        }
    }

    newFile.Write(wxString("this is a test"));
    newFile.Close();

    std::cout << "Inside main frame" << fileToSetInState << std::endl;
    app_state->SetNoteInView(fileToSetInState);
}

void MainFrame::SaveNote(wxCommandEvent& event)
{
    display_parent->SaveCurrentNote();
}

void MainFrame::DeleteNote(wxCommandEvent& event)
{
    int confirmationDialogReturnValue = wxID_NO;
    wxMessageDialog* confirmNoteDeletion = new wxMessageDialog(
        this,
        wxString("Do you want to permanentely delete this note?"),
        wxString("Confirmation"),
        wxYES_NO | wxCANCEL
        );
    confirmationDialogReturnValue = confirmNoteDeletion->ShowModal();

    if (confirmationDialogReturnValue == wxID_YES) {
        display_parent->DeleteCurrentNote();
    }
}

bool MainFrame::CreateWatcherIfNecessary()
{
    if (file_watcher) {
        return false;
    }

    file_watcher = new wxFileSystemWatcher();
    file_watcher->SetOwner(this);
    Bind(wxEVT_FSWATCHER, &MainFrame::TriggerFileListRefresh, this);

    return true;
}

void MainFrame::TriggerFileListRefresh(wxFileSystemWatcherEvent& event)
{
    int type = event.GetChangeType();

    if ((type == wxFSW_EVENT_CREATE) || (type == wxFSW_EVENT_DELETE)) {
        side_parent->DestroyChildren();
        side_parent->FetchAllNotes();
        display_parent->DisplaySelectedNote();
    }
    if (type == wxFSW_EVENT_RENAME) {
        wxMessageBox("Rename event");
    }
}

void MainFrame::AddEntry(wxFSWPathType type, wxString filename)
{
    wxFileName fn = wxFileName::DirName(filename);
    bool ok = false;

    if (wxFSWPath_Dir)
    {
        ok = file_watcher->Add(fn);
    }
    if (!ok)
        wxMessageBox("Could not watch the dir");
}

void MainFrame::RefreshMainFrame(wxCommandEvent& event)
{
    this->Refresh();
    this->Update();
}

void MainFrame::OnBoldClicked(wxCommandEvent& event)
{
    display_parent->OnBold();
}

void MainFrame::OnItalicClicked(wxCommandEvent& event)
{
    display_parent->OnItalic();
}

void MainFrame::OnH1Clicked(wxCommandEvent& event)
{
    display_parent->OnH1();
}

void MainFrame::OnH2Clicked(wxCommandEvent& event)
{
    display_parent->OnH2();
}