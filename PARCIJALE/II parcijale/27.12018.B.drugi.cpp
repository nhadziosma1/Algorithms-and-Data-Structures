#include <iostream>
#include <cmath>

//kolizije-orvoreno linearno adresiranje
//sve vrijednosti pozitivne 

//0 znaci da nema elemnta, a -1 znaci da je izbrisan element

int hash1(int v)
{
    
}

int hash2(int v)
{
    
}

void UbaciUdrugi(int *niz2, int vr, int vel)
{
    int pozicija = hash2(vr);
    /* I nacin
    bool treazi = false;
    
    for(int i = pozicija; i<vel; i++)
    {
        if(niz2[i] == -1 || niz2[i] == 0)
        {
            niz[i] = vr;
            break;
        }
        
        if(i+1==vel)
        treazi = true;
    }
    
    for(int i=0; i<pozicija; i++)
    {
        
    }
    */
    // II nacin, brojac betlje sluzi samo da oznaci da moze imati maks "n" potraga za mjestom
    for (int i = 0; i < vel; i++)
    {
        if(niz2[pozicija%vel] == 0 || niz2[pozicija%vel] == -1 )
        {
            niz2[pozicija%vel] = vr;
            break;
        }
        
        pozicija++;
    }
    
    if(i == vel)
        std::cout<<"pun"<<std::endl;
    
}

void premjestiHesh(int *niz1, int vel1, int *niz2, int vel2)
{
    int udaljenost = 0;
    
    for(int i=0; i<vel1; i++)
    {
        int pozicija;
        if(niz[i] != 0 || niz[i] != -1)
        {
            pozicija = hash1( niz1[i] );
        
            //"i" je stvarna pozicija, a "pozicija" je pozicija koju je odredila hes funkcija
            
            if( (pozicija+2)%vel1 <= i)
            {
                UbaciUdrugi(niz2, niz[i], vel2);
                niz[i] = -1;
            }
        }
    }
    
}