#include "GlobalState.h"

GlobalState::GlobalState() {}

wxString GlobalState::GetNoteInView()
{
	return this->noteInView;
}

void GlobalState::SetNoteInView(wxString noteName)
{
	this->noteInView = noteName;
}