// CppClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "CppClient.h"
#include "../MsgServer/Msg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void ProcessMessages()
{
    while (true)
    {
        Message m = Message::Send(M_BROKER, M_GETDATA);
        switch (m.m_Header.m_Type)
        {
        case M_DATA:
        {
            cout << m.m_Data << endl;
            break;
        }
        default:
        {
            Sleep(100);
            break;
        }
        }
    }
}

// The one and only application object
void Client()
{
    AfxSocketInit();
    thread t(ProcessMessages);
    t.detach();
    Message m = Message::Send(M_BROKER, M_INIT);

    while (true)
    {
        string s;
        cin >> s;    
        Message::Send(M_ALL, M_DATA, s);
    }

    Message::Send(M_BROKER, M_EXIT);
}

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            Client();
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
