#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include "kwencryptMain.h"
#include <wx/progdlg.h>
#include <wx/app.h>
class ZipUtil {
public:

    // �����Զ����¼������̣߳����½�����
    static void sendEvent(kwencryptFrame *frame, wxString msg, int n) {
        static int lastInt = -9999;
        if (n == lastInt) return; // ��Ҫ�ظ���������Ῠ�����߳�
        lastInt = n;
        wxCommandEvent evt(wxMY_ENCRYPT_EVENT, wxID_ANY); // ���͸����̵߳��¼���������Դ�һЩ��Ϣ
        evt.SetString(msg);
        evt.SetInt(n);
        frame->AddPendingEvent(evt);
    }


    /**
    ����
    files��Դ�ļ���ԴĿ¼�ľ���·������Щ·�����û���ק���öԻ���ѡȡ��Ŀ¼·�������������ļ�����Ŀ¼��·��
    outputPath��ѹ���ļ����������·��

    ��û����;����Ĵ������ȶԻ����ǲ�����ʧ�� 2020��2��25��12:56:09
    **/
    static void writeToZip(wxArrayString files, wxString outputPath, kwencryptFrame *frame)
    {
        // ɾ��ԭ����ļ�
        if (wxFileExists(outputPath)) wxRemoveFile(outputPath);
        // ��ʼ�� wxZipOutputStream
        wxFFileOutputStream out(wxFileName(outputPath).GetFullPath());
        wxZipOutputStream zip(out);
        if (!zip.IsOk()) {
            zip.Close();
            return;
        }
        printf("[Log] wxZipOutputStream initial done!\n");

        // �������ֽڣ�ͬʱ�����ļ���Ŀ¼�Ƿ���ڣ������ڸ��½���
        wxULongLong total = 0; // Դ�ļ����ֽ�
        wxULongLong encryptedSrcSize = 0; // �Ѽ��ܵ��ļ��ֽ�
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

        // ѹ��Դ�ļ�
        for (size_t i = 0; i < files.GetCount(); i++) {
            wxFileName fname(files[i]);
            if (!fname.Exists()) {
                zip.Close();
                return;
            }
            if (fname.FileExists()) {
                // �ļ�ֱ��д�뵽ѹ���ļ���
                wxFileInputStream in(fname.GetFullPath());
                if (!in.IsOk()) {
                    zip.Close();
                    return;
                }
                zip.PutNextEntry(fname.GetFullName());
                zip.Write(in);
                encryptedSrcSize += fname.GetSize(); // �����Ѽ���Դ�ļ��ֽ���
                sendEvent(frame, fname.GetFullPath(), int(encryptedSrcSize.ToDouble() / total.ToDouble() * 100));
            } else {
                // Ŀ¼��ɾ����·����д�룬���� D:\\Folder1\\Folder2\\ �Ͱ� D:\\Folder1\\ ��Ϊǰ׺ɾ��
                wxArrayString folder;
                wxString prePath = fname.GetPath(); // Ҫɾ����ǰ׺·��
                wxString parentPath = fname.GetFullPath();
                parentPath.Replace(prePath, ""); // ɾ��ǰ׺
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
                        path.Replace(prePath, ""); // ɾ��ǰ׺
                        zip.PutNextEntry(path);
                        zip.Write(in);
                        encryptedSrcSize += fname2.GetSize(); // �����Ѽ���Դ�ļ��ֽ���
                        sendEvent(frame, fname2.GetFullPath(), int(encryptedSrcSize.ToDouble() / total.ToDouble() * 100));
                    } else {
                        wxString path = fname2.GetFullPath();
                        path.Replace(prePath, ""); // ɾ��ǰ׺
                        zip.PutNextDirEntry(path);
                    }
                }
            }
        }
        printf("[Log] Compression completed\n");
        sendEvent(frame, "", -1); // -1 ��ʾ���
        zip.Close();
    }


};


