/**
���б����ݣ����б�Ĳ���Ҳͨ������ļ������API�������Ԫ�ء�����б�
**/
#ifndef LISTDATA_H
#define LISTDATA_H


#include <vector>
#include <wx/string.h>
#include <wx/filename.h>
#include <wx/listctrl.h>
#include <algorithm>
#include <wx/icon.h>
#include <wx/imaglist.h>

class FileItem {
public:
    FileItem(wxFileName n, wxString s, wxString t) : fileName(n), fileSize(s), fileType(t) {}
    wxFileName fileName;
    wxString fileSize;
    wxString fileType;
    bool selected = false;
};

extern std::vector<FileItem> fileItems;


// ��list����ȥ�أ�Ȼ�����listctrl
void overwriteListCtrl(wxListCtrl *listCtrl, std::vector<FileItem> &files);

#endif // LISTDATA_H
