#include "SidePanel.h"

wxDEFINE_EVENT(EVT_NOTIFY_DISPLAY_PANEL, wxCommandEvent);
wxDEFINE_EVENT(EVT_REFRESH_MAIN_FRAME, wxCommandEvent);

SidePanel::SidePanel(wxSplitterWindow* parent, GlobalState* app_state)
    : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN)
{
    main_parent = parent;
    state = app_state; 

    FetchAllNotes();
}

void SidePanel::FetchAllNotes()
{
    wxDir dir("C:\\tempNotes");

    if (!dir.IsOpened())
    {
        wxMessageBox("dir was NOT open");
        // deal with the error here - wxDir would already log an error message
        // explaining the exact reason of the failure
        return;
    }

    // Enumerates through the directory
    wxString dirName = dir.GetName();
    wxArrayString filesArr;
    wxString currentFile;
    FileTraverser traverser(filesArr);
    size_t numberOfFiles = dir.Traverse(traverser);

    // Array of FileStructure to sort by last modified and display later
    std::vector<FileStructure> allFilesWithDetails;

    // Handle displaying the first line as the note title
    wxTextFile textFile;
    wxString fileDisplayTitle;

    wxBoxSizer* allFilesBoxSizer = new wxBoxSizer(wxVERTICAL);

    // First loop to build the array of files with details
    for (int i = 0; i < numberOfFiles; i++) {
        currentFile = filesArr[i];

        FileStructure currentFileWithDetails;
        currentFileWithDetails.lastModified = wxFileModificationTime(currentFile);
        currentFileWithDetails.filename = currentFile;

        bool isFileOpen = textFile.Open(currentFile);

        if (isFileOpen && textFile.GetLineCount() > 0) {
            fileDisplayTitle = textFile.GetFirstLine();
        }
        else {
            fileDisplayTitle = "New Note";
        }
        
        textFile.Close();

        currentFileWithDetails.fileTitleDisplay = fileDisplayTitle;

        allFilesWithDetails.push_back(currentFileWithDetails);
    }

    std::sort(allFilesWithDetails.begin(), allFilesWithDetails.end(), std::greater<FileStructure>());

    // Creates the buttons elements to display
    for (int i = 0; i < allFilesWithDetails.size(); i++) {
        UserData* userData = new UserData();
        userData->fileName = allFilesWithDetails[i].filename;

        wxButton* fileBox = new wxButton(this, wxID_ANY, allFilesWithDetails[i].fileTitleDisplay, wxPoint(0, 30 * i), wxSize(200, 30));
        allFilesBoxSizer->Add(fileBox, 0, wxEXPAND);

        Bind(wxEVT_BUTTON, &SidePanel::SelectNote, this, fileBox->GetId(), -1, userData);
    }
    
    // Display the first note when the app starts or after deleting
    if (numberOfFiles > 0 && !state->GetNoteInView()) {
        state->SetNoteInView(allFilesWithDetails[0].filename);
    }
}

void SidePanel::SelectNote(wxCommandEvent& event) {
    UserData* receivedUserData = (UserData*)event.GetEventUserData();
    state->SetNoteInView(receivedUserData->fileName);
    SendDisplayPanelEvent();
}

void SidePanel::SendDisplayPanelEvent()
{
    wxCommandEvent displayPanelEvent(EVT_NOTIFY_DISPLAY_PANEL, GetId());
    displayPanelEvent.SetEventObject(this);
    displayPanelEvent.SetString("This is display evt");

    ProcessWindowEvent(displayPanelEvent);
}

void SidePanel::SendMainFrameRefreshEvent()
{
    wxCommandEvent mainFrameRefreshEvent(EVT_REFRESH_MAIN_FRAME, GetId());
    mainFrameRefreshEvent.SetEventObject(this);
    mainFrameRefreshEvent.SetString("This is refresh event");

    ProcessWindowEvent(mainFrameRefreshEvent);
}