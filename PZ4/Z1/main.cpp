#include <iostream>
#include <cmath>

template <typename Tip1>
class Red
{
    private:
    int indeks_pocetka;
    int indeks_kraja;
    Tip1* niz;
    int kapacitet;
    
    public:
    Red();
    Red(const Red &r);
    ~Red();
    Red &operator=(const Red &r);
    
    void brisi();
    void stavi(const Tip1 &el);
    Tip1 skini();
    Tip1 &celo();
    int brojElemenata();
    
    bool JeLiPun();
    bool JeLiPrazan();
};

template<typename Tip1>
Red<Tip1>::Red()
{
    indeks_pocetka=0;
    indeks_kraja=0;
    kapacitet=0;
    niz=nullptr;
}

template<typename Tip1>
Red<Tip1>::Red(const Red &r)
{
    niz = new Tip1[r.kapacitet];  //ako ne uspije ova alokacija, ne mora se nista ni dealocirati
    
    if( r.indeks_pocetka < r.indeks_kraja)
    {
        for(int i=r.indeks_pocetka; i<r.indeks_kraja; i++)
        niz[i]=r.niz[i];
    }
    else if(r.indeks_pocetka > r.indeks_kraja)
    {
        for(int i=r.indeks_pocetka; i<r.kapacitet; i++)
        niz[i] = r.niz[i];
        
        for(int i=0; i<r.indeks_kraja; i++)
        niz[i] = r.niz[i];
    }
    
    kapacitet = r.kapacitet;
    indeks_pocetka = r.indeks_pocetka;
    indeks_kraja = r.indeks_kraja;
}

template<typename Tip1>
Red<Tip1>::~Red()
{
    delete [] niz;
}

template<typename Tip1>
Red<Tip1> &Red<Tip1>::operator=(const Red &r)
{
    Tip1* pomocni= new Tip1[r.kapacitet];
    
    if(r.indeks_pocetka < r.indeks_kraja)
    {
        for(int i=r.indeks_pocetka; i<r.indeks_kraja; i++)
        pomocni[i]=r.niz[i];
    }
    else if (r.indeks_pocetka > r.indeks_kraja)
    {
        for(int i=r.indeks_pocetka; i<r.kapacitet; i++)
        pomocni[i]=r.niz[i];
        
        for(int i=0; i<r.indeks_kraja; i++)
        pomocni[i]=r.niz[i];
    }
    
    delete [] niz;
    
    niz=pomocni;
    
    indeks_pocetka=r.indeks_pocetka;
    indeks_kraja=r.indeks_kraja;
    kapacitet=r.kapacitet;
    
    return *this;
}

template<typename Tip1>
void Red<Tip1>::brisi()
{
   Tip1 *pok = new Tip1 [kapacitet];
   
   delete [] niz;
   
   niz=pok;
   pok=nullptr;
   
   indeks_pocetka=0;
   indeks_kraja=0;
}

template<typename Tip1>
int Red<Tip1>::brojElemenata()
{
    
    int udaljenost;
    
    if(JeLiPrazan())
    return 0;
    else if(indeks_pocetka<indeks_kraja)
    udaljenost = indeks_kraja-indeks_pocetka;
    else if(indeks_pocetka > indeks_kraja)
    udaljenost = (kapacitet-indeks_pocetka) + indeks_kraja;
    
    return udaljenost;
    
}

template<typename Tip1>
bool Red<Tip1>::JeLiPrazan()
{
    if(indeks_pocetka==indeks_kraja)
    return true;
    
    return false;
}

template<typename Tip1>
bool Red<Tip1>::JeLiPun()
{
    if( kapacitet==0 || indeks_pocetka == (indeks_kraja+1) % kapacitet) // morao se dodati prije uslov kapacitet==0, jer bi inace 
    {                                                                   // dijelili sa nulom, sto nije dozvoljeno
         return true;
    }
    else
    return false;
}

template<typename Tip1>
void Red<Tip1>::stavi(const Tip1 &el)
{
    //mora se realocirati prostor ako je pun
    if(JeLiPrazan())
    {
        if(kapacitet!=0)
        delete [] niz;  //AKO JE nullptr NECE IMATI NIKAKAVE STETE, AL MOZE SE DESITI DA JE PRAZAN, A DA IMA VECI KAPACITET 
        
        niz = new Tip1[20];
        kapacitet=20;
        
        niz[0]=el;
        indeks_pocetka=0;
        indeks_kraja=1;
    }
    else if( JeLiPun() )
    {
        Tip1 *realokacija = new Tip1[2*kapacitet];
       
        if(indeks_pocetka < indeks_kraja)
        {
            for(int i=indeks_pocetka; i<indeks_kraja; i++)
            realokacija[i] = niz[i];
        }
        else if(indeks_pocetka > indeks_kraja)
        {
            for(int i=indeks_pocetka; i<kapacitet; i++)
            realokacija[i] = niz[i];
            
            for(int i=0; i<indeks_kraja; i++)
            realokacija[ kapacitet+i] = niz[i];
        }
        
        delete [] niz;
        niz=realokacija;
        realokacija=nullptr;
        
        kapacitet = 2*kapacitet;
        indeks_kraja = indeks_pocetka + brojElemenata();
        
        niz[indeks_kraja]=el;
        indeks_kraja++;
    }
    else
    {
        niz[indeks_kraja]=el;
        
        indeks_kraja = (indeks_kraja+1)%kapacitet;    
    }
}

template<typename Tip1>
Tip1 &Red<Tip1>::celo()
{
    if(indeks_pocetka==indeks_kraja)
    throw std::domain_error("Red je prazan");
    
    return niz[indeks_pocetka];
}

template<typename Tip1>
Tip1 Red<Tip1>::skini()
{
    if(indeks_pocetka==indeks_kraja)
    throw std::domain_error("Red je prazan");
    
    Tip1 sacuvano_celo = niz[indeks_pocetka];
    
    indeks_pocetka = (indeks_pocetka+1)%kapacitet;    //prividno brisanje elementa iz reda, ostavreno pomjeranjem "indeks_pocetka" za jedan unaprijed
    
    return sacuvano_celo;
}

//////////////////////////testovi

bool Test1()
{
    Red<int> r;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    r.stavi(i);
    
    for(int i=0; i<5; i++)
    r.skini();
    
    for(int i=0; i<10; i++)
    r.stavi(i);
    
    int n=r.brojElemenata();
    if(n!=15)
    tacan=false;
    
    return tacan;
}

bool Test2()
{
    Red<int> r;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    r.stavi(i);
    
    int n=r.brojElemenata();
    if(n!=10)
    tacan=false;

    for(int i=0; i<n; i++)
    r.skini();
    
    if(r.brojElemenata()!=0)
    tacan=false;
    
    return tacan;
}

bool Test3()
{
    Red<int> r;
    bool tacan=true;
    
    for(int i=0; i<10000; i++)
    r.stavi(i);
        
     if(r.JeLiPun() || r.brojElemenata()!=10000)
     tacan=false;
    
    return tacan;
}

bool Test4()
{
    Red<int> r;
    bool tacan=true;
     
    if(r.JeLiPrazan()==false)
    tacan=false;
    
    return tacan;
}

bool Test5()
{
    Red<int> r1;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    r1.stavi(i);
    
    Red<int> r2(r1);
    
    if(r1.brojElemenata()!=r2.brojElemenata())
    tacan=false;
    
    int spasena_vel_red1=r1.brojElemenata(), spasena_vel_red2=r2.brojElemenata();

    for(int i=0; i<spasena_vel_red1; i++)
    r1.skini();
    
    if(r1.brojElemenata()==spasena_vel_red1)
    tacan=false;
    
    if(r2.brojElemenata()!=spasena_vel_red2)
    tacan=false;
    
    return tacan;
}

bool Test6()
{
    Red<int> r1;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    r1.stavi(i);
    
    Red<int> r2;
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        r2.stavi(i);
    }
    
    r2=r1;
    
    if(r1.brojElemenata()!=r2.brojElemenata())
    tacan=false;
    
    int spasena_vel_red1=r1.brojElemenata(), spasena_vel_red2=r2.brojElemenata();

    for(int i=0; i<spasena_vel_red1; i++)
    r1.skini();
    
    if(r1.brojElemenata()==spasena_vel_red1)
    tacan=false;
    
    if(r2.brojElemenata()!=spasena_vel_red2)
    tacan=false;
    
    return tacan;
}

bool Test7()
{
    Red<int> r;
    bool tacan=true;
     
    for(int i=0; i<1000; i++)
    r.stavi(i);
    
    r.brisi();
    
    if(r.brojElemenata()!=0)
    tacan= false;
    
    return tacan;
}

int main() 
{
    //NizLista   
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
