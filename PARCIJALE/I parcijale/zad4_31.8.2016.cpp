#include <iostream>
#include <stdexcept>

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

void PrecbaciParneUJedanNeparneUdrugi(Red<int> &r1, Red<int> &r2)
{
    int vel1=r1.brojElemenata();
    for(int i=0; i<vel1; i++)
    {
        int br= r1.skini();
        
        if(br%2==0)
        r1.stavi(br);
        else
        r2.stavi(br);
    }
    
    int vel2=r2.brojElemenata();
    for(int i=0; i<vel2; i++)
    {
        int br2= r2.skini();
        
        if(br2%2==0)
        r1.stavi(br2);
        else
        r2.stavi(br2);
    }
    
    
    /*if(r1.brojElemenata()==0)
    return;
    
    int br1=r1.skini();
    int br2=r2.skini();
    
    PrecbaciParneUJedanNeparneUdrugi(r1, r2);
    
    if(br2%2==0)
    r1.stavi(br2);
    else
    r2.stavi(br2);
    
    if(br1%2==0)
    r2.stavi(br1);
    else
    r1.stavi(br1);*/
    
}

int main()
{
    Red<int> r1;
    r1.stavi(10);
    r1.stavi(8);
    r1.stavi(11);
    r1.stavi(3);
    r1.stavi(4);
    r1.stavi(2);
    r1.stavi(7);
    //std::cout<<r1.skini()<<r1.skini()<<std::endl;
    
    Red<int> r2;
    r1.stavi(9);
    r1.stavi(5);
    r1.stavi(3);
    r1.stavi(6);
    r1.stavi(4);
    r1.stavi(5);
    r1.stavi(1);
    
    PrecbaciParneUJedanNeparneUdrugi(r1, r2);
    
    int vel1=r1.brojElemenata();
    for(int i=0; i<vel1;i++)
    std::cout<<r1.skini()<<" ";
    
    std::cout<<std::endl;
    
    int vel2=r2.brojElemenata();
    for(int i=0; i<vel2;i++)
    std::cout<<r2.skini()<<" ";
    
    return 0;
}
