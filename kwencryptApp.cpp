/***************************************************************
 * Name:      kwencryptApp.cpp
 * Purpose:   Code for Application Class
 * Author:    kw ()
 * Created:   2020-02-16
 * Copyright: kw ()
 * License:
 **************************************************************/

#include "kwencryptApp.h"

//(*AppHeaders
#include "kwencryptMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(kwencryptApp);

bool kwencryptApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	kwencryptFrame* Frame = new kwencryptFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
