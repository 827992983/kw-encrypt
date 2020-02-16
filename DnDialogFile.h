#include <wx/dnd.h>
#include <wx/dialog.h>
#include "kwencryptMain.h"
class DnDialogFile : public wxFileDropTarget
{
public:
    DnDialogFile(kwencryptFrame *pOwner) {
        frame = pOwner;
    }

    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filepaths);
private:
    kwencryptFrame *frame;
};
