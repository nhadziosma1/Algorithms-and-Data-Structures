#include <iostream>

void selectionSort(int *niz, int vel)
{
    for(int i=0; i<vel; i++)
    {
        int pozicija=i;
        
        int element = niz[i];
        
        int j=i-1;
        
        while(j>=0 && element<niz[j])
        {
            int tmp=niz[pozicija];
            niz[pozicija]=niz[j];
            niz[j]=tmp;
            
            j--;
            pozicija--;
        }
        
        if(j>=0)
        niz[j]=element;
        
        /*for(int j=i; j>0; j--)
        {
            if(niz[pozicija]<=niz[j])
            {
                int temp = niz[pozicija];
                
                niz[pozicija] = niz[j];
                niz[j] = temp;
                
                pozicija--;
            }
          
        }*/
    }
}

void ispisiNiz(int *niz, int vel)
{
    
    for(int i=0; i<vel; i++)
    std::cout<<niz[i]<<" ";
}


int main()
{
    int niz[10] = {9,8,7,6,5,4,3,2};
    
    selectionSort(niz, 8);
    ispisiNiz(niz, 8);
    return 0;
}
