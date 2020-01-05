#include <iostream>

int nzd(int x, int y)
{
    if(y==0)
    return x;
    else
    return nzd(y, x%y);
}

/*Koristeći funkciju fib_petlja, napraviti rekurzivnu verziju fib2_0(int n) koja ima O(n) vrijeme
izvršenja i koristi repnu rekurziju. Dozvoljeno je dodati nove parametre u prototip pod uslovom da
imaju default vrijednost.

int fib_petlja (int n)
{
    if (n<=1) return n; // 0 i 1
    int pretprosli(0), prosli(1), rezultat;
    
    for (int i(2); i<=n; i++)
    {
        rezultat = pretprosli+prosli;
        pretprosli = prosli;
        prosli = rezultat;
    }
    return rezultat;
}
*/

int fib2_0(int n, int pretprosli=0, int prosli=1)
{
    if(n<=1)
    return prosli;
    else
    {
        int rezultat = pretprosli + prosli;
        
        pretprosli = prosli;
        prosli = rezultat;
        
        return fib2_0( n-1, pretprosli, prosli);
    }
    
}

int main() 
{
    std::cout << fib2_0(10);
    return 0;
}
