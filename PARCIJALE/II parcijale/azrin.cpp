#include<iostream>
#include<vector>

/*int NadjiUGrafu(UsmjerenGraf ug, std::vector<int> kolizini)
{
    int brojac=0;
    
    for(int i=0; i < ug.dajBrojCvorova(); i++)
    {
        for(int j=0; j < ug.dajBrojCvorova(); j++)
        {
            //u usmjerenom grafu imam matricu susjedstva, a polje matrice obiljezava granu
            
            if(ug.matrica[i][j].posjecen == false )
            {
                ug.matrica[i][j].posjecen = true;
                
                for(int k=0; k < kolizini.size(); k++)
                {
                    if(kolizini.at(k) == ug.matrica[i][j])
                    {
                        brojac++;
                        break;
                    }
                }
            }
            
        }
    }
}*/


//ovo je sablon DFS-a
int ProdjiPoDubini(std::vector<int> koloizioni)
{
    int brojac=0;
    
    for (int i = 0; i < broj_cvorova; i++)
    {
        if(!cvorovi[i].posjecen)
            dfs(i, brojac, kolizini);
    }
    
    return brojac
}

void dfs(int trenutni, const std::vector<int> &kolizioni, int &brojac)
{
    int vrijednost = cvorovi[i].vrijednost;
    
    for(int i=0; i<kolizini.size(); i++)
    {
        if(kolizini[i] == vrijednost)
        brojac++;
    }
    
    cvorovi[trenutni].posjecen = true;
    
    for (int i = 0; i < broj_cvorova; i++)
    {
        //1 u matrici oznacava da postoji veza izmedju cvorova  
        
        if(matrica[trenutni][i]==1 && cvorovi[i].posjecen == false)
            dfs(i);
    }
    
}

int hash()
{}

//koristiti DFS
//atribut hashMape je niz
//vraca borj vrijednosti u grafu
int HeshMapa::Nova(UsmjereniGraf ug)
{
    std::vector<int> kolizioni;
    int brojac=0;
    for(int i=0; i<vel; i++)
    {
        int pozicija = hash( niz[i]);
        
        if( fabs(i-pozicija) != 0)
        kolizioni.push_back(niz[i]);
    }
    
    ibrojac = NadjiUGrafu(ug, kolizioni);
}