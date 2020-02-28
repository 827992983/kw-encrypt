#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
#include "kwencryptMain.h"
#include <wx/progdlg.h>
#include <wx/app.h>

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "aes.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "picosha2.h"
#include <time.h>

#define CTR 1

class EncryptUtil {
public:

    // �����Զ����¼������̣߳����½�����
    static void sendEvent(kwencryptFrame *frame, wxString msg, int n) {
        //printf("[Log] sendEvent called!\n");
        static long lastTimeStamp = time(NULL);
        long nowTimeStamp = time(NULL);
        if (n == -1) {
            wxCommandEvent evt(wxMY_ENCRYPT_EVENT, wxID_ANY); // ���͸����̵߳��¼���������Դ�һЩ��Ϣ
            evt.SetString("Done!");
            evt.SetInt(-1);
            Sleep(100);
            frame->AddPendingEvent(evt);
            return;
        }
        if (nowTimeStamp == lastTimeStamp) return;
        lastTimeStamp = nowTimeStamp;
        wxCommandEvent evt(wxMY_ENCRYPT_EVENT, wxID_ANY); // ���͸����̵߳��¼���������Դ�һЩ��Ϣ
        evt.SetString(msg);
        evt.SetInt(n);
        frame->AddPendingEvent(evt);
    }


    /**
    ����
    files��Դ�ļ���ԴĿ¼�ľ���·������Щ·�����û���ק���öԻ���ѡȡ��Ŀ¼·�������������ļ�����Ŀ¼��·��
    outputPath��ѹ���ļ����������·��
    **/
    static void encrypt(wxArrayString files, wxString outputPath, kwencryptFrame *frame, std::string password)
    {
        if (files.GetCount() == 0) {
            printf("[Error] file or directory not exist!\n");
            sendEvent(frame, "", -1); // -1 ��ʾ���
            return;
        }
        // ɾ��ԭ����ļ�
        if (wxFileExists(outputPath)) wxRemoveFile(outputPath);
        // ��ʼ�� wxZipOutputStream
        wxFFileOutputStream out(wxFileName(outputPath).GetFullPath() + ".tmp"); // ��ʱѹ���ļ�
        wxZipOutputStream zip(out);
        if (!zip.IsOk()) {
            zip.Close();
            sendEvent(frame, "", -1); // -1 ��ʾ���
            return;
        }
        printf("[Log] wxZipOutputStream initial done!\n");

        // �������ֽڣ�ͬʱ�����ļ���Ŀ¼�Ƿ���ڣ������ڸ��½���
        wxULongLong unProcessedSize = 0; // Դ�ļ����ֽ�
        wxULongLong processedSize = 0; // �Ѽ��ܵ��ļ��ֽ�

        for (size_t i = 0; i < files.GetCount(); i++) {
            wxFileName fname(files[i]);
            if (!fname.Exists()) {
                zip.Close();
                printf("[Error] file or directory not exist!\n");
                sendEvent(frame, "", -1); // -1 ��ʾ���
                return;
            }
            if (fname.FileExists()) {
                unProcessedSize += fname.GetSize();
            } else {
                unProcessedSize += wxDir::GetTotalSize(fname.GetFullPath());
            }
        }
        //printf("[Log] calc unProcessedSize size = %lld\n", unProcessedSize);

        // ѹ��Դ�ļ�
        for (size_t i = 0; i < files.GetCount(); i++) {
            wxFileName fname(files[i]);
            if (!fname.Exists()) {
                zip.Close();
                printf("[Error] file or directory not exist!\n");
                sendEvent(frame, "", -1); // -1 ��ʾ���
                return;
            }
            if (fname.FileExists()) {
                // �ļ�ֱ��д�뵽ѹ���ļ���
                wxFileInputStream in(fname.GetFullPath());
                if (!in.IsOk()) {
                    zip.Close();
                    sendEvent(frame, "", -1); // -1 ��ʾ���
                    return;
                }
                zip.PutNextEntry(fname.GetFullName());
                zip.Write(in);
                processedSize += fname.GetSize(); // �����Ѽ���Դ�ļ��ֽ���
                sendEvent(frame, fname.GetFullPath(), int(processedSize.ToDouble() / unProcessedSize.ToDouble() * 100));
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
                            sendEvent(frame, "", -1); // -1 ��ʾ���
                            return;
                        }
                        wxString path = fname2.GetFullPath();
                        path.Replace(prePath, ""); // ɾ��ǰ׺
                        zip.PutNextEntry(path);
                        zip.Write(in);
                        processedSize += fname2.GetSize(); // �����Ѽ���Դ�ļ��ֽ���
                        sendEvent(frame, fname2.GetFullPath(), int(processedSize.ToDouble() / unProcessedSize.ToDouble() * 100));
                    } else {
                        wxString path = fname2.GetFullPath();
                        path.Replace(prePath, ""); // ɾ��ǰ׺
                        zip.PutNextDirEntry(path);
                    }
                }
            }
        }
        printf("[Log] Compression completed\n");
        if (!zip.Close()) printf("[Error] zip.Close() == false\n");
        if (!out.Close()) printf("[Error] out.Close() == false\n");


        // ��ȡѹ���ļ���С������ͳ�Ƽ��ܽ���
        processedSize = 0;
        unProcessedSize = wxFileName::GetSize(outputPath + ".tmp");
        printf("[Log] processedSize = %lld\tunProcessedSize = %lld\n", processedSize, unProcessedSize);

        // ѹ���ļ���ɣ���һ������ѹ���ļ�
        wxFFileInputStream inFile(wxFileName(outputPath).GetFullPath() + ".tmp"); // ��ʱzip�ļ�
        wxFFileOutputStream outFile(wxFileName(outputPath).GetFullPath()); // �����ļ�
        std::vector<unsigned char> vKey(picosha2::k_digest_size);
        picosha2::hash256(password.begin(), password.end(), vKey.begin(), vKey.end());
        assert(vKey.size() == 32);
        uint8_t key[32];
        for (size_t i = 0; i < 32; i++)  key[i] = vKey[i];
        uint8_t iv[16]  = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };
        constexpr int BUFFER_SIZE = 1024;
        uint8_t buffer[BUFFER_SIZE];
        struct AES_ctx ctx;
        AES_init_ctx_iv(&ctx, key, iv);
        do {
            inFile.Read(buffer, BUFFER_SIZE);
            size_t lastRead = inFile.LastRead(); // ʵ�ʶ�ȡ�ֽ���
            //printf("[Log] lastRead = %d\n", lastRead);
            AES_CTR_xcrypt_buffer(&ctx, buffer, lastRead);
            if (!outFile.WriteAll(buffer, lastRead)) printf("[Error] WriteAll return false!\n"); // δ����
            processedSize += lastRead;
            //printf("[Log] processedSize = %lld\tunProcessedSize = %lld\n", processedSize, unProcessedSize);
            sendEvent(frame, "Encryption progress", 100 + int(processedSize.ToDouble() / unProcessedSize.ToDouble() * 100));

        } while (inFile.CanRead());

        inFile.GetFile()->Close();
        outFile.GetFile()->Close();
        wxRemoveFile(outputPath + ".tmp"); // ɾ����ʱѹ���ļ�

        sendEvent(frame, "", -1); // -1 ��ʾ���
    }


    static void decrypt(wxString kweFile, wxString outputPath, kwencryptFrame *frame, std::string password)
    {
//        wxMessageOutputStderr logger = wxMessageOutputStderr(stdout);
//        logger.Printf("[Log] kweFile:%s\n[Log] outputPath:%s\n", kweFile, outputPath);
//        std::cout << "[Log] password:" << password << std::endl;

        wxFileName kweFileName(kweFile);
        wxFFileInputStream kweIn(kweFileName.GetFullPath());
        wxFFileOutputStream tmpOut(outputPath + ".zip");
        wxULongLong unProcessedSize = kweFileName.GetSize();
        wxULongLong processedSize = 0;
        std::vector<unsigned char> vKey(picosha2::k_digest_size);
        picosha2::hash256(password.begin(), password.end(), vKey.begin(), vKey.end());
        assert(vKey.size() == 32);
        uint8_t key[32];
        for (size_t i = 0; i < 32; i++)  key[i] = vKey[i];
        uint8_t iv[16]  = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };
        constexpr int BUFFER_SIZE = 1024;
        uint8_t buffer[BUFFER_SIZE];
        struct AES_ctx ctx;
        AES_init_ctx_iv(&ctx, key, iv);
        do {
            kweIn.Read(buffer, BUFFER_SIZE);
            size_t lastRead = kweIn.LastRead(); // ʵ�ʶ�ȡ�ֽ���
            AES_CTR_xcrypt_buffer(&ctx, buffer, lastRead);
            if (!tmpOut.WriteAll(buffer, lastRead)) printf("[Error] WriteAll return false!\n");
            processedSize += lastRead;
            //printf("[Log] processedSize = %lld\tunProcessedSize = %lld\n", processedSize, unProcessedSize);
            sendEvent(frame, "Decryption progress", int(processedSize.ToDouble() / unProcessedSize.ToDouble() * 100));
        } while (kweIn.CanRead());
        kweIn.GetFile()->Close();
        tmpOut.GetFile()->Close();
        sendEvent(frame, "", -1); // -1 ��ʾ���

        // ���ṩ��ѹ�����ܣ����ܵõ�zip�����ˣ���Ϊ�޷���֤��Կ��ȷ�ԣ�
        // �ô������Կ���ܵõ�zip���޷���ѹ
        // ������Կ��ȷ��Ҫ��ѹҲ��Ҫ�ܶ���⹤�����������ý�ѹ·�����Ƿ�ȫ����ѹ�ȣ��ɴ���ⲿ�ֹ��������û�ʹ��ѹ��������

    }
};


