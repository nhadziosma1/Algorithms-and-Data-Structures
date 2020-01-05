#include <iostream>

void POSTORDER(Cvor *stabloB, int n)
{
    if(stabloB->lijevi != nullptr)
        POSTORDER(stabloB, n);
        
    if(stabloB->desni != nullptr)
        POSTORDER(stabloB, n);
    
    delete stabloA;
}

void INORDER(Cvor *stabloB, int &trenutni, int n)
{
    //ako hoces u opadajucem poretku, prvo se spustas u desno podstablo, tj samo zamijenis ifove
    if(stabloB->desni!= nullptr)
        INORDER(stabloB->desni,minB,maxA);
    
    trenutni++;
   
   if(trenutni == n)
   POSTORDER(stabloB, n); //izbrisat ce i trenutni
   
    if(stabloB->lijevi!= nullptr)
        INORDER(stabloB->lijevi,minB,maxA);
    
}

void ObrisiClanNizaInNjegovuDjecu(std::vector<int> stabloA, int indeks)
{
    if(2*indeks+1 < stabloA.size() && stabloA.at(2*indeks+1) != -1)
    ObrisiClanNizaInNjegovuDjecu(stabloA, 2*indeks+1);
    
    if(2*indeks+2 < stabloA.size() && stabloA.at(2*indeks+2) != -1)
    ObrisiClanNizaInNjegovuDjecu(stabloA, 2*indeks+2);
    
    stabloA.at(indeks) = -1;
}

void INORDERzaNiz(std::vector<int> stabloA, int indeks, int n, int &bojac)
{
    if( 2*indeks+1 < stabloA.size() && stabloA.at( 2*indeks+1) != -1)
    INORDERzaNiz(stabloA, 2*indeks+1, n, brojac);
    
    brojac++;
    
    if(brojac == n)
    ObrisiClanNizaInNjegovuDjecu(stabloA, indeks);
    
    if( 2*indeks+2 < stabloA.size() && stabloA.at( 2*indeks+2) != -1)
    INORDERzaNiz(stabloA, 2*indeks+2, n, brojac);
}
//stabloB implementirano preko strukture Cvor
// koji cvor je koji redni brj zavisi od odabira pretrege, tj. da li se koristi preorder, postorder ili inorder

void izbaciNti(std::vector<int> stabloA, Cvor *stabloB, int n)
{
    //stablo B
    INORDER(stabloB, 0, n);
    
    //stablo A
    INORDERzaNiz(stabloA, 0, n, 0);
}

int main()
{
    return 0;
}