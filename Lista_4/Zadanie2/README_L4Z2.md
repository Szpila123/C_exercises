Używając kodowania podzbiorów maskami bitowymi napisz funkcję o deklaracji
int print_combinations(int n, int k, const int A[]);
która  wypisze w  osobnych  liniach k-elementowe podzbiory  zbioru n pierwszych liczb tablicy A.
Zakładamy k ⩽ n ⩽ 30.
Przykładowo program o funkcji main

int main(){
    const int A[] = {1, 2, 3, 4, 5, 6};
    print_combinations(6, 4, A);
}
powinien wypisać (wiersze mogą być w innej kolejności):
1 2 3 4 
1 2 3 5 
1 2 4 5 
1 3 4 5 
2 3 4 5 
1 2 3 6 
1 2 4 6 
1 3 4 6 
2 3 4 6 
1 2 5 6 
1 3 5 6 
2 3 5 6 
1 4 5 6 
2 4 5 6 
3 4 5 6 
