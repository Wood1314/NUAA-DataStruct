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
    // ��ȡ���̿���
    hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( hSnapshot == INVALID_HANDLE_VALUE )
    {
        printf( "CreateToolhelp32Snapshot (of processes) ʧ��" );
        return 0;
    }
    // ��������������ṹ�Ĵ�С
    pe32.dwSize = sizeof( PROCESSENTRY32 );
 
    // ��ʼ�оٽ�����Ϣ
    if( !Process32First( hSnapshot, &pe32 ) )
    {
        printf( "Process32First() ʧ��" );
        CloseHandle( hSnapshot ); // �رվ��
        return 0;
    }
 	
    printf("����IDt������t�߳���t���ȼ�t������tռ���ڴ�"); // �������ȼ�
    do {
    	//�򿪶�Ӧid�Ľ���
		hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pe32.th32ProcessID); 
		//GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
        // ��ӡ���������Ϣ
        printf( "\n%u", pe32.th32ProcessID );    // ����id
        printf( "\t%u", pe32.th32ParentProcessID );  // ������id
        printf( "\t%d", pe32.cntThreads );       // �߳���
        printf( "\t%d", pe32.pcPriClassBase );   // �������ȼ�
        printf( "\t%s", pe32.szExeFile );        // ������
      	//printf("\t%dkb",int(pmc.WorkingSetSize)); //ռ���ڴ� 
 
    } while( Process32Next( hSnapshot, &pe32 ) );
 
    CloseHandle( hSnapshot );   //�رվ��
 
    return 0;
}
