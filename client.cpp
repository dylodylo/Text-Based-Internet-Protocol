#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <time.h>

#pragma comment(lib,"ws2_32.lib")
#pragma warning (disable:4996)

std::string _Operacja;
std::string _Status;
std::string _ID = "8";
std::string moje_ID = "8";
std::string _Wiadomosc;
std::string _Czas;
bool zmienna_globalna = true;


void czyszczenie_pakietu()
{
	_Operacja = "";
	_Status = "";
	_Wiadomosc = "";
	_Czas = "";
}
std::string get_czas()
{
	time_t czas;
	struct tm * data;
	char godzina[20];

	time(&czas);
	data = localtime(&czas);

	strftime(godzina, 20, "%H:%M:%S", data);
	std::string _Czas = godzina;
	return _Czas;
}

void pakowanie_pakietu(std::string &pakiet, std::string op, std::string st, std::string id, std::string wiad,std::string czas)
{
	//get_czas(_Czas);
	pakiet = "Operacja-)" + op + "(|Status-)" + st + "(|Identyfikator-)" + id + "(|Wiadomosc-)" + wiad + "(|Czas-)" + czas + "(|";
}

void odpakowanie_pakietu(char tablica[])
{
	std::string pom = tablica;
	std::string pom2;
	int XD = 0;
	for (int i = 10; i < pom.size() - 1; i++)
	{
		XD++;
		if (tablica[i] != '(' && tablica[i + 1] != '|')  pom2.push_back(pom[i]);
		if (tablica[i] == '(' && tablica[i + 1] == '|') i = pom.size();
	}
	_Operacja = pom2;
	XD--;
	std::cout << _Operacja << std::endl;
	pom2 = "";
	for (int i = 20 + XD; i < pom.size() - 1; i++)
	{
		XD++;
		if (tablica[i] != '(' && tablica[i + 1] != '|')  pom2.push_back(pom[i]);
		if (tablica[i] == '(' && tablica[i + 1] == '|') i = pom.size();

	}
	_Status = pom2;
	XD--;
	pom2 = "";
	for (int i = 37 + XD; i < pom.size() - 1; i++)
	{
		XD++;
		if (tablica[i] != '(' && tablica[i + 1] != '|')  pom2.push_back(pom[i]);
		if (tablica[i] == '(' && tablica[i + 1] == '|') i = pom.size();

	}
	_ID = pom2;
	XD--;
	pom2 = "";
	for (int i = 50 + XD; i < pom.size() - 1; i++)
	{
		XD++;
		if (tablica[i] != '(' && tablica[i + 1] != '|')  pom2.push_back(pom[i]);
		if (tablica[i] == '(' && tablica[i + 1] == '|') i = pom.size();
	}
	_Wiadomosc = pom2;
	XD--;
	pom2 = "";
	for (int i = 58 + XD; i < pom.size() - 1; i++)
	{
		XD++;
		if (tablica[i] != '(' && tablica[i + 1] != '|')  pom2.push_back(pom[i]);
		if (tablica[i] == '(' && tablica[i + 1] == '|') i = pom.size();
	}
	_Czas = pom2;
	pom2 = "";
}


class Wysylanie
{
	std::string napis;
	SOCKET sock;

public:
	Wysylanie(std::string _napis, SOCKET _sock) : napis(_napis), sock(_sock) {}
	void operator()()
	{
		while (_Status != "Polaczony"&&_Status !="Przyjete")
		{

			//	std::cout << "> ";
			std::getline(std::cin, napis);
			if (napis.size() > 0)
			{
				//czyszczenie_pakietu();
				if (napis == ">zapros")
				{
					czyszczenie_pakietu();
					_Operacja = "Zapros";
					
				}
				if (_Status == "Zaproszenie")
				{
					czyszczenie_pakietu();
					_Operacja = "Zapros";
					

					if (napis == "t") _Status = "Przyjete";
					if (napis == "n") _Status = "Odrzucone";
				}
				_Czas=get_czas();
				pakowanie_pakietu(napis, _Operacja, _Status, moje_ID, _Wiadomosc,_Czas);
				send(sock, napis.c_str(), napis.size(), 0);
				std::cout << std::endl;
			}
			//koniec ³ajla
		}

		while (zmienna_globalna)
		{

			//	std::cout << "> ";
			std::getline(std::cin, napis);
			if (napis.size() > 0)
			{
				//czyszczenie_pakietu();
				//if (napis == ">zapros")
				//{
				//	czyszczenie_pakietu();
				//	_Operacja = "Zapros";
				//}
				czyszczenie_pakietu();
				_Operacja = "Chat";
				_Wiadomosc = napis;
				_Status = "Polaczony";
				
				_Czas = get_czas();
				pakowanie_pakietu(napis, _Operacja, _Status, moje_ID, _Wiadomosc,_Czas);
				send(sock, napis.c_str(), napis.size(), 0);
				std::cout << std::endl;
			}
			//koniec ³ajla
		}
	}
};
class Odbieranie
{
	std::string buf;
	SOCKET sock;

public:
	Odbieranie(std::string _buf, SOCKET _sock) : buf(_buf), sock(_sock) {}

	void operator()()
	{
		char buff[4096];



		ZeroMemory(buff, 4096);
		int bytesReceived = recv(sock, buff, 4096, 0);
		if (bytesReceived > 0)
		{
			std::cout << "Klient odebral pakiet" << std::endl;
			odpakowanie_pakietu(buff);
			moje_ID = _ID;

			std::cout << "SERVER> " << _Operacja << " " << _Status << " " << _ID << " " << _Wiadomosc << std::endl;

			//std::cout << "MOJE ID TO " << _ID << std::endl;
			buf = buff;

		}

		while (_Status != "Polaczony")
		{

			ZeroMemory(buff, 4096);
			int bytesReceived = recv(sock, buff, 4096, 0);
			if (bytesReceived > 0)
			{
				std::cout << "Klient odebral pakiet" << std::endl;
				odpakowanie_pakietu(buff);
				if (_Operacja == "Zapros")
					if (_Status == "Zaproszony")
					{
						{
							std::cout << "Drugi klient chce Cie zaprosic. Zaakceptowac? (t/n)" << std::endl;
							_Status = "Zaproszenie";
						}
					}
				std::cout << "SERVER> " << _Operacja << " " << _Status << " " << _ID << " " << _Wiadomosc << std::endl;

				//std::cout << "MOJE ID TO " << _ID << std::endl;
				buf = buff;

			}
		}

		while (zmienna_globalna)
		{

			ZeroMemory(buff, 4096);
			int bytesReceived = recv(sock, buff, 4096, 0);

			if (bytesReceived > 0)
			{
				std::cout << "Klient odebral pakiet" << std::endl;
				czyszczenie_pakietu();
				odpakowanie_pakietu(buff);
				if (_Operacja == "Rozlaczanie" && _Status == "Rozlaczony") zmienna_globalna=false;
				_Operacja == "Chat";
				std::cout << "SERVER> " << _Operacja << " " << _Status << " " << _ID << " " << _Wiadomosc << std::endl;

				//std::cout << "MOJE ID TO " << _ID << std::endl;
				buf = buff;

			}




		}

	};
};

void main()
{
	std::string ipAddress = "127.0.0.1";
	int port = 54000;

	WSADATA data;
	WORD ver MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock" << wsResult << std::endl;
		return;
	}

	//socket

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket" << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);


	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return;
	}


	char buf[4096];
	std::string userInput;
	std::string buf_pom;



	Odbieranie o1(buf_pom, sock);
	std::thread t1(o1);
	Wysylanie w1(userInput, sock);
	std::thread t2(w1);
	t1.join();
	t2.join();





	closesocket(sock);
	WSACleanup();



}