#include "DisplayPanel.h"
#include "App.h"

DisplayPanel::DisplayPanel(wxSplitterWindow* parent, GlobalState* app_state)
	: wxPanel(parent, -1, wxDefaultPosition, wxSize(270, 150), wxBORDER_SUNKEN)
{
	state = app_state;

	editor = new wxRichTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(350, 400),
		32L, wxDefaultValidator, "Editor");
	editor->WriteText(wxT("Welcome to wxRichTextCtrl"));
	editor->Bind(wxEVT_COMMAND_TEXT_UPDATED, &DisplayPanel::SaveOnChange, this);

	wxFont swissFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

	editor->SetFont(swissFont);
	editor->SetStyleSheet(wxGetApp().GetStyleSheet());

	wxBoxSizer* editorSizer = new wxBoxSizer(wxHORIZONTAL);
	editorSizer->Add(editor, 1, wxEXPAND);

	this->SetSizerAndFit(editorSizer);

	if (state->GetNoteInView() != "") {
		this->DisplaySelectedNote();
	}

	// https://forums.wxwidgets.org/viewtopic.php?t=31708
	// https://docs.wxwidgets.org/3.0/classwx_rich_text_style_sheet.html
	// https://github.com/wxWidgets/wxWidgets/blob/WX_3_0_BRANCH/samples/richtext/richtext.cpp
	// https://docs.wxwidgets.org/3.0/overview_richtextctrl.html
}

void DisplayPanel::DisplaySelectedNote()
{
	wxString selectedFile = wxString::Format("%s", state->GetNoteInView());
	

	if (!editor->LoadFile(selectedFile, 0)) {
		wxMessageBox("Could not open the file");
	}
}

void DisplayPanel::SaveCurrentNote()
{
	wxString selectedFile = wxString::Format("%s", state->GetNoteInView());

	if (!editor->SaveFile(selectedFile, 0)) {
	 	wxMessageBox("Could not save file");
	}
}

void DisplayPanel::DeleteCurrentNote()
{
	wxString selectedFile = wxString::Format("%s", state->GetNoteInView());

	if (!wxRemoveFile(selectedFile)) {
		wxMessageBox("Could not delete the file");
	}
	state->SetNoteInView("");
}

void DisplayPanel::SaveOnChange(wxCommandEvent& event)
{
	this->SaveCurrentNote();
}

void DisplayPanel::OnBold()
{
	editor->ApplyBoldToSelection();
}

void DisplayPanel::OnItalic()
{
	editor->ApplyItalicToSelection();
}

void DisplayPanel::OnH1()
{
	wxRichTextAttr h1Attr;
	h1Attr.SetFlags(wxTEXT_ATTR_FONT_WEIGHT | wxTEXT_ATTR_FONT_SIZE);

	if (editor->GetStyle(editor->GetInsertionPoint(), h1Attr))
	{
		wxRichTextRange range(editor->GetInsertionPoint(), editor->GetInsertionPoint());
		if (editor->HasSelection())
			range = editor->GetSelectionRange();

		h1Attr.SetFontSize(24);
		h1Attr.SetFontWeight(wxFONTWEIGHT_BOLD);
		h1Attr.SetFlags(wxTEXT_ATTR_FONT_WEIGHT | wxTEXT_ATTR_FONT_SIZE);
		editor->SetStyle(range, h1Attr);
	}
}

void DisplayPanel::OnH2()
{
	wxRichTextAttr h2Attr;
	h2Attr.SetFlags(wxTEXT_ATTR_FONT_WEIGHT | wxTEXT_ATTR_FONT_SIZE);

	if (editor->GetStyle(editor->GetInsertionPoint(), h2Attr))
	{
		wxRichTextRange range(editor->GetInsertionPoint(), editor->GetInsertionPoint());
		if (editor->HasSelection())
			range = editor->GetSelectionRange();

		h2Attr.SetFontSize(16);
		h2Attr.SetFontWeight(wxFONTWEIGHT_BOLD);
		h2Attr.SetFlags(wxTEXT_ATTR_FONT_WEIGHT | wxTEXT_ATTR_FONT_SIZE);
		editor->SetStyle(range, h2Attr);
	}
}

// https://docs.wxwidgets.org/trunk/classwx_rich_text_ctrl.html#aa8732eb042af3de0a8aa135a2d6b1d56
// https://docs.wxwidgets.org/trunk/overview_richtextctrl.html