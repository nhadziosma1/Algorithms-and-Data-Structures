#include <iostream>
#include <vector>

using namespace std;

template<typename Tip1>
class Stek
{
    private:
    
    struct Cvor
    {
        Tip1 el;
        Cvor* veza;
    };
    
    Cvor* top;
    int velicina;
    
    public:
    
    ~Stek();
    Stek();
    Stek(const Stek &s2);
    Stek &operator=(const Stek &s2);
    
    void brisi();
    void stavi( const Tip1 &el);
    Tip1 skini();
    Tip1 &vrh();
    int brojElemenata();
    
    void Ispisi()
    {
        if(top==nullptr)
        std::cout<<"prazan je!"<<std::endl;
        
        for(Cvor *pok=top; pok!=nullptr; pok=pok->veza)
        {
            if(pok->veza!=nullptr)
            std::cout<<pok->el<<", ";
            else
            std::cout<<pok->el<<std::endl;
        }
    }
    
    void pretraga(Stek<std::vector<int>> &s, int trazeni);
};

template<typename Tip1>
Stek<Tip1>::~Stek()
{
    Cvor *sljedeci;
        
    for(Cvor* pok=top; pok!=nullptr; pok=sljedeci)
    {           
        sljedeci=pok->veza;
        delete pok;
    }
        
    top=nullptr;
    velicina=0;
}

template<typename Tip1>
Stek<Tip1>::Stek()
{
    top=nullptr;
    velicina=0;
}

template<typename Tip1>
Stek<Tip1>::Stek(const Stek &s2)
{
    top= nullptr;
    velicina=0;
    try
    {
        Cvor *kroz_s2=s2.top;
        
        Cvor *prethodni;
        for(int i=0; i<s2.velicina; i++)
        {
            Cvor *novi=new Cvor{kroz_s2->el, nullptr};
            
            if(top==nullptr)
            top=novi;
            else
            prethodni->veza=novi;
            
            prethodni=novi;
            
            kroz_s2=kroz_s2->veza;
        }
        
        velicina=s2.velicina;
    }
    catch(...)
    {
        Cvor *sljedeci;
        
        for(Cvor* pok=top; pok!=nullptr; pok=sljedeci)
        {
            sljedeci=pok->veza;
            delete pok;
        }
        
        top=nullptr;
        velicina=0;
        
        throw;
    }
}

template<typename Tip1>
Stek<Tip1> &Stek<Tip1>::operator=(const Stek &s2)
{
    Stek<Tip1> pomocni;
    try
    {
        Cvor *kroz_s2=s2.top;
        
        Cvor *prethodni;
        for(int i=0; i<s2.velicina; i++)
        {
            Cvor *novi = new Cvor{kroz_s2->el, nullptr};
            
            if(pomocni.top==nullptr)
            pomocni.top=novi;
            else
            prethodni->veza=novi;
            
            prethodni=novi;
            
            kroz_s2=kroz_s2->veza;
        }
        
        
        Cvor *sljedeci;
        for(Cvor* pok=top; pok!=nullptr; pok=sljedeci)
        {           
            sljedeci=pok->veza;
            delete pok;
        }
        
        top=pomocni.top;
        pomocni.top=nullptr;
        velicina=s2.velicina;
    }
    catch(...)
    {
        throw;
    }
}

template<typename Tip1>
void Stek<Tip1>::brisi()
{
    while(top!=nullptr && velicina!=0)
    {
        Cvor *prije_vrha = top->veza;
        
        delete top;
        
        top = prije_vrha;
        velicina--;
    }
    
    //std::cout<<"velicina nakon obrisi:"<<velicina<<std::endl;
}

template<typename Tip1>
Tip1 Stek<Tip1>::skini()
{
    if(top==nullptr && velicina==0)                
    throw std::domain_error("Stek je prazan!");
    
    //std::cout<<"velicina prije skini:"<<velicina<<std::endl;
    
    Tip1 pomocni = top->el;
    Cvor *prije_vrha = top->veza;
    
    delete top;
    
    top = prije_vrha;
    velicina--;
    
    //std::cout<<"velicina nakon skini:"<<velicina<<std::endl;
    
    return pomocni;
}

template<typename Tip1>
void Stek<Tip1>::stavi(const Tip1 &el)
{
    if(top==nullptr && velicina==0)
    {
        Cvor *novi = new Cvor{el,nullptr};
        velicina++;
        top = novi;
    }
    else
    {
        Cvor* novi = new Cvor{el, top};
        velicina++;
        top = novi;
    }
}

template<typename Tip1>
Tip1 &Stek<Tip1>::vrh()
{
    if(top==nullptr && velicina==0)
    throw std::domain_error("Stek je parazn");
    
    return top->el;
}

template<typename Tip1>
int Stek<Tip1>::brojElemenata()
{
    return velicina;
}

template <typename Tip1>
void Stek<Tip1>::pretraga(Stek<std::vector<int>> &s, int trazeni)
{
    Cvor *kroz_stek=s.top;
    bool nadjen=false;
    int indeks_u_vektoru, pregledano_vektora;
    
    for(int i=0; i<s.brojElemenata(); i++)
    {
        int indeks_dna=0;
        int indeks_vrha = kroz_stek->el.size()-1;
        
        while(indeks_vrha>=indeks_dna)
        {
            int indeks_srednjeg = (indeks_vrha+indeks_dna)/2;
            
            if(trazeni == kroz_stek->el.at(indeks_srednjeg) )
            {
                nadjen=true;
                indeks_u_vektoru = indeks_srednjeg;
                pregledano_vektora=i;
                
                break;
            }
            else if(trazeni > kroz_stek->el.at(indeks_srednjeg))
            {
                indeks_dna = indeks_srednjeg+1;
            }
            else if(trazeni < kroz_stek->el.at(indeks_srednjeg))
            {
                indeks_vrha = indeks_srednjeg-1;
            }
        }
        
        if(nadjen==true)
        break;
        
        kroz_stek = kroz_stek->veza;
    }
    
    if(nadjen==true)
    {
        std::cout<<indeks_u_vektoru<<" "<<pregledano_vektora<<std::endl;
    }
    else
    {
        std::cout<<"Nema elementa"<<std::endl;
    }
}

int BinarnaPretraga(std::vector<int> v, int trazeni)
{
    //vrati -1 ako nije nadjen clan, u suprotnom vrati njegovu poziciju u vektoru
    if(v.size()==0 || trazeni > v.at(v.size()-1) )
    return -1;
    
    int indeks_vrha = v.size()-1;
    int indeks_dna = 0;
    
    while(indeks_dna<=indeks_vrha)
    {
        int indeks_srednjeg = (indeks_dna+indeks_vrha)/2;
        
        if(trazeni == v.at(indeks_srednjeg) )
        {
            return indeks_srednjeg;
        }
        else if(trazeni > v.at(indeks_srednjeg))
        {
            indeks_dna=indeks_srednjeg+1;
        }
        else if (trazeni < v.at(indeks_srednjeg))
        {
            indeks_vrha=indeks_srednjeg-1;
        }
    }
    
    return -1;
}

void pretraga(Stek<std::vector<int>> &s, int trazeni)
{
    static int br_poziva=0;//staticka promjenjiva ce se alocirati u memoriji samo jednom na vrijednost 0,
                        //dok ce se svakom sljedecom neuspjelom pretragom uvecati za jedan, a ne opet postaviti na 0
    
    if(s.brojElemenata()==0)
    {
        std::cout<<"Nema elementa"<<std::endl;
        return;
    }
    
    int indeks_trazenog = BinarnaPretraga(s.vrh(), trazeni);
    
    if(indeks_trazenog!=-1)
    {
        std::cout<<indeks_trazenog<<" "<<s.brojElemenata()-1;
    }
    else 
    {
        std::vector<int> v = s.skini();
        
        br_poziva++;    
        pretraga(s, trazeni);
        
        s.stavi(v);
    }
}


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

void UbaciParneIzReda(Stek<int> &s, Red<int> &r)
{
    if(r.brojElemenata()==0)
    return;
        
    int br=r.skini();
    
    UbaciParneIzReda(s, r);
    
    if(br%2==0)
    s.stavi(br);
    
    if(br%2!=0)
    r.stavi(br);
}

void funckija_sa_parc(Stek<int> &s, Red<int> &r)
{
    if(s.brojElemenata()==0)
    {
        UbaciParneIzReda(s, r);
        return;
    }
    
    int br = s.skini();
    funckija_sa_parc(s, r);
    s.stavi(br);
}


int main()
{
    
    
    Stek<int> s;
    for(int i=2; i<7; i++)
    s.stavi(i);
    //6,5,4,3,2

    Red<int> r;
    for(int i=100; i<107; i++)
    r.stavi(i);
    //100,101,102,103,104,105,106
    
    funckija_sa_parc(s, r);
    s.Ispisi();
    return 0;
}