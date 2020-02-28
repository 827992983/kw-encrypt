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

    // 发送自定义事件给主线程，更新进度条
    static void sendEvent(kwencryptFrame *frame, wxString msg, int n) {
        //printf("[Log] sendEvent called!\n");
        static long lastTimeStamp = time(NULL);
        long nowTimeStamp = time(NULL);
        if (n == -1) {
            wxCommandEvent evt(wxMY_ENCRYPT_EVENT, wxID_ANY); // 发送给主线程的事件，里面可以带一些信息
            evt.SetString("Done!");
            evt.SetInt(-1);
            Sleep(100);
            frame->AddPendingEvent(evt);
            return;
        }
        if (nowTimeStamp == lastTimeStamp) return;
        lastTimeStamp = nowTimeStamp;
        wxCommandEvent evt(wxMY_ENCRYPT_EVENT, wxID_ANY); // 发送给主线程的事件，里面可以带一些信息
        evt.SetString(msg);
        evt.SetInt(n);
        frame->AddPendingEvent(evt);
    }


    /**
    参数
    files：源文件和源目录的绝对路径，这些路径由用户拖拽或用对话框选取，目录路径不包含其子文件、子目录的路径
    outputPath：压缩文件的输出绝对路径
    **/
    static void encrypt(wxArrayString files, wxString outputPath, kwencryptFrame *frame, std::string password)
    {
        if (files.GetCount() == 0) {
            printf("[Error] file or directory not exist!\n");
            sendEvent(frame, "", -1); // -1 表示完成
            return;
        }
        // 删除原输出文件
        if (wxFileExists(outputPath)) wxRemoveFile(outputPath);
        // 初始化 wxZipOutputStream
        wxFFileOutputStream out(wxFileName(outputPath).GetFullPath() + ".tmp"); // 临时压缩文件
        wxZipOutputStream zip(out);
        if (!zip.IsOk()) {
            zip.Close();
            sendEvent(frame, "", -1); // -1 表示完成
            return;
        }
        printf("[Log] wxZipOutputStream initial done!\n");

        // 计算总字节（同时检验文件、目录是否存在），用于更新进度
        wxULongLong unProcessedSize = 0; // 源文件总字节
        wxULongLong processedSize = 0; // 已加密的文件字节

        for (size_t i = 0; i < files.GetCount(); i++) {
            wxFileName fname(files[i]);
            if (!fname.Exists()) {
                zip.Close();
                printf("[Error] file or directory not exist!\n");
                sendEvent(frame, "", -1); // -1 表示完成
                return;
            }
            if (fname.FileExists()) {
                unProcessedSize += fname.GetSize();
            } else {
                unProcessedSize += wxDir::GetTotalSize(fname.GetFullPath());
            }
        }
        //printf("[Log] calc unProcessedSize size = %lld\n", unProcessedSize);

        // 压缩源文件
        for (size_t i = 0; i < files.GetCount(); i++) {
            wxFileName fname(files[i]);
            if (!fname.Exists()) {
                zip.Close();
                printf("[Error] file or directory not exist!\n");
                sendEvent(frame, "", -1); // -1 表示完成
                return;
            }
            if (fname.FileExists()) {
                // 文件直接写入到压缩文件中
                wxFileInputStream in(fname.GetFullPath());
                if (!in.IsOk()) {
                    zip.Close();
                    sendEvent(frame, "", -1); // -1 表示完成
                    return;
                }
                zip.PutNextEntry(fname.GetFullName());
                zip.Write(in);
                processedSize += fname.GetSize(); // 更新已加密源文件字节数
                sendEvent(frame, fname.GetFullPath(), int(processedSize.ToDouble() / unProcessedSize.ToDouble() * 100));
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
                            sendEvent(frame, "", -1); // -1 表示完成
                            return;
                        }
                        wxString path = fname2.GetFullPath();
                        path.Replace(prePath, ""); // 删除前缀
                        zip.PutNextEntry(path);
                        zip.Write(in);
                        processedSize += fname2.GetSize(); // 更新已加密源文件字节数
                        sendEvent(frame, fname2.GetFullPath(), int(processedSize.ToDouble() / unProcessedSize.ToDouble() * 100));
                    } else {
                        wxString path = fname2.GetFullPath();
                        path.Replace(prePath, ""); // 删除前缀
                        zip.PutNextDirEntry(path);
                    }
                }
            }
        }
        printf("[Log] Compression completed\n");
        if (!zip.Close()) printf("[Error] zip.Close() == false\n");
        if (!out.Close()) printf("[Error] out.Close() == false\n");


        // 获取压缩文件大小，用以统计加密进度
        processedSize = 0;
        unProcessedSize = wxFileName::GetSize(outputPath + ".tmp");
        printf("[Log] processedSize = %lld\tunProcessedSize = %lld\n", processedSize, unProcessedSize);

        // 压缩文件完成，下一步加密压缩文件
        wxFFileInputStream inFile(wxFileName(outputPath).GetFullPath() + ".tmp"); // 临时zip文件
        wxFFileOutputStream outFile(wxFileName(outputPath).GetFullPath()); // 加密文件
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
            size_t lastRead = inFile.LastRead(); // 实际读取字节数
            //printf("[Log] lastRead = %d\n", lastRead);
            AES_CTR_xcrypt_buffer(&ctx, buffer, lastRead);
            if (!outFile.WriteAll(buffer, lastRead)) printf("[Error] WriteAll return false!\n"); // 未测试
            processedSize += lastRead;
            //printf("[Log] processedSize = %lld\tunProcessedSize = %lld\n", processedSize, unProcessedSize);
            sendEvent(frame, "Encryption progress", 100 + int(processedSize.ToDouble() / unProcessedSize.ToDouble() * 100));

        } while (inFile.CanRead());

        inFile.GetFile()->Close();
        outFile.GetFile()->Close();
        wxRemoveFile(outputPath + ".tmp"); // 删除临时压缩文件

        sendEvent(frame, "", -1); // -1 表示完成
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
            size_t lastRead = kweIn.LastRead(); // 实际读取字节数
            AES_CTR_xcrypt_buffer(&ctx, buffer, lastRead);
            if (!tmpOut.WriteAll(buffer, lastRead)) printf("[Error] WriteAll return false!\n");
            processedSize += lastRead;
            //printf("[Log] processedSize = %lld\tunProcessedSize = %lld\n", processedSize, unProcessedSize);
            sendEvent(frame, "Decryption progress", int(processedSize.ToDouble() / unProcessedSize.ToDouble() * 100));
        } while (kweIn.CanRead());
        kweIn.GetFile()->Close();
        tmpOut.GetFile()->Close();
        sendEvent(frame, "", -1); // -1 表示完成

        // 不提供解压缩功能，解密得到zip就算了，因为无法验证密钥正确性，
        // 用错误的密钥解密得到zip后，无法解压
        // 就算密钥正确，要解压也需要很多额外工作，比如设置解压路径，是否全部解压等，干脆把这部分工作交给用户使用压缩软件完成

    }
};


