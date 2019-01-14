Celem zadania jest wykrycie kolizji trójkątów poruszających się na płaszczyźnie, a dokładniej czy obecnie trójkąt (wraz z wnętrzem) przecina się z innym, a jeśli nie to czy w trakcie jednostki czasu dojdzie do kolizji z innym. Trójkąty stykające się wierzchołkiem przecinają się, a także jeśli bok jednego zawiera się w boku drugiego to także przecinają się.

Na wejściu otrzymujemy w pierwszym wierszu liczbę n trójkątów (n<1000), a następnie w każdym wierszu jeden trójkąt w postaci 3 wierzchołków i wektora prędkości zadanych jako pary liczb całkowitych (współrzędne x i y). Czyli w każdym następnym wierszu mamy 8 liczb całkowitych oddzielonych spacjami. Wektor prędkości definiuje nam przesunięcie trójkąta w jednostce czasu.

Na wyjściu w jednym wierszu zwracamy oddzielone spacjami n liczb oznaczające status dla każdego z trójkątów.  Liczby oznaczają:

0 - trójkąt przecina się z innym
1 - trójkąt nie przecina się i nie dojdzie do kolizji (w jednostce czasu)
3 - trójkąt nie przecina się z innym ale dojdzie do kolizji z innym (w jednostce czasu)


IN:
10
31 35 33 30 30 33 3 0
16 13 8 21 20 17 0 -2
15 23 7 29 19 26 -5 -3
33 34 34 34 33 34 -5 -3
39 21 29 19 44 20 2 0
25 20 27 29 24 24 1 -4
46 44 52 53 43 48 0 -3
44 12 51 18 41 15 4 3
19 12 23 14 17 13 -1 2
24 23 32 19 20 21 -2 3

OUT:

 3   3   1   3   0   0   1   1   3   0
