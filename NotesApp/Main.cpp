#pragma once

#include <wx/wxprec.h>
#include <wx/dir.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "App.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    AllocConsole();
    //freopen("conin$", "r", stdin); 
    freopen("conout$", "w", stdout); 
    //freopen("conout$", "w", stderr); 
    std::cout << "Hey" << std::endl;

    main_styleSheet = new wxRichTextStyleSheet();
    CreateStyles();

    frame = new MainFrame();
    frame->Show(true);
    return true;
}

void MyApp::CreateStyles()
{
    wxFont swissFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    // Paragraph
    wxRichTextParagraphStyleDefinition* paragraphDefinition = new wxRichTextParagraphStyleDefinition(wxT("Normal"));
    wxRichTextAttr paragraphAttr;
    paragraphAttr.SetFontFaceName(swissFont.GetFaceName());
    paragraphAttr.SetFontSize(12);
    paragraphDefinition->SetStyle(paragraphAttr);

    main_styleSheet->AddParagraphStyle(paragraphDefinition);
}