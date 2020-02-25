#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include "kwencryptMain.h"
#include <wx/progdlg.h>
#include <wx/app.h>
class ZipUtil {
public:

    // 发送自定义事件给主线程，更新进度条
    static void sendEvent(kwencryptFrame *frame, wxString msg, int n) {
        static int lastInt = -9999;
        if (n == lastInt) return; // 不要重复发，否则会卡爆主线程
        lastInt = n;
        wxCommandEvent evt(wxMY_ENCRYPT_EVENT, wxID_ANY); // 发送给主线程的事件，里面可以带一些信息
        evt.SetString(msg);
        evt.SetInt(n);
        frame->AddPendingEvent(evt);
    }


    /**
    参数
    files：源文件和源目录的绝对路径，这些路径由用户拖拽或用对话框选取，目录路径不包含其子文件、子目录的路径
    outputPath：压缩文件的输出绝对路径

    还没加中途出错的处理，进度对话框是不会消失的 2020年2月25日12:56:09
    **/
    static void writeToZip(wxArrayString files, wxString outputPath, kwencryptFrame *frame)
    {
        // 删除原输出文件
        if (wxFileExists(outputPath)) wxRemoveFile(outputPath);
        // 初始化 wxZipOutputStream
        wxFFileOutputStream out(wxFileName(outputPath).GetFullPath());
        wxZipOutputStream zip(out);
        if (!zip.IsOk()) {
            zip.Close();
            return;
        }
        printf("[Log] wxZipOutputStream initial done!\n");

        // 计算总字节（同时检验文件、目录是否存在），用于更新进度
        wxULongLong total = 0; // 源文件总字节
        wxULongLong encryptedSrcSize = 0; // 已加密的文件字节
        for (size_t i = 0; i < files.GetCount(); i++) {
            wxFileName fname(files[i]);
            if (!fname.Exists()) {
                zip.Close();
                return;
            }
            if (fname.FileExists()) {
                total += fname.GetSize();
            } else {
                total += wxDir::GetTotalSize(fname.GetFullPath());
            }
        }
        printf("[Log] calc total size = %lld\n", total);

        // 压缩源文件
        for (size_t i = 0; i < files.GetCount(); i++) {
            wxFileName fname(files[i]);
            if (!fname.Exists()) {
                zip.Close();
                return;
            }
            if (fname.FileExists()) {
                // 文件直接写入到压缩文件中
                wxFileInputStream in(fname.GetFullPath());
                if (!in.IsOk()) {
                    zip.Close();
                    return;
                }
                zip.PutNextEntry(fname.GetFullName());
                zip.Write(in);
                encryptedSrcSize += fname.GetSize(); // 更新已加密源文件字节数
                sendEvent(frame, fname.GetFullPath(), int(encryptedSrcSize.ToDouble() / total.ToDouble() * 100));
            } else {
                // 目录则删除父路径后写入，比如 D:\\Folder1\\Folder2\\ 就把 D:\\Folder1\\ 作为前缀删掉
                wxArrayString folder;
                wxString prePath = fname.GetPath(); // 要删除的前缀路径
                wxString parentPath = fname.GetFullPath();
                parentPath.Replace(prePath, ""); // 删除前缀
                zip.PutNextDirEntry(parentPath);
                wxDirTraverserImplement traverser(folder);
                wxDir dir(fname.GetFullPath());
                dir.Traverse(traverser);

                for (size_t j = 0; j < folder.GetCount(); j++) {
                    wxFileName fname2(folder[j]);
                    if (fname2.FileExists()) {
                        wxFileInputStream in(fname2.GetFullPath());
                        if (!in.IsOk()) {
                            zip.Close();
                            return;
                        }
                        wxString path = fname2.GetFullPath();
                        path.Replace(prePath, ""); // 删除前缀
                        zip.PutNextEntry(path);
                        zip.Write(in);
                        encryptedSrcSize += fname2.GetSize(); // 更新已加密源文件字节数
                        sendEvent(frame, fname2.GetFullPath(), int(encryptedSrcSize.ToDouble() / total.ToDouble() * 100));
                    } else {
                        wxString path = fname2.GetFullPath();
                        path.Replace(prePath, ""); // 删除前缀
                        zip.PutNextDirEntry(path);
                    }
                }
            }
        }
        printf("[Log] Compression completed\n");
        sendEvent(frame, "", -1); // -1 表示完成
        zip.Close();
    }


};


