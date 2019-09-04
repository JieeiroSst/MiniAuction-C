// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"
#include "afxsock.h"
#include<string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

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
		CSocket client;
		char sAdd[1000];
		unsigned int port = 1234; //Cung port voi server
		AfxSocketInit(NULL);

		//1. Tao socket
		client.Create();

		// Nhap dic chi IP cua server
		printf("\n Nhap dia chi IP cua server: ");
		gets_s(sAdd);
		
		if (client.Connect(CA2W(sAdd),port))
		{
			printf("\nBan da da ket noi toi server thanh cong.");

			//Nhan tu server, cho biet day la client thu may
			int id;
			client.Receive((char*)&id, sizeof(id), 0);
			printf(" \nServer: Day la khac hang thu %d\n", id + 1);
			srand( (unsigned)time( NULL ) );

			//ĐĂNG KÝ NICK NAME
			int MsgSize;
			do {
				cout << "Nhap nick nanme:(abc@gmail.com): ";
				char nickname[100];
				gets_s(nickname);
				MsgSize = strlen(nickname);
				// Gui di do dai thong diep de Server biet duoc do dai nay
				client.Send(&MsgSize, sizeof(MsgSize), 0);
				// Gui di thong diep voi do dai la MsgSize
				client.Send(nickname, MsgSize, 0);
				// Nhận con số báo thành công hay thất bại đến từ server
				client.Receive((char*)&MsgSize, sizeof(int), 0);
				if (MsgSize != -1)
				{
					cout << "Server: Nickname dang ki thanh cong.\n";
				}
				else
				{
					cout << "Nickname bi trung. Moi nhap nickname khac.\n";
				}
			} while (MsgSize == -1);
			// Nhân danh sách đấu giá từ server
			char *IDH;
			char * ten;
			int gia_kn;
			int kt;
			int somathang;
			cout << "Server: Thong tin mat hang dau gia:\n";
			// Nhận số lượng mặt hàng từ server
			client.Receive((char*)&somathang, sizeof(int), 0);
			cout << "so luong hang dau gia: " << somathang << endl;
			for (int i = 0; i < somathang; i++)
			{
				cout << "PHIEN DAU GIA THU: " << i + 1 << endl;
				// Nhận ID
				client.Receive((char*)&kt, sizeof(int), 0); // Neu nhan loi thi tra ve la SOCKET_ERROR.		
				IDH = new char[kt + 1];
				client.Receive((char*)IDH, kt, 0);
				IDH[kt] = '\0';
				cout << "< ID: " << IDH << " >\t";
				//Nhận tên
				client.Receive((char*)&kt, sizeof(int), 0); // Neu nhan loi thi tra ve la SOCKET_ERROR.		
				ten = new char[kt + 1];
				client.Receive((char*)ten, kt, 0);
				ten[kt] = '\0';
				cout << "< Ten mat hang: " << ten << " >\t";
				//Nhận giá
				client.Receive((char*)&gia_kn, sizeof(int), 0);
				cout << "< Gia: " << gia_kn << " >\n";

				// Nhập giá tiền và mã sản phẩm đấu giá
				char tiendaugia[255];
				cout << "Nhap ID va sotiendaugia:<ID>-<SOTIEN>:\t";
				gets_s(tiendaugia);
				int do_dai;
				do_dai = strlen(tiendaugia);
				// Gui di do dai thong diep de Server biet duoc do dai nay
				client.Send(&do_dai, sizeof(do_dai), 0);
				// Gui di thong diep voi do dai la MsgSize
				client.Send(tiendaugia, do_dai, 0);
				fflush(stdin);

				// nhan ket qua
				client.Receive((char*)&gia_kn, sizeof(int), 0);
				if (gia_kn == 1)
				{
					cout << "Chuc mung ban da chien thang trong lan dau gia nay.\n";
				}
				else
				{
					cout << "Ban da thua trong cuoc dau gia nay. Hay co gan o luot tiep theo nhe.\n";
				}
			}	
		}
		else
			printf("Khong connect duoc toi server....");
		
	}
	getchar();
	return nRetCode;
}
