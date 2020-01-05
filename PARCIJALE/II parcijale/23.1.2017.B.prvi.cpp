#include <iostream>

int hesh(int broj)
{
    return broj/100;
}

void mergeBrisanjeKolizija(int *A, int velA, int l, int p, int q, int u, int *B)
{
    for(int i=0; i<velA; i++)
    {
        if(A[i] != -1)
        {
            int pozicija = hash(A[i]);
        
            for(int j=0; j<velA; j++)
            {
                if(j != i && A[j] != -1 )
                {
                   int pozicija2 = hash(A[j]);
                    
                    if(pozicija == pozicija2)
                    {
                        if(A[i] > A [j])
                        {
                            A[i] = -1;
                            
                            break;
                        }
                        else
                        A[j] = -1;
                    }
                }
            }   
        }
    }
}