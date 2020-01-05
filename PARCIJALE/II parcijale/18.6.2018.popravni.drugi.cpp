#include <iostream>
#include <vector>

int particija(std::vector<int> &v, int prvi, int zadnji)
{
    int pivot = v.at(prvi);
    int p = prvi+1;
    
    //mora ici "zadnji", a ne v.size(), jer ne mora znaciti da korisnik hoce cilei niz da sortira
    while(p<zadnji && v.at(i) > pivot)
    p++;
    
    for(int i = p+1; i<v.size(); i++)
    {
        if(v.at(prvi) < v.at(i))
        {
            int tmp = v.at(i);
            v.at(i) = v.at(p);
            v.at(p) = tmp;
            
            p++;
        }
    }
    
    int tmp = v.at(prvi);
    v.at(prvi) = v.at(p-1);
    v.at(p-1) = tmp;
    
    return p-1;
}

void quickSort(std::vector<int> &v, int prvi, int zadnji)
{
    if(prvi < zadnji)
    {
        int p = particija(v, prvi);
    
        quickSort(v, prvi, p-1);
        quickSort(v, p+1, zadnji);
    }
}

int hash(int broj)
{
    return broj + broj%2;
}

//hesh funkcija daje samo uslov za izbacivanje 
void quickHashMap(std::vector<int> &v)
{
    for(int i=0; i<v.size(); i++)
    {
        int pozicija = hash(v.at(i));
        
        for(int j=0; j<v.size(); j++)
        {
            if(j != i)
            {
                int pozicija2 = hash(v.at(j));
                
                if(pozicija == pozicija2)
                {
                    v.at(i) = v.at(v.size()-1);
                    v.resize(v.size()-1);
                    
                    v.at(j) = v.at(v.size()-1);
                    v.resize(v.size()-1);
                }
            }
        }
        
        /*if(pozicija != i)
        {
            int j = i;
            
            while(j != pozicija)
            {
                v.at(j) = v.at(v.size()-1);
                
                v.resize(v.size()-1);
                j++;
            }
        }*/
    }
    
    quickSort(v);
}