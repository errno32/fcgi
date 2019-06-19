Jeśli zdarzy się kiedyś, że funkcja **fcgi_parse()** wyjdzie poza dopuszczalny
zakres, można spróbować odrobinę zwiększyć wartość *NONBLOCK_WAITER* w pliku
*conf.h*. Pierwotnie ustawiłem ją na 1000 mikrosekund (1 ms).

Możliwe, że ma to związek ze spowolnionym przesyłem danych od serwera HTTP do
nieblokującego się gniazda.
