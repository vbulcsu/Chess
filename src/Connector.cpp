#include "Connector.h"

STARTUPINFO sti = {0};
SECURITY_ATTRIBUTES sats = {0};
PROCESS_INFORMATION pi = {0};
HANDLE pipin_w = {0}, pipin_r = {0}, pipout_w = {0}, pipout_r = {0};
BYTE buffer[2048];//[4096];
DWORD writ = {0}, read = {0}, available = {0};
WINBOOL success = FALSE;

void connectToEngine(const char* path)
{
    char p[50];
    strcpy(p, path);

   pipin_w = pipin_r = pipout_w = pipout_r = nullptr;
   sats.nLength = sizeof(sats);
   sats.bInheritHandle = TRUE;
   sats.lpSecurityDescriptor = nullptr;

   CreatePipe(&pipout_r, &pipout_w, &sats, 0);
   CreatePipe(&pipin_r, &pipin_w, &sats, 0);
         
   sti.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
   sti.wShowWindow = SW_HIDE;
   sti.hStdInput = pipin_r;
   sti.hStdOutput = pipout_w;
   sti.hStdError = pipout_w;

   CreateProcess(nullptr, p, nullptr, nullptr, TRUE,0, nullptr, nullptr, &sti, &pi);
}

std::string getNextMove(std::string position)
{
    std::string str;

    position = "position startpos moves "+position+"\ngo\n";
    WriteFile(pipin_w, position.c_str(), position.length(),&writ, nullptr);

    Sleep(300);

    std::string stop;
    stop = "stop\n";
    WriteFile(pipin_w, stop.c_str(), stop.length(), &writ, nullptr);

    short tries = 0;
    int moveIndex = -1;

    do {
        PeekNamedPipe(pipout_r, buffer,sizeof(buffer), &read, &available, nullptr);
        do
        {
            ZeroMemory(buffer, sizeof(buffer));
            if(!ReadFile(pipout_r, buffer, sizeof(buffer), &read, nullptr) || !read) break;
            buffer[read] = 0;
            str+=(char*)buffer;

        } while(read >= sizeof(buffer));

        tries++;
        moveIndex = str.find("bestmove");

    }while (moveIndex == -1 && tries < 5);

    if (tries < 5) {

        return str.substr(moveIndex+9,5);
    }
             
    return "error";
}

void closeConnection()
{
    WriteFile(pipin_w, "quit\n", 5,&writ, nullptr);
    if(pipin_w != nullptr) CloseHandle(pipin_w);
    if(pipin_r != nullptr) CloseHandle(pipin_r);
    if(pipout_w != nullptr) CloseHandle(pipout_w);
    if(pipout_r != nullptr) CloseHandle(pipout_r);
    if(pi.hProcess != nullptr) CloseHandle(pi.hProcess);
    if(pi.hThread != nullptr) CloseHandle(pi.hThread);
}
