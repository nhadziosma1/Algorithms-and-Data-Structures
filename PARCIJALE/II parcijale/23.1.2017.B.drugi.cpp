#include <iostream>
#include <vector>

void postorderBrisi(std::vector<int> &a, std::vector<int> &b)
{
    if(2*indeks+1 < a.size() && a.at(2*indeks+1) != -1)
    postorderBrisanje(a, 2*indeks+1, b);
    
    if(2*indeks+2 < a.size() && a.at(2*indeks+2) != -1)
    postorderBrisanje(a, 2*indeks+2, b);
    
    for(int i=0; i<b.size(); i++)
    {
        if(b.at(i) > a.at(indeks))
        {
            b.insert(b.begin()+i, a.at(indeks));
            break;
        }
    }
        
    a.at(indeks) = -1;
}

void postorderNadji(std::vector<int> &a, int indeks, std::vector<int> &b)
{
    if(2*indeks+1 < a.size() && a.at(2*indeks+1) != -1)
    postorderBrisanje(a, 2*indeks+1, b);
    
    if(2*indeks+2 < a.size() && a.at(2*indeks+2) != -1)
    postorderBrisanje(a, 2*indeks+2, b);
    
    if(a.at(indeks)%7 == 0)
    {
        postorderBrisi(a, indeks, b);
    }
}

//vektor "a" je binarno stablo, a vektor "b" je vec sortiran
void brisiCvoroveDjeljiveSaSedam(std::vector<int> &a, std::vector<int> &b)
{
    postorderNadji(a, 0, b);
}