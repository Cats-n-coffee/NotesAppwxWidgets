#pragma once

#include <wx/dir.h>

class FileTraverser : public wxDirTraverser
{
public:
	FileTraverser(wxArrayString& files) : m_files(files) {}

	virtual wxDirTraverseResult OnFile(const wxString& filename)
	{
		m_files.Add(filename);
		return wxDIR_CONTINUE;
	}

	virtual wxDirTraverseResult OnDir(const wxString& WXUNUSED(dirname))
	{
		return wxDIR_CONTINUE;
	}

private:
	wxArrayString& m_files;
};