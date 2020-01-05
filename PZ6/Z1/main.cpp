#include <iostream>
#include <string>
#include <fstream>

#include <ctime>
#include <chrono>

using namespace std;

template <typename Tip1>
void bubble_sort(Tip1 *niz, int vel)
{
    for(int i=vel-1; i>0; i--)
    {
        for(int j=1; j<=i; j++)
        {
            if(niz[j-1]>niz[j])
            {
                Tip1 temp = niz[j-1];
                
                niz[j-1]=niz[j];
                niz[j]=temp;
            }
        }
    }
}

template <typename Tip1>
void bubble_sort_upgrade(Tip1 *niz, int vel)
{
    int p;
    int j=0;
    
    do
    {
        p=0;
        
        for(int i=0; i<vel-j-1; i++)
        {
          if(niz[i]>niz[i+1])
          {
              Tip1 tmp = niz[i];
              
              niz[i] = niz[i+1];
              niz[i+1] = tmp;
              p=i;
          }
        }
        
      j++;  
    }while(p!=0);
    
}

template <typename Tip1>
void selection_sort(Tip1 *niz, int vel)
{
    for(int i=0; i<vel; i++)
    {
        int ind_min_el=i;
        
        for(int j=i+1; j<vel; j++)
        {
            if(niz[ind_min_el]>niz[j])
                ind_min_el=j;
        }
        
        Tip1 temp = niz[i];
        niz[i] = niz[ind_min_el];
        niz[ind_min_el] = temp;
    }
}

template<typename Tip1>
void merge(Tip1* niz, int  l, int p, int q, int u)
{
    //"niz" je niz koji se sastoji od dva podniza koji su, gledajuci ih zasebno, sortirani
    
    int i = 0;   //"i" je indeks prvog clana prvog podniza
    int j = q-l; //"j" je indeks prvog clana drugog podniza
    int k = l;   //"k" je pozicija u rezultujucem nizu u koju ubacujemo najmanji elemnt
    
    Tip1* novi = new Tip1[u-l+1];
    
    //ETAPA I:
    //prepisujes sve u novi niz
    for(int m=0; m<u-l+1; m++)  
    {
        novi[m] = niz[l+m];
    }
    
    //ETAPA II:
    //prepisujes iz novonapravljenog niza, koji se sastoji iz dva podniza elemnte u prvi niz
    while(i <= p-l && j <= u-l)
    {
        // "u-l" je indeks zadnjeg elementa u nizu, a ujedno i zadnji element drugog podniza.
        // Ide "u-l" za drugi podniz granica, jer je "j" koje se krece kroz drugi podniz, odmah postavljeno na "q-l",
        // pa da smo satvili "q-u", j bi odmaha bio ili jednak toj vrijednosti, ili za jedan manji od nje.
        
        //iz prvog podniza ubacujemo u "niz"
        if(novi[i] < novi[j])
        {
            niz[k] = novi[i];
            i++;
        }
        else    //iz drugog podniza ubacujemo u "niz"
        {
            niz[k] = novi[j];
            j++;
        }
        k++;
    }
    
    //ETAPA III:
    //ako su uneseni svi elemnti jednog pod dva podniza, dopuni samo elementima koji su ostali u drugom podnizu
    while(i <= p-l)
    {
        niz[k] = novi[i];
        k++;
        i++;
    }
    
    while(j <= u-l)
    {
        niz[k] = novi[j];
        
        k++;
        j++;
    }
    
    delete[] novi;
}

template<typename Tip1>
void merge_sortiraj(Tip1 *niz, int l, int u)
{
    if(u > l)
    {
        //indeks zadnjeg elemnta prvog podniza
        int p = (int)( (l+u-1)/2 );
        
        //indeks prvog elemnta drugog podniza; odmah element poslije "p"
        int q = p+1;
        
        merge_sortiraj(niz, l, p);
        merge_sortiraj(niz, q, u);
        
        merge(niz, l, p, q, u);
    }
}

template <typename Tip1>
void merge_sort(Tip1 *niz, int vel)
{
    int indeks_prve_pozicije = 0;
    int indeks_zadnje_pozicije = vel-1;
    
    merge_sortiraj(niz, indeks_prve_pozicije, indeks_zadnje_pozicije);
}

template<typename Tip1>
int particija(Tip1* niz, int prvi, int zadnji)  //"prvi" i "zadnji" su indeksi elemenata u nizu
{
    Tip1 pivot = niz[prvi];
    int p = prvi+1;
    
    while(p <= zadnji && niz[p] < pivot)
        p++;
    
    for(int i=p+1; i<zadnji+1; i++)
    {
        if(niz[i] < pivot)
        {
            Tip1 tmp = niz[p];
            niz[p] = niz[i];
            niz[i] = tmp;
            
            p++;
        }
    }
    
    Tip1 tmp = niz[prvi];
    niz[prvi] = niz[p-1];
    niz[p-1] = tmp;
    
    return p-1;
}

template<typename Tip1>
void quick_sortiraj(Tip1* niz, int prvi, int zadnji)
{
    if(prvi<zadnji)
    {
        int j = particija(niz, prvi, zadnji);
        
        quick_sortiraj(niz, prvi, j-1);
        quick_sortiraj(niz, j+1, zadnji);
    }
}

template <typename Tip1>
void quick_sort(Tip1 *niz, int vel)
{
    quick_sortiraj(niz, 0, vel-1);
}

void ucitaj(std::string filename, int*& niz, int &vel)
{
    std::ifstream citaj_iz_dat(filename);
    int count=0;
    
    if(!citaj_iz_dat)
    std::cout<<"Otvaranje datoteke nije uspjelo!"<<std::endl;
    else
    {
        int br1;
        
        while(true)
        {
            citaj_iz_dat>> br1;          //>> razmak;
        
            if(!citaj_iz_dat)
            break;
            
            count++;
        }
    }
    vel=count;
    
    citaj_iz_dat.close();
    
    niz = new int [vel];
    
    citaj_iz_dat.open(filename);
    
    if(!citaj_iz_dat)
    std::cout<<"Otvaranje datoteke nije uspjelo!"<<std::endl;
    else
    {
        int br1, brojac=0;
        
        while(true)
        {
            citaj_iz_dat>> br1;;
        
            if(!citaj_iz_dat)
            break;
            
            niz[brojac] = br1;
            
            brojac++;
        }
    }
}
/*The numbers generated by the "rand()"" are not random because it generates the same sequence each time
  the code executed. So, if we run the code again, we'll get the same sequence repeated as in the previous run.*/
/*We will get the diffent sequence of numbers by using the srand(), however, the sequence will still be repeated each 
  time the code is executed. To generate a ever changing sequence, we need to feed something other than static integer
  to the argument of the srand() function.*/
/*The best solution to get different numbers by using "rand" function is by using the current time as the argument to 
  "srand()"", by calling time() function from the standard C++ library, <ctime>. This returns the time as the type of 
  time_t. Then, for portability, we cast as an integer type:*/

void generisi(std::string filename, int vel)
{
    std::ofstream upisi_u_datoteku(filename);
    
    srand((int)time(0));
    
    for(int i=0; i<vel; i++)
    {
        upisi_u_datoteku<< rand()%100 <<" ";
    }
}

void ispisiNiz(int *niz, int vel)
{
    
    for(int i=0; i<vel; i++)
    std::cout<<niz[i]<<" ";
    
    std::cout<<std::endl;
}

void funkcija()
{
    int *niz= new int [10];
    int vel=10;
    
    generisi("proba.txt", vel);
    ucitaj("proba.txt", niz, vel);
    ispisiNiz(niz, vel);
    
    std::cout<<"Unesite: 1-bubble_sort, 2-selection_sort, 3-quick_sort, 4-merge_sort"<<std::endl;
    
    int izbor;
    
    std::cin>>izbor;
    
    while(izbor!=1 && izbor!=2 && izbor!=3 && izbor!=4)
    {
        std::cout<<"Unijeli ste pogresnu komandu, unesite ponovno!"<<std::endl;
        std::cin>>izbor;
    }
    
    clock_t start, end;
    
    if(izbor==1)
    {
        start = clock();
        bubble_sort(niz, vel);
        end = clock();
    }
    else if(izbor==2)
    {
        start = clock();
        selection_sort(niz, vel);
        end = clock();
    }
    else if(izbor==3)
    {
        start = clock();
        quick_sort(niz, vel);
        end = clock();
    }
    else
    {
        start = clock();
        merge_sort(niz, vel);
        end = clock();
    }
    ispisiNiz(niz, vel);
    
    double duration_sec = double(end-start); 
    std::cout<<"sortiranje je trajalo "<<duration_sec<<" ms"<<std::endl;
    
    bool tacno=true;
    for(int i=0; i<vel; i++)
    {
        if(i+1<vel && niz[i]>niz[i+1])
        {
            tacno=false;
            break;
        }
    }
    
    if(tacno)
        std::cout<<"Tacno sortirano"<<std::endl;
    else
        std::cout<<"Netacno sortirano"<<std::endl;
    
    
    std::ofstream upisi_u_datoteku("nova.txt");
    for(int i=0; i<vel; i++)
    {
        upisi_u_datoteku<<niz[i]<<" ";
    }
    
    delete[]niz;
}

void izmjesaj(int *niz, int vel)
{
    for(int i=0; i<vel; i++)
    {
        niz[i]=vel-i;
    }
    
}
/////////////////////////////////////////////////////////////////////
bool Test1()
{
    int niz[10] = {9,8,7,6,5,4,3,2};
    int provjera[10] = {2,3,4,5,6,7,8,9};
    
    //izmjesaj(niz, 8);
    std::cout<<"prije bubble sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    bubble_sort(niz, 8);
    std::cout<<"poslije naprednog bubble sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    std::cout<<std::endl;
    
    bool tacno=true;
    for(int i=0; i<10; i++)
    {
        if(niz[i]!=provjera[i])
        {
            tacno=false;
            break;
        }
    }
    return tacno;
}

bool Test2()
{
    int niz[10] = {9,8,7,6,5,4,3,2};
    int provjera[10] = {2,3,4,5,6,7,8,9};
    
    //izmjesaj(niz, 8);
    std::cout<<"prije naprednog bubble sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    bubble_sort_upgrade(niz, 8);
    std::cout<<"poslije naprednog bubble sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    std::cout<<std::endl;
    
    bool tacno=true;
    for(int i=0; i<10; i++)
    {
        if(niz[i]!=provjera[i])
        {
            tacno=false;
            break;
        }
    }
    return tacno;
}

bool Test3()
{
    int niz[10] = {9,8,7,6,5,4,3,2};
    int provjera[10] = {2,3,4,5,6,7,8,9};
    
    //izmjesaj(niz, 8);
    std::cout<<"prije naprednog bubble sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    bubble_sort_upgrade(niz, 8);
    std::cout<<"poslije naprednog bubble sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    std::cout<<std::endl;
    
    bool tacno=true;
    for(int i=0; i<10; i++)
    {
        if(niz[i]!=provjera[i])
        {
            tacno=false;
            break;
        }
    }
    return tacno;
}

bool Test4()
{
    int niz[10] = {9,8,7,6,5,4,3,2};
    int provjera[10] = {2,3,4,5,6,7,8,9};
    
    //izmjesaj(niz, 8);
    std::cout<<"prije selection sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    selection_sort(niz, 8);
    std::cout<<"poslije selection sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    std::cout<<std::endl;
    
    bool tacno=true;
    for(int i=0; i<10; i++)
    {
        if(niz[i]!=provjera[i])
        {
            tacno=false;
            break;
        }
    }
    return tacno;
}

bool Test5()
{
    int niz[10] = {9,8,7,6,5,4,3,2};
    int provjera[10] = {2,3,4,5,6,7,8,9};
    
    //izmjesaj(niz, 8);
    std::cout<<"prije selection sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    selection_sort(niz, 8);
    std::cout<<"poslije selection sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    std::cout<<std::endl;
    
    bool tacno=true;
    for(int i=0; i<10; i++)
    {
        if(niz[i]!=provjera[i])
        {
            tacno=false;
            break;
        }
    }
    return tacno;
}

bool Test6()
{
    int niz[10] = {9,8,7,6,5,4,3,2};
    int provjera[10] = {2,3,4,5,6,7,8,9};
    
    //izmjesaj(niz, 8);
    std::cout<<"prije merge sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    merge_sort(niz, 8);
    std::cout<<"poslije merge sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    std::cout<<std::endl;
    
    bool tacno=true;
    for(int i=0; i<10; i++)
    {
        if(niz[i]!=provjera[i])
        {
            tacno=false;
            break;
        }
    }
    return tacno;
}

bool Test7()
{
    int niz[10] = {9,8,7,6,5,4,3,2};
    int pr[10] = {2,3,4,5,6,7,8,9};
    
    //izmjesaj(niz, 8);
    std::cout<<"prije quick sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    quick_sort(niz, 8);
    std::cout<<"poslije quick sorta: "<<std::endl;
    ispisiNiz(niz, 8);
    std::cout<<std::endl;
    
    bool tacno=true;
    for(int i=0; i<10; i++)
    {
        if(niz[i]!=pr[i])
        {
            tacno=false;
            break;
        }
    }
    return tacno;
}


int main()
{ 
    funkcija();
    
    if(Test1())
    std::cout<<"TACAN TEST 1"<<std::endl;
    else
    std::cout<<"NETACAN TEST 1"<<std::endl;
    
    if(Test2())
    std::cout<<"TACAN TEST 2"<<std::endl;
    else
    std::cout<<"NETACAN TEST 2"<<std::endl;
    
    if(Test3())
    std::cout<<"TACAN TEST 3"<<std::endl;
    else
    std::cout<<"NETACAN TEST 3"<<std::endl;
    
    if(Test4())
    std::cout<<"TACAN TEST 4"<<std::endl;
    else
    std::cout<<"NETACAN TEST 4"<<std::endl;
    
    if(Test5())
    std::cout<<"TACAN TEST 5"<<std::endl;
    else
    std::cout<<"NETACAN TEST 5"<<std::endl;
    
    if(Test6())
    std::cout<<"TACAN TEST 6"<<std::endl;
    else
    std::cout<<"NETACAN TEST 6"<<std::endl;
    
    if(Test7())
    std::cout<<"TACAN TEST 7"<<std::endl;
    else
    std::cout<<"NETACAN TEST 7"<<std::endl;
    
        
    return 0;
}
