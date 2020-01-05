#include <iostream>
#include <vector>

struct Cvor
{
    int vrijednost; 
    Cvor *desni, *lijevi;
};

void INORDER(Cvor *stabloB, int minB, int maxA)
{
    //ako hoces u opadajucem poretku, prvo se spustas u desno podstablo, tj samo zamijenis ifove
    if(stabloB->lijevi!= nullptr)
        INORDER(stabloB->lijevi,minB,maxA);
    
    std::cout<<stabloB->vrijednost;
    
    if(stabloB->desni!= nullptr)
        INORDER(stabloB->desni,minB,maxA);
    
}

//stabloA implementirano kao niz. Na prvom mijestu je korijen, a svaki el pored informacinong sadrzaja sadrzi i indekse koji odgovoraju djeci
// ovakva realizacija se najcesce desava preko Gomile(Heap)

//stabloB implementirano preko strukture Cvor

void preklapanjeSatabala(std::vector<int> stabloA, Cvor *stabloB)
{
    int minA, maxA, minB, maxB;
    
    Cvor *pretraga = stabloB;
    while(true)
    {
        if(pretraga->desni != nullptr)
        pretraga = pretraga->desni;
        else
        break;
    }
    maxB = pretraga->vrijednost;
    
    pretraga = stabloB;
    while(true)
    {
        if(pretraga->lijevi != nullptr)
        pretraga = pretraga->lijevi;
        else 
        break;
    }
    minB = pretraga->vrijednost;
    
    //niz nije gomila, pa se najmanji i najveci clan nalaze skroz lijevo od korijena, i skorz desno od korijena
    int i=0;
    while(true)
    {
        if(2*i+1 < stabloA.size() && stabloA.at(2*i+1) != -1)
        i = 2*i+1;
        else
        break;
    }
    minA = stabloA.at(i);
    
    i=0;
    while(true)
    {
        if(2*i+2 < stabloA.size() && stabloA.at(2*i+2) != -1)
        i = 2*i+2;
        else
        break;
    }
    maxA = stabloA.at(i);
    
    if(minA< maxA && maxA<minB && minB<maxB)
    {
        //izbrisi cvor koji je po velicini odmah prije korijena
        
        Cvor *prije_korijena = stabloB->lijevo;
        Cvor *roditelj=stabloB;
        
        //ako je graf sacinjen samo od cvora
        if(prije_korijena == nullptr)
        {
            delete roditelj;
            return;
        }
        
        
        while(true)
        {
            if(prije_korijena->desno != nullptr)
            {
                roditelj = prije_korijena;
                prije_korijena = prije_korijena->desno;
            }
            else
            break;
        }
        
        if(roditelj != stabloB)
            roditelj->desni = nullptr;
        else
            stabloB->lijevi = prije_korijena->lijevi;
       
        delete prije_korijena;
        
        //za stablo A:
        
        if(stabloA.size() == 1)
        {
            delete stabloA(0);
            return;
        }
        
        int  poz = 1;
        
        while(true)
        {
            if( poz < stabloA.size() && stabloA.at(poz) != -1 )
            poz = 2*poz+2;
            else
            break;
        }
        
        if(2*poz+1 < stabloA.size() && stabloA.at(2*poz+1) != -1)
        {
        
            stabloA.at(poz) = stabloA.at(2*poz+1);
            delete stabloA.at(2*poz+1);
        }
        
        
    }
    else if(minA<minB && minB<maxA && maxA<maxB)
    {
        //ispisati u opadajucem poretku elemente za koje vrijedi minB<element<maxA
        INORDER(stabloB, minB, maxA);
    }
    else
    std::cout<<"nijedan od punudjenih slucajeva";
}

int main()
{
    return 0;
}