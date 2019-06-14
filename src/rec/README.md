# Zapisywanie zdarzeń
Obecnie wszystkie komunikaty kierowane są na standardowe wyjścia, ale nic nie
stoi na przeszkodzie, żeby zgrywać je bezpośrednio do plików.

## Użycie REC()
```c
REC(int type, int errno, const char *format, ...)
```

### Domyślne typy
- OK
- WARNING
- ERROR
- FUCK :-)

### errno
Do zapisu wartości



### Wyjątek
Niestety standardowe listy argumentów w makrach wymagają przynajmniej jednego
argumentu - stąd ten zerowy bajt.
```c
REC("Zwykły komunikat.%c", 0);
```

