#include "DnDialogFile.h"
#include <wx/listbox.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <algorithm>
#include <list>
#include "listData.h"

// wxListCtrl 的排序太难用了，这里我用vector来存数据，用<algorithm>的排序函数，每次排序后将vector里的数据覆盖到listCtrl

// 拖放文件触发该函数
bool DnDialogFile::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filepaths)
{
    wxMessageOutputStderr out = wxMessageOutputStderr(stdout);
    for (size_t i = 0; i < filepaths.GetCount(); i++) {
        wxFileName file = filepaths[i];
        if (file.FileExists()) {
            wxString fileSize = file.GetHumanReadableSize();
            wxString fileType("File");
            fileItems.push_back(FileItem(file, fileSize, fileType));
        } else if (file.DirExists()) {
            wxString fileSize("");
            wxString fileType("Folder");
            fileItems.push_back(FileItem(file, fileSize, fileType));
        } else {
            out.Printf("[Error] \"%s\" is not a file or a folder!", file.GetFullPath());
        }
    }
    overwriteListCtrl(frame->listOriginFiles, fileItems);
    return true;
}

