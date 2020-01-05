#include <iostream>

int hash(int v)
{
    
}

std::vector<int> ocistiHash(int *niz, int vel, int n)
{
    std::vector<int> vratiMe;
    
    for(int i=0; i<vel; i++)
    {
        int pozicija = hash(niz[i]);
        
        if( (pozicija+n)%vel == i)
        {
            int ovdje=0;
            for(int j=0; j<vratiMe.size(); j++)
            {
                if(vratiMe.at(j) < niz[i])
                break;
                
                ovdje++;
            }
            
            vratiMe.insert(vratiMe.begin()+ovdje, niz[i]);
            
             niz[i] = -1;
        }
    }
    
    
}