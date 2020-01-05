#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int DajMaksimum(std::vector<int> a)
{
    int maks=a.at(0);
    
    for(int i=0; i<a.size(); i++)
    {
        if(a.at(i)>maks)
        maks=a.at(i);
    }
    
    return maks;
}

void radixSort(std::vector<int> &a)
{
    int maks;
    
    if(a.size()>0)
    maks=DajMaksimum(a);
    
    vector<int> rezultujuci(a.size());
    
    int stepen_desetke=1;
    
    while(maks!=0)
    {
        int histogram[10]={0};
         
        for(int i=0; i<a.size(); i++)
        histogram[ (a.at(i)/stepen_desetke)%10]++;
        
        for(int i=1; i<10; i++)
        histogram[i]+=histogram[i-1];
        
        for(int i=a.size()-1; i>=0; i--)
        {
            rezultujuci.at(histogram[(a.at(i)/stepen_desetke)%10]-1) = a.at(i);
            histogram[(a.at(i)/stepen_desetke)%10]--;
        }
        
        for(int i=0; i<a.size();  i++)
        a.at(i)=rezultujuci.at(i);
        
        maks/=10;
        stepen_desetke*=10;
    }
}

void IspisiNiz(std::vector<int> v)
{
    for(int i=0; i<v.size(); i++)
    std::cout<<v.at(i)<<" ";
    
    std::cout<<std::endl;
}
////////////

bool DaLiJeList(int velicina, int i)
{
    if(i < velicina && i >= velicina/2)
    return true;
    
    return false;
}

int IndeksLijevogDjeteta(const std::vector<int> &a, int i)
{
    return 2*i+1;
}

int IndeksDesnogDjeteta(const std::vector<int> &a, int i)
{
    return 2*i+2;
}

int Roditelj(int i)
{
    return (int)( (i-1)/2 );
}

void PomjeriDolje(std::vector<int> &a, int i, int velicina)
{
    while( DaLiJeList(velicina, i) == false)
    {
        int ind_l_d = IndeksLijevogDjeteta(a, i);
        int ind_d_d = IndeksDesnogDjeteta(a, i);
        
        int ind_veceg_dj = ind_l_d;
        
        if( ind_d_d < velicina && a.at(ind_d_d) > a.at(ind_l_d) )
        ind_veceg_dj = ind_d_d;
        
        if(a.at(i) > a.at(ind_veceg_dj) )
        return;
        
        int tmp = a.at(i);
        a.at(i) = a.at(ind_veceg_dj);
        a.at(ind_veceg_dj) = tmp;
        
        i = ind_veceg_dj;
    }
}

void stvoriGomilu(std::vector<int> &a)
{
    int od_pola = (int)(a.size()/2);
    
    for(int i = od_pola; i >= 0; i--)
        PomjeriDolje(a, i, a.size());
}

void PopraviGore(std::vector<int> &a, int i)
{
    while( i != 0 && a.at(i) > a.at( Roditelj(i) ))
    {
        int tmp = a.at(i);
        a.at(i) = a.at( Roditelj(i) );
        a.at( Roditelj(i) ) = tmp;
        
        i = Roditelj(i);
    }
}

void umetniUGomilu(std::vector<int> &a, int umetnuti, int &velicina)
{
    if(velicina > a.size())
    throw std::domain_error("ma ovo ti ne moze nikako drug moj ljepi");
    
    a.push_back(umetnuti);
    velicina++;
    
    PopraviGore(a, velicina-1);
}

int izbaciPrvi(std::vector<int> &a, int &velicina)
{
    if(a.size() == 0)
    throw std::domain_error("ne mere, al nikako");
    
    velicina--;
    int vracam_se_majci_u_Bosnu = a.at(0);
    
    int tmp = a.at(0);
    a.at(0) = a.at( velicina );
    a.at( velicina )  = tmp;
    
    
    if(velicina != 0)
    PomjeriDolje(a, 0, velicina);
    
    return vracam_se_majci_u_Bosnu;//a.at(velicina);
}

void gomilaSort(std::vector<int> &a)
{
    stvoriGomilu(a);
    
    for(int i = a.size()-1; i>0; i--)
    {
        int tmp = a.at(0);
        
        a.at(0) = a.at(i);
        a.at(i) = tmp;
        
        PomjeriDolje(a, 0, i);
    }
}

int main() 
{
    try
    {
        vector<int> a;
        a.push_back(545);
        a.push_back(1545);
        a.push_back(4536);
        a.push_back(124121);
        a.push_back(1);
        a.push_back(95468);
        int velicina = a.size();
        stvoriGomilu(a);
        umetniUGomilu(a,100000,velicina);   
        umetniUGomilu(a,3,velicina);   
        umetniUGomilu(a,1,velicina);  
        umetniUGomilu(a,100,velicina); 
        umetniUGomilu(a,200,velicina);  
        for(int i(0);i<a.size();i++){
            cout<<a[i]<<" ";
        }
        gomilaSort(a);
        for(int i(0);i<a.size();i++){
            cout<<a[i]<<" ";
        }
    }
    catch(std::domain_error e)
    {
        std::cout<<e.what();
    }
    
    
    /*try
    {
        vector<int> a;
        vector<int> b;
        a.push_back(545);
        a.push_back(1545);
        a.push_back(4536);
        a.push_back(124121);
        a.push_back(1);
        a.push_back(95468);
        for(int i=0;i<10000;i+=13){
            a.push_back(i);
        }
        for(int i=0;i<a.size();i++){
            b.push_back(a[i]);
        }
        radixSort(a);
        gomilaSort(b);
        bool testOk = true;
        for(int i(0);i<a.size();i++){
            if(a[i]!=b[i]) testOk = false;;
        }
        if(testOk) 
        cout<<"OK";
        
    }
    catch(std::domain_error e)
    {
        std::cout<<e.what()<<std::endl;
    }*/
      
   
    
    /*std::vector<int> a = {209,3,48,91,66,101,30,795, 201};
    
    //radixSort(a);
    
    stvoriGomilu(a);
    
    int dv = 12;
    umetniUGomilu(a, 200, dv);
    umetniUGomilu(a, 800, dv);
    umetniUGomilu(a, 1, dv);
    
    IspisiNiz(a);
    
    izbaciPrvi(a, dv);
    IspisiNiz(a);
    
    gomilaSort(a);
    std::cout<<"sortiran "<<std::endl;
    IspisiNiz(a);*/
    return 0;
}
