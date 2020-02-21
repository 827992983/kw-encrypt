#include "listData.h"


std::vector<FileItem> fileItems;

void overwriteListCtrl(wxListCtrl *listCtrl, std::vector<FileItem> &files) {
    //printf("[Log] overwriteListCtrl\n");
    // sort
    sort(files.begin(), files.end(), [](const FileItem &item1, const FileItem &item2){
        if (item1.fileType != item2.fileType) {
            return item1.fileType.Cmp(item2.fileType) > 0;
        } else if (item1.fileName != item2.fileName) {
            return item1.fileName.GetFullPath().Cmp(item2.fileName.GetFullPath()) < 0;
        } else if (item1.fileSize != item2.fileSize) {
            return item1.fileSize.Cmp(item2.fileSize) > 0;
        }
        return true;
    });
    // unique
    if (files.size() > 1) {
        auto it = files.begin();
        while (it != files.end() - 1) {
            if (it->fileName.GetFullPath() == (it + 1)->fileName.GetFullPath()) it = files.erase(it);
            else it++;
        }
    }
    // write to listCtrl
    listCtrl->DeleteAllItems();
    wxIcon iconFile, iconFolder;

    //iconFile.LoadFile("res/file.ico", wxBITMAP_TYPE_ICO);
    iconFile.LoadFile("FILEICON", wxBITMAP_TYPE_ICO_RESOURCE);
    iconFolder.LoadFile("FOLDERICON", wxBITMAP_TYPE_ICO_RESOURCE);
    wxImageList *imageList = new wxImageList(16, 16);

    imageList->Add(iconFile);
    imageList->Add(iconFolder);
    listCtrl->SetImageList(imageList, wxIMAGE_LIST_SMALL);

    for (auto it = files.begin(); it != files.end(); it++) {
        int total = listCtrl->GetItemCount();
        long insertedIndex = listCtrl->InsertItem(total, it->fileName.GetFullPath());
        listCtrl->SetItem(insertedIndex, 1, it->fileSize);
        listCtrl->SetItem(insertedIndex, 2, it->fileType);
        if (it->fileType == "File") {
            //imageList->Add(iconFile);
            listCtrl->SetItemImage(insertedIndex, 0);
        } else {
            //imageList->Add(iconFolder);
            listCtrl->SetItemImage(insertedIndex, 1);
        }
    }



    listCtrl->Refresh(); // refresh the scrollBar
}
