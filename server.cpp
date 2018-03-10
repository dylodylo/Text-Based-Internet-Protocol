#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <bitset>
#include <iostream>
#include <numeric>
#include <time.h>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma warning (disable:4996)

std::string pakiet;
std::string _Operacja;
std::string _Status;
std::string _ID;
std::string _Wiadomosc;
std::string _Czas;
int a = 1125;

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

int kl = 0;


void main()
{
	//--------------------------------------------------------------------------
	//winsok
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int ws0k = WSAStartup(ver, &wsData);
	if (ws0k != 0)
	{
		std::cerr << "Can't Initialize winsok! Quitting" << std::endl;
		return;
	}

	//------------------------------------------------------------------------
	//soket

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create a socket! Quitting" << std::endl;
		return;
	}

	//---------------------------------------------------------------------------
	//ip i port do soketu

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//--------------------------------------------------------
	//nas³uch

	listen(listening, SOMAXCONN);

	fd_set master;
	FD_ZERO(&master);
	FD_SET(listening, &master);

	while (true)
	{
		fd_set copy = master;

		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);


		for (int i = 0; i < socketCount; i++)
		{

			SOCKET sock = copy.fd_array[i];
			if (sock == listening && kl < 2)
			{
				kl++;
				//accept n aew connection
				SOCKET client = accept(listening, nullptr, nullptr);
				//add new connection
				FD_SET(client, &master);
				//send welcome

				//wysy³anie pakietu z ID

				int rp = 4096; //rozmiar pakietu
							   //	czyszczenie_pakietu(pakiet);
							   //	std::cout << "Serwer czysci pakiet" << std::endl;
				
				_ID = std::to_string(a);
				a++;
				_Operacja = "NadajID";
				_Status = "Oczekuj na drugiego uzytkownika";
				_Wiadomosc = "Witam na serwerze, Twoje id to " + _ID;
				_Czas = get_czas();
				pakowanie_pakietu(pakiet, _Operacja, _Status, _ID, _Wiadomosc,_Czas);
				std::cout << "Serwer pakuje pakiet" << std::endl;


				send(client, pakiet.c_str(), pakiet.size(), 0);




				/*char *tablica_pakietu;
				tablica_pakietu = &pakiet[0u];*/
				//	send(client, pakiet.c_str(), rp, 0);
				std::cout << "Serwer wyslal pakiet z ID" << std::endl;
				//wys³ano pakiet z ID


			}
			else
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				int bytesIn = recv(sock, buf, 4096, 0);


				if (bytesIn <= 0)
				{
					kl--;
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else
				{
					std::cout << "Serwer odebra³ pakiet" << std::endl;
					/*czyszczenie_pakietu();*/
					odpakowanie_pakietu(buf);
					if (_Operacja == "Zapros")
					{
						if (_Status == "Przyjete")
						{
							for (int i = 0; i < master.fd_count; i++)
							{
								SOCKET outSock = master.fd_array[i];
								if (outSock != listening)
								{
									czyszczenie_pakietu();
									_Operacja = "Chat";
									_Status = "Polaczony";
									_Wiadomosc = "Polaczono z drugim klientem";
									_ID = "0000";
									_Czas = get_czas();
									pakowanie_pakietu(pakiet, _Operacja, _Status, _ID, _Wiadomosc,_Czas);
									send(outSock, pakiet.c_str(), pakiet.size(), 0);
									std::cout << "Serwer wyslal pakiet polaczony" << std::endl;
								}
							}
						}
						else if (_Status == "Odrzucone")
						{
							for (int i = 0; i < master.fd_count; i++)
							{
								SOCKET outSock = master.fd_array[i];
								if (outSock != listening && outSock != sock)
								{
									czyszczenie_pakietu();
									_Operacja = "Zapros";
									_Status = "Niepolaczony";
									_Wiadomosc = "Odrzucono zaproszenie";
									
									pakowanie_pakietu(pakiet, _Operacja, _Status, _ID, _Wiadomosc,_Czas);
									send(outSock, pakiet.c_str(), pakiet.size(), 0);
									std::cout << "Serwer wyslal pakiet odrzucony" << std::endl;
								}
							}
						}

						else
						{
							for (int i = 0; i < master.fd_count; i++)
							{
								SOCKET outSock = master.fd_array[i];
								if (outSock != listening && outSock != sock)
								{
									_Operacja = "Zapros";
									_Status = "Zaproszony";

									_Czas = get_czas();
									pakowanie_pakietu(pakiet, _Operacja, _Status, _ID, _Wiadomosc,_Czas);
									send(outSock, pakiet.c_str(), pakiet.size(),0);
									std::cout << "Serwer wyslal pakiet zaproszony" << std::endl;
								}
							}
						}
						//koniec ifa _Operacja
					}
					else if (_Operacja == "Chat" && _Wiadomosc!=">zakoncz")
					{
						
						for (int i = 0; i < master.fd_count; i++)
						{
							SOCKET outSock = master.fd_array[i];
							if (outSock != listening && outSock != sock)
							{
								std::string pom = _Wiadomosc;
								czyszczenie_pakietu();
								_Operacja = "Chat";
								_Status = "Polaczony";
								
								_Czas = get_czas();
								pakowanie_pakietu(pakiet, _Operacja, _Status, _ID, pom,_Czas);
								send(outSock, pakiet.c_str(), pakiet.size(), 0);
								std::cout << "Serwer wyslal wiadomosc" << std::endl;
							}
						}
					}
					else if (_Operacja == "Chat" && _Wiadomosc == ">zakoncz")
					{
						
						for (int i = 0; i < master.fd_count; i++)
						{
							SOCKET outSock = master.fd_array[i];
							if (outSock != listening)
							{
								std::string pom = _Wiadomosc;
								czyszczenie_pakietu();
								_Operacja = "Rozlaczanie";
								_Status =  "Rozlaczony";
								_ID = "0000";
								_Czas = get_czas();
								pakowanie_pakietu(pakiet, _Operacja, _Status, _ID, pom, _Czas);
								send(outSock, pakiet.c_str(), pakiet.size(), 0);
								std::cout << "Rozlaczenie" << std::endl;
								
							}
						}
					}
					else
					{
						for (int i = 0; i < master.fd_count; i++)
						{
							SOCKET outSock = master.fd_array[i];
							if (outSock != listening)
							{
								_Operacja = "Komunikat";
								_Status = "Niepowodzenie";
								_Wiadomosc = "Wystapil blad";
								_Czas = get_czas();
								pakowanie_pakietu(pakiet, _Operacja, _Status, _ID, _Wiadomosc,_Czas);
								send(outSock, pakiet.c_str(), pakiet.size() , 0);
								std::cout << "Serwer wyslal pakiet komunikat" << std::endl;
							}
						}
					}
					// poni¿ej koniec elsa "Serwer odebra³ pakiet"
				}
			}
		}
	}
	WSACleanup();

}
