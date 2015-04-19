#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;
int length(char s[]){
    int l=0;
    while(s[l]!='\0'){
        l++;
    }
    return l;
}

HANDLE hPort = CreateFile("COM4",
                            GENERIC_WRITE|GENERIC_READ,
                            0,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

DCB dcb={0};
//dcbSerial.DCBlength=sizeof(dcb);

bool writebyte (char* data)
{
    DWORD byteswritten;
    if (!GetCommState(hPort, &dcb))
    {
        cout<<"\n Serial port can't be opened";
        return false;
    }


    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    if (!SetCommState(hPort, &dcb))
    return false;

    bool retVal = WriteFile(hPort, data, 1, &byteswritten, NULL);
    return retVal;
}

int readByte()
{
    int Val;
    BYTE byte;
    DWORD dwBytesTransferred;
    DWORD dwCommModemStatus;
    if(!GetCommState(hPort, &dcb))
        return 0;

    SetCommMask(hPort, EV_RXCHAR|EV_ERR);
    WaitCommEvent(hPort, &dwCommModemStatus, 0);
    if (dwCommModemStatus& EV_RXCHAR)
        ReadFile(hPort, &byte, 1, &dwBytesTransferred, 0);
    Val = byte;
    return Val;
}

/*void sendAndRecieve(char &data)
{
    writebyte(&data);
    cout<<"Data sent "<<int(data)<<"   ";
    char readData=readByte();
    cout<<int(readData)<<endl;
}*/

void send_int(int num){
    unsigned char data;
    for(int i=0; i<4; i++){
        data=num/(1<<(8*(3-i)));

        //writebyte(&data);
        cout<<int(data)<<" ";
        cout<<int(data)<<" | ";
    }
    cout<<endl;
}

void send_char(char c){
    //writebyte(&c);
        cout<<int(c)<<" ";
        cout<<int(c)<<endl;
}

int main(){
    ifstream fin("output.txt");

    if(!fin.is_open()) {
        cerr<<"File not opened";
        return 1;
    }

    cout<<"SENDING VIA XBEE : "<<endl<<endl;

    DWORD n;
    int radius, theta, num_arcs;
    char data, direction;

    fin>>num_arcs;
    while(num_arcs--){
        fin>>radius;
        fin>>theta;
        fin>>direction;

        send_int(radius);
        send_int(theta);
        send_char(direction);
    }
    fin.close();
    CloseHandle(hPort);
    return 0;
}



