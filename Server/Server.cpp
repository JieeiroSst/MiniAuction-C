// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
#include "afxsock.h"
#include "math.h"
#include<iostream>
#include<string>
#include<string.h>
#include<fstream>
#include<Windows.h>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;
//nickname
struct NickNmae {
	char  *ten;
};
// hang dau gia
struct MATHANG
{
	char ID[255];
	char ten[255];
	int gia_kn;
};

// chuyen char[] sang int
void String_convert_int(char s[], int &sl)
{
		int res = 0;
		for (int i = 0; i < strlen(s); i++)
		{
			res = res * 10 + (s[i] -'0');
		}
		sl = res;
	
}
//Ham check cú pháp đấu gia của khác hàng xem gia đấu có hợp lện không
bool Check_true_or_false(char cuphap[], char mathang[], int giatiencu, int &gtmoi)
{
	if (cuphap[3] == '-')
	{
		for (int i = 0; i < 3;i++)
		{
			if (cuphap[i] != mathang[i])
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	int res = 0;
	for (int i = 4; i < strlen(cuphap);i++)
	{
		res = res * 10 + (cuphap[i] - '0');
	}
	gtmoi = res;
	return true;
}
// MAIN
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int thangcuoc[20];
	int chay = 0;
	int nRetCode = 0;
	// NAP DỮ LIỆU VÔ STRUCT
	char line_[255];
	int so_luong = 0;
	int dem = 0;
	MATHANG  * HangDauGia;
	string mathangx[3];
	int mhx = 0;
	int _yes = 0;

	std::fstream fileInput("DauGia.txt", std::ios::in);
	// kIỂM TRA XEM FILE CÓ BỊ LỖI
	if (fileInput.fail())
		std::cout << "Failed to open this file!" << std::endl;
	// lấy số lượng hàng để khỏi tao struct Hang dau gia
	char temp1[255];
	fileInput.getline(temp1, 255);
	String_convert_int(temp1, so_luong);
	HangDauGia = new MATHANG[so_luong];// khởi tạo mảng
	// Đọc dữ liệu vô struct mặt hàng
	while (!fileInput.eof())
	{
		char temp[255];
		fileInput.getline(temp, 255);
		strcpy(HangDauGia[dem].ID, temp);

		fileInput.getline(temp, 255);
		strcpy(HangDauGia[dem].ten, temp);

		fileInput.getline(temp, 255);
		int xx;
		String_convert_int(temp, xx);
		HangDauGia[dem].gia_kn = xx;
		dem++;
		
	}
	std::cout << std::endl;
	fileInput.close(); // ĐÓNG FILE NẠP
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		CSocket server;
		unsigned int port = 1234;
		int i;
		AfxSocketInit(NULL);
		server.Create(port);
		server.Listen(5);

		//Nhap so luong client
		int num_client = 1;
		do {
			printf("\n Nhap so luong khach hang: ");
			scanf_s("%d", &num_client);
			if (num_client < 2)
				cout << "So luong khach hang phai lon hon 2.\n";
		} while (num_client < 2);
		//Khởi tạo mảng nickname
		NickNmae  * nickname;
		nickname = new NickNmae[num_client];
		printf("\n Dang lang nghe ket noi tu khach hang...\n");
		//Tao mang chua cac socket client
		CSocket * sockClients = new CSocket[num_client];
		int *money;
		money = new int[num_client];
		for (i = 0; i < num_client; i++){
			server.Accept(sockClients[i]);
			printf("Da tiep nhan khach hang %d/%d\n",i+1,num_client);

			//Bao cho client biet minh la client thu may
			sockClients[i].Send((char*)&i,sizeof(int),0);
			// Nhận nick name từ client
			int MsgSize;
			char *temp;
			do {
				sockClients[i].Receive((char*)&MsgSize, sizeof(int), 0); // Neu nhan loi thi tra ve la SOCKET_ERROR.		
				temp = new char[MsgSize + 1];
				sockClients[i].Receive((char*)temp, MsgSize, 0);
				temp[MsgSize] = '\0';
				//Kiểm tre nick name có bị trùng không
				for (int j = 0; j < i;j++)
				{
					if (strcmp(nickname[j].ten, temp) == 0)
					{
						MsgSize = -1;
						// gửi tin nhắn báo cho client biet nickname bị trùng
						sockClients[i].Send(&MsgSize, sizeof(MsgSize), 0);
					}
				}
				if (MsgSize != -1)
				{
					MsgSize = 0;
					sockClients[i].Send(&MsgSize, sizeof(MsgSize), 0);
					nickname[i].ten = temp;
				}
			} while (MsgSize == -1);//lặp lại khi nick name bị trùng
		}
		// DANH SACH NICKNAME HỢP LỆ
		cout << "Danh sach cac nickname hop le:\n";
		for (int i = 0; i < num_client;i++)
		{
			cout << nickname[i].ten << endl;
		}
		//Gửi số lượng mặt hàng dấu giá chon cilent
		for (int i = 0; i < num_client;i++)
		{
			sockClients[i].Send(&so_luong, sizeof(so_luong), 0);
		}
		//Gửi hàng dấu giá đến tất cả client client
		int size;
		for (int i = 0; i < so_luong; i++)
		{
			cout << "Phien dau gia thu " << i + 1 << ":\n";
			for (int j = 0; j < num_client;j++)
			{
				// ID
				// Gui di do dai thong diep de client biet duoc do dai nay
				size = strlen(HangDauGia[i].ID);
				sockClients[j].Send(&size, sizeof(size), 0);
				// Gui di thong diep voi do dai la MsgSize
				sockClients[j].Send(HangDauGia[i].ID, size, 0);
				// TÊN
				// Gui di do dai thong diep de client biet duoc do dai nay
				size = strlen(HangDauGia[i].ten);
				sockClients[j].Send(&size, sizeof(size), 0);
				// Gui di thong diep voi do dai la MsgSize
				sockClients[j].Send(HangDauGia[i].ten, size, 0);
				// Gia
				// Gui di do dai thong diep de client biet duoc gia san pham
				sockClients[j].Send(&HangDauGia[i].gia_kn, sizeof(size), 0);
				Sleep(60000);// chờ 60 s
				//Nhan gia tien tu khach hang thu j
				int len = 0;
				char * cuphap;
				sockClients[j].Receive((char*)&len, sizeof(int), 0); // Neu nhan loi thi tra ve la SOCKET_ERROR.
				cuphap = new char[len + 1];
				sockClients[j].Receive((char*)cuphap, len, 0);
				cout << "client " << j + 1 << " da tra loi: " << cuphap << endl;
				//if (len > 4)
				//{
					int giatiendg = 0;
					if (Check_true_or_false(cuphap, HangDauGia[i].ID, HangDauGia[i].gia_kn, giatiendg))
					{
						cout << "Khach hay " << j + 1 << " Dau gia san pham hop le.\n" << giatiendg << endl;
						money[j] = giatiendg;
					}
					else
					{
						cout << "Khach hang " << j + 1 << " dau gia san khong hop le.\n";
						money[j] = 0;
					}	
			}
			int max = 0;
			// tim max gia tien
			for (int i = 0; i < num_client; i++)
			{
				if (money[i] > max)
				{
					max = money[i];
				}
			}
			int closes = 0;
			int win = 1;
			// truyen kết quả đến client
			for (int i = 0; i < num_client;i++)
			{
				if (max == money[i] && win != 2 && max > 0)
				{
					thangcuoc[chay++] = i;
					// gửi thắng cuộc
					int win = 1;
					sockClients[i].Send(&win, sizeof(win), 0);
					win = 2;
				}
				else
				{
					sockClients[i].Send(&closes, sizeof(closes), 0);
				}
			}
		}
		// EM ĐÃ BIẾT ĐƯỢC NGƯỜI THẮNG CUỘC NHƯNG KHÔNG KIP LÀM LẤY THỂ NGÂN HÀNG NGƯỜI THẮNG CUỘC

		// ĐÓNG KẾT NỐI
		for (int i = 0; i < num_client;i++)
		{
			sockClients[i].ShutDown(2); //Ngat ca chieu Gui va Nhan
			sockClients[i].Close();
		}
		getchar();
		server.Close();
	}
	getchar();
	return nRetCode;
}
