#include <iostream>
void selectionSort(int *niz, int vel)
{
    int p = vel;
    
    do
    {
        p=0;
        
        for(int j=0; j<vel; j++)
        {
            if(j+1<vel && niz[j]>niz[j+1])
            {
                int tmp = niz[j];
                niz[j] = niz[j+1];
                niz[j+1] = tmp;
                
                p=j;
            }
        }
        
    }while(p!=0);
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
