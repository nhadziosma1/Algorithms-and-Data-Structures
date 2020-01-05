#include <iostream>

struct Cvor
{
    int vrijednost; 
    Cvor *desni, *lijevi;
};

void ObrisiCvorInjegovuDjecu(Cvor *a, std::vector<int> &obrisani)
{
    if(a->lijevo != nullptr)
    a = a->lijevo;
    
    if(a->desno != nullptr)
    a = a->desno;
    
    obrisani.push_back(a->vrijednost);
    delete a;
}

void POSTORDER(Cvor *a, int trenutni, std::vector<int> &obrisani)
{
    if(a->lijevi != nullptr)
    a = a->lijevi;
    
    if(a->desni != nullptr)
    a = a->desni;
    
    if( a->vrijednost % 2 != 0)
    {
        ObrisiCvorInjegovuDjecu(a, obrisani);
    }
   
}

//"a" je korijen binarnog stabla
std::vector<int> postOrderGomila(Cvor *a)
{
    std::vectr<int> obrisani;
    POSTORDER(a, 0, obrisani);
    
    StvoriGomilu()...........
}