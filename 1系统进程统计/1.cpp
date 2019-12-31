#include <Windows.h>
#include <stdio.h>
#include <psapi.h>
#include <TlHelp32.h>
/*
TlHelp32.h for
    PROCESSENTRY32
    CreateToolhelp32Snapshot()
    Process32First()
    Process32Next()
*/
 
int main(int argc, char const *argv[])
{
    HANDLE hSnapshot;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    PROCESS_MEMORY_COUNTERS pmc; 
    // 获取进程快照
    hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( hSnapshot == INVALID_HANDLE_VALUE )
    {
        printf( "CreateToolhelp32Snapshot (of processes) 失败" );
        return 0;
    }
    // 设置输入参数，结构的大小
    pe32.dwSize = sizeof( PROCESSENTRY32 );
 
    // 开始列举进程信息
    if( !Process32First( hSnapshot, &pe32 ) )
    {
        printf( "Process32First() 失败" );
        CloseHandle( hSnapshot ); // 关闭句柄
        return 0;
    }
 	
    printf("进程IDt父进程t线程数t优先级t进程名t占用内存"); // 基本优先级
    do {
    	//打开对应id的进程
		hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pe32.th32ProcessID); 
		//GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
        // 打印进程相关信息
        printf( "\n%u", pe32.th32ProcessID );    // 进程id
        printf( "\t%u", pe32.th32ParentProcessID );  // 父进程id
        printf( "\t%d", pe32.cntThreads );       // 线程数
        printf( "\t%d", pe32.pcPriClassBase );   // 基本优先级
        printf( "\t%s", pe32.szExeFile );        // 进程名
      	//printf("\t%dkb",int(pmc.WorkingSetSize)); //占用内存 
 
    } while( Process32Next( hSnapshot, &pe32 ) );
 
    CloseHandle( hSnapshot );   //关闭句柄
 
    return 0;
}
