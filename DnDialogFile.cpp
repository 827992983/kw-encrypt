#include "DnDialogFile.h"
#include <wx/listbox.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <algorithm>
#include <list>
#include "listData.h"
#include <wx/dir.h>
// wxListCtrl ������̫�����ˣ���������vector�������ݣ���<algorithm>����������ÿ�������vector������ݸ��ǵ�listCtrl

// �Ϸ��ļ������ú���
bool DnDialogFile::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filepaths)
{
    wxMessageOutputStderr out = wxMessageOutputStderr(stdout);
    for (size_t i = 0; i < filepaths.GetCount(); i++) {
        wxFileName file = filepaths[i];
        if (file.FileExists()) {
            wxString fileSize = file.GetSize().ToString();
            wxString fileType("File");
            fileItems.push_back(FileItem(file, fileSize, fileType));
        } else if (file.DirExists()) {
            wxString fileSize(wxDir::GetTotalSize(file.GetFullPath()).ToString());
            wxString fileType("Folder");
            fileItems.push_back(FileItem(file, fileSize, fileType));
        } else {
            out.Printf("[Error] \"%s\" is not a file or a folder!", file.GetFullPath());
        }
    }
    overwriteListCtrl(frame->listOriginFiles, fileItems);
    return true;
}

