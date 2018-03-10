# Text-Based Internet Protocol

Projekt z przedmiotu Technologie Sieciowe.

Temat: Komunikacja pomiędzy klientami poprzez serwer (2:1), w oparciu o autorski protokół tekstowy.

Protokół:

 połączeniowy,

 wszystkie dane przesyłane w postaci tekstowej (sekwencja znaków ASCII),

 każdy komunikat opatrzony znacznikiem czasu,

 struktura elementów nagłówka zdefiniowana jako <i>klucz-)wartość(|</i>

	o (przykład) <i>Operacja-)dodaj(|</i>

 wymagane pola:

	o pole operacji – „Operacja”,

	o pole statusu – „Status”,

	o pole identyfikatora – „Identyfikator”.

 dodatkowe pola zdefiniowane przez programistę.

Funkcje oprogramowania:

 klienta:

	o nawiązanie połączenia z serwerem,

	o uzyskanie identyfikatora sesji,

	o wysłanie zaproszenia do drugiego klienta,

	o przyjęcie/odrzucenie zaproszenie,

	o przesłanie wiadomości tekstowej,

	o zamknięcie sesji komunikacyjnej,

	o zakończenie połączenia.

 serwera:

	o wygenerowanie identyfikatora sesji,

	o informowanie klienta, czy drugi węzeł jest osiągalny:

		 w przypadku braku osiągalności, należy zwrócić błąd.

	o pośredniczenie w transmisji.

Inne:

 identyfikator sesji powinien być przesyłany w trakcie komunikacji