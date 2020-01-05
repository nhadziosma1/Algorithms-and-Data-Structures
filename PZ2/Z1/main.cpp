#include <iostream>
#include <vector>
#include <stdexcept>
#include <type_traits>

using namespace std;

template<typename Tip1>
class Lista
{
    //kopirajuci konst i oprator dodjele
    
    public:
    Lista(){};
    virtual ~Lista(){};
    
    virtual int brojElemenata() const =0;
    virtual Tip1 &trenutni() const =0;
    virtual bool prethodni() =0;
    virtual bool sljedeci() =0;
    
    virtual void pocetak() =0;
    virtual void kraj() =0;
    virtual void obrisi() =0;
    virtual void dodajIspred(const Tip1 &el) =0;
    virtual void dodajIza(const Tip1 &el) =0;
    
    virtual Tip1 &operator [](int indeks) const =0;
    virtual Tip1 &operator [](int indeks) =0;
};

//////////////////////////////////////////////////////////////////////////
template <typename Tip1> 
class NizLista : public Lista<Tip1>
{
    private:
    int trenutno_upisanih, trenutni_kapacitet, indeks_trenutnog;
    Tip1 *poc_niz; 
    
    public:
    NizLista();
    NizLista(const NizLista &el);
    ~NizLista();
    NizLista &operator=(const NizLista &el);
    
    int brojElemenata() const override;
    Tip1 &trenutni() const override;
    
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override;
    void kraj() override;
    void obrisi() override;
    
    void dodajIspred(const Tip1 &el) override;
    void dodajIza(const Tip1 &el) override;
    
    Tip1 &operator[](int broj) const override;
    Tip1 &operator[](int broj) override;
};

template<typename Tip1>
NizLista<Tip1>::~NizLista()
{
    delete[] poc_niz;
}

template<typename Tip1>
NizLista<Tip1>::NizLista(const NizLista &el)
{
    poc_niz=new Tip1[el.trenutni_kapacitet];
    trenutni_kapacitet=el.trenutni_kapacitet;
    
    for(int i=0; i<el.trenutno_upisanih; i++)
    poc_niz[i]=el.poc_niz[i];
    
    trenutno_upisanih=el.trenutno_upisanih;
    indeks_trenutnog=el.indeks_trenutnog;
}

template<typename Tip1>
NizLista<Tip1>::NizLista()
{
    trenutno_upisanih=0;
    trenutni_kapacitet=0;
    poc_niz=nullptr;  
}

template<typename Tip1>
NizLista<Tip1> &NizLista<Tip1>::operator=(const NizLista &el)
{
   Tip1 *pomocni;
   pomocni=new Tip1[el.trenutni_kapacitet];
   
   for(int i=0; i<el.trenutno_upisanih; i++)
   pomocni[i]=el.poc_niz[i];
   
   delete[] poc_niz;
   
   poc_niz=pomocni;
   //pomocni=nullptr
   
   trenutni_kapacitet=el.trenutni_kapacitet;
   trenutno_upisanih=el.trenutno_upisanih;
   indeks_trenutnog=el.indeks_trenutnog;
    
    return *this;
}

template<typename Tip1>
int NizLista<Tip1>::brojElemenata() const
{
    return trenutno_upisanih;
}

template<typename Tip1>
Tip1 &NizLista<Tip1>::trenutni() const
{
    if(trenutno_upisanih==0)
    throw std::domain_error ("Lista je prazna");
    
    return poc_niz[indeks_trenutnog];
}

template<typename Tip1>
bool NizLista<Tip1>::prethodni() 
{
    if(trenutno_upisanih==0)
    throw std::domain_error ("Lista je prazna");
    
    if(indeks_trenutnog-1>=0)
    {
        indeks_trenutnog--;
        return true;
    }
    else
    return false;
}

template<typename Tip1>
bool NizLista<Tip1>::sljedeci()
{
    if(trenutno_upisanih==0)
    throw std::domain_error ("Lista je prazna");
    
    if(indeks_trenutnog+1<trenutno_upisanih)
    {
        indeks_trenutnog++;
        return true;
    }
    else
    return false;
}

template<typename Tip1>
void NizLista<Tip1>::pocetak()
{
    if(trenutno_upisanih==0)
    throw std::domain_error ("Lista je prazna");
    
    indeks_trenutnog=0;
}

template<typename Tip1>
void NizLista<Tip1>::kraj()
{
    if(trenutno_upisanih==0)
    throw std::domain_error ("Lista je prazna");
    
    indeks_trenutnog=trenutno_upisanih-1;
}

template<typename Tip1>
void NizLista<Tip1>::obrisi()
{
    if(trenutno_upisanih==0)
    throw std::domain_error ("Lista je prazna");
    
    if(trenutno_upisanih == indeks_trenutnog+1)
    {
        indeks_trenutnog--;
    }
    else
    {
        for(int i=indeks_trenutnog; i<trenutno_upisanih; i++)
            poc_niz[i]=poc_niz[i+1];
    }
    
    trenutno_upisanih--;
}

template<typename Tip1>
void NizLista<Tip1>::dodajIspred(const Tip1 &el)
{
    Tip1 *pomocni;
    
    if(trenutno_upisanih==0 && poc_niz==nullptr)   //dodao ovo za nullptr da vidim hoce l radit
    {
        poc_niz=new Tip1[25];
        
        poc_niz[0]=el;
        
        trenutno_upisanih++;
        trenutni_kapacitet=25;
        
        indeks_trenutnog=0;
    }
    else if(trenutno_upisanih+1>=trenutni_kapacitet)
    {
        pomocni = new Tip1[trenutni_kapacitet+25];
        
        for(int i=0; i<indeks_trenutnog; i++)
        {
            pomocni[i]=poc_niz[i];
        }
        
        pomocni[indeks_trenutnog]=el;
        
        for(int i=indeks_trenutnog; i<trenutno_upisanih; i++)
        {
            pomocni[i+1]=poc_niz[i];
        }
        
        delete[] poc_niz;
        poc_niz=pomocni;
        pomocni=nullptr; //ovo cak ni ne treba, jer ce "pomocni" netsatti na kraju funkcije, a on je pokazivac na Tip, pa se poziva destruktor
        
        indeks_trenutnog++;
        
        trenutno_upisanih++;
        trenutni_kapacitet+=25;
        
    }
    else if(trenutno_upisanih+1<=trenutni_kapacitet)
    {
        for(int i=trenutno_upisanih-1; i>=indeks_trenutnog; i--)
        poc_niz[i+1]=poc_niz[i];
        
        poc_niz[indeks_trenutnog]=el;
        
        indeks_trenutnog++;
        trenutno_upisanih++;
    }
}

template<typename Tip1>
void NizLista<Tip1>::dodajIza(const Tip1 &el)
{
    Tip1 *pomocni;
    
    if(trenutno_upisanih==0 && poc_niz==nullptr)  //dodao ovo za nullptr da vidim hoce l radit
    {
        poc_niz=new Tip1[25];
        
        poc_niz[0]=el;
        
        trenutno_upisanih++;
        trenutni_kapacitet=25;
        
        indeks_trenutnog=0;
    }
    else if(trenutno_upisanih+1>trenutni_kapacitet)
    {
        pomocni= new Tip1[trenutni_kapacitet+25];
        
        for(int i=0; i<=indeks_trenutnog; i++)
        {
            pomocni[i]=poc_niz[i];
        }
        
        pomocni[indeks_trenutnog+1]=el;
        
        for(int i=indeks_trenutnog+1; i<trenutno_upisanih; i++)
        {
            pomocni[i+1]=poc_niz[i];
        }
        
        delete[] poc_niz;
        poc_niz=pomocni;
        pomocni=nullptr;
        
        trenutno_upisanih++;
        trenutni_kapacitet+=25;
        
    }
    else if(trenutno_upisanih+1<=trenutni_kapacitet)
    {
        for(int i=trenutno_upisanih-1; i>indeks_trenutnog; i--)
        poc_niz[i+1]=poc_niz[i];
        
        poc_niz[indeks_trenutnog+1]=el;
        
        trenutno_upisanih++;
    }
}

template<typename Tip1>
Tip1 &NizLista<Tip1>::operator[](int broj) const 
{
    if(broj>=trenutno_upisanih || broj<0)
    throw std::domain_error("Unijeli ste nepostojeci indeks");
    
    return  poc_niz[broj];
}

template<typename Tip1>
Tip1 &NizLista<Tip1>::operator[](int broj)
{
    if(broj>=trenutno_upisanih || broj<0)
    throw std::domain_error("Unijeli ste nepostojeci indeks");
    
    return  poc_niz[broj];
}
///////////////////////////////////////////
template<typename Tip1>
class JednostrukaLista : public Lista<Tip1>
{
    private:
    
    struct Cvor
    {
        Tip1 element;
        Cvor *veza;
    };
    
    Cvor *p_poc;
    Cvor *p_kr;
    Cvor *p_tren;
    
    int d_tren, l_tren;  //u promjenjivu "d_tren" brojimo koliko ima brojeva od [0, tren] (ukljucujuci 
                         //i cvor na koji ukazuje pok "p_tren")
    
    public:
    JednostrukaLista();
    ~JednostrukaLista();
    JednostrukaLista(const JednostrukaLista &el);
    JednostrukaLista &operator=(const JednostrukaLista &el);
    
    int brojElemenata() const override;
    Tip1 &trenutni() const override;
    
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override;
    void kraj() override;
    void obrisi() override;
    
    void dodajIspred(const Tip1 &el) override;
    void dodajIza(const Tip1 &el) override;
    
    Tip1 &operator[](int broj) const override;
    Tip1 &operator[](int broj) override;
};

template<typename Tip1>
JednostrukaLista<Tip1>::~JednostrukaLista()
{
    Cvor *sljedeci;
    
    for(Cvor* pok=p_poc; pok!=nullptr; pok=sljedeci)
    {
        sljedeci=pok->veza;
        delete pok;
    }
}

template<typename Tip1>
JednostrukaLista<Tip1>::JednostrukaLista(const JednostrukaLista<Tip1> &el)
{
    /* AKO VISE STVARI ALOCIRAS U KONSTRUKTORU, MORAS SE POBRINUTI STA CE SE DESITI AKO SE BACI IZUZETAK, JER SE KONSTRUKTOR POZIVA SAMO NAD OBJEKTIMA 
     SU U POTPUNOSTI STVORENI!!!!!!!!!!!!!!!*/
    d_tren=0;
    l_tren=0;
    p_poc=nullptr;
    p_tren=nullptr;
    p_kr=nullptr;
    
    try
    {
        Cvor *prethodni(nullptr);
        
        for(int i=0; i<el.d_tren+el.l_tren; i++)
        {
            //nadji element kojim zelis da inicijaliziras
            Tip1 trazeni;
            int brojac(0);
            for(Cvor *pok=el.p_poc; pok!=nullptr; pok=pok->veza)
            {
                if(brojac==i)
                {
                    trazeni=pok->element;
                    break;
                }
                brojac++;
            }
            
            Cvor *novi=new Cvor{trazeni, nullptr};
            
            if(p_poc==nullptr)
                p_poc=novi;
            else
                prethodni->veza=novi;
            
            prethodni=novi;
        }
        p_kr=prethodni; //jer na kraju petlje "prethodni", i "novi" pokazuju na zadnji element, ali "novi" je lokalna promjenjiva
    }
    catch(...)
    {
        //MORA SE MEMORIJA OVDJE OSLOBODITI, JER INACE CE JE NIKO NECE POBRISATI
        Cvor *sljedeci;
    
        for(Cvor* pok=p_poc; pok!=nullptr; pok=sljedeci)
        {
            sljedeci=pok->veza;
            delete pok;
        }
        
        p_poc=nullptr;
        p_kr=nullptr;
        
        throw;
    }
    
    if(p_poc==nullptr)
    p_tren=nullptr;
    
    int brojac(1);
    for(Cvor *pok=p_poc; pok!=nullptr; pok=pok->veza)
    {
        if(brojac==el.l_tren)
        {
            p_tren=pok;
            break;
        }
        
        brojac++;
    }
    
    d_tren=el.d_tren;
    l_tren=el.l_tren;
}

template<typename Tip1>
JednostrukaLista<Tip1>::JednostrukaLista()
{
    d_tren=0;
    l_tren=0;
    p_poc=nullptr;
    p_tren=nullptr;
    p_kr=nullptr;
}

template<typename Tip1>
JednostrukaLista<Tip1> &JednostrukaLista<Tip1>::operator=(const JednostrukaLista<Tip1> &el)
{
   // 1) poksaj alocirati isti sadrzaj kao sto ima element "el" u neki novi prostor u memoriji
   JednostrukaLista<Tip1> pom;
   try 
   {
        Cvor *prethodni(nullptr);
        for(int i=0; i<el.d_tren+el.l_tren; i++)
        {
            //nadji element kojim zelis da inicijaliziras
            Tip1 trazeni;
            int brojac(0);
            for(Cvor *pok=el.p_poc; pok!=nullptr; pok=pok->veza)
            {
                if(brojac==i)
                {
                    trazeni=pok->element;
                    break;
                }
                brojac++;
            }
            
            Cvor *novi=new Cvor{trazeni, nullptr};
            
            if(pom.p_poc==nullptr)
                pom.p_poc=novi;
            else
                prethodni->veza=novi;
            
            prethodni=novi;
        }
        pom.p_kr=prethodni; //jer na kraju petlje prethodni, i novi pokazuju na zadnji element, ali novi je lokalna promjenjiva
   }
   catch (...) 
   {
       //ako alokacija ne uspije gore, ovdje se ova fija zavrsava, a destruktor oslobadja prostor zauzeti, pa je moglo i bez try-catch bloka!!
       throw;
   }    
    
    // 2) brisi ono na sta je operand s desen strane pokazivao
    Cvor *sljedeci;
    for(Cvor *pok=p_poc; pok!=nullptr; pok=sljedeci)
    {
        sljedeci=pok->veza;
        delete pok;
    }
    
    // 3) dodjeli novoalocirani prostor operadnu s lijeve strane jednakosti
    p_poc = pom.p_poc;
    pom.p_poc=nullptr;
    p_kr = pom.p_kr;
    pom.p_kr=nullptr;
    
    int brojac(1);
    p_tren = p_poc;
    while(brojac!=el.l_tren)
    {
        p_tren=p_tren->veza;
        brojac++;
    }
        
    d_tren=el.d_tren;
    l_tren=el.l_tren;
   
    return *this;
}

template<typename Tip1>
int JednostrukaLista<Tip1>::brojElemenata() const
{
    return d_tren+l_tren;
}

template<typename Tip1>
Tip1 &JednostrukaLista<Tip1>::trenutni() const
{
    if(d_tren+l_tren==0 || l_tren==0)
    throw std::domain_error ("Lista je prazna");
    
    return p_tren->element;
}

template<typename Tip1>
bool JednostrukaLista<Tip1>::prethodni() 
{
    if(d_tren+l_tren==0)
    throw std::domain_error ("Lista je prazna");
    
    if(l_tren>1)                   //mora imati svor iza trenutnog, tj. da "p_tren" ne bude nullprt, da bi ga pomakao nazad
    {
        Cvor *pok=p_poc;
            
        while(pok->veza!=p_tren)
            pok=pok->veza;
            
        p_tren=pok;
        
        
        d_tren++;
        l_tren--;
        
        return true;
    }
    else
    return false;
}

template<typename Tip1>
bool JednostrukaLista<Tip1>::sljedeci()
{
    if(d_tren+l_tren==0)
    throw std::domain_error ("Lista je prazna");
    
    if(d_tren!=0)                  //mora imat jedan cvor s desne strane da bi se pomakao na sljedeci
    {
        if(l_tren==0)
            p_tren=p_poc;
        else
            p_tren=p_tren->veza;
        
        d_tren--;
        l_tren++;
        
        return true;
    }
    else
    return false;
}

template<typename Tip1>
void JednostrukaLista<Tip1>::pocetak()
{
    if(d_tren+l_tren==0)
    throw std::domain_error ("Lista je prazna");
    
    
    d_tren=d_tren+l_tren-1;
    l_tren=1;
    
    p_tren=p_poc;
}

template<typename Tip1>
void JednostrukaLista<Tip1>::kraj()
{
    if(d_tren+l_tren==0)
    throw std::domain_error ("Lista je prazna");
    
    l_tren=d_tren+l_tren;
    d_tren=0;
    
    p_tren=p_kr;
}

template<typename Tip1>
void JednostrukaLista<Tip1>::obrisi()
{
    if(d_tren+l_tren==0 || l_tren==0)
    throw std::domain_error ("Lista je prazna");
    
    if(l_tren==1)           
    {
        Cvor *pomocni;
        
        pomocni=p_poc;
        
        p_poc=p_poc->veza;
        delete pomocni;
        
        p_tren=p_poc;  
        
        d_tren--;  
    }
    else if(d_tren==0)   
    {
        /*f(l_tren==1)
        {
            delete p_poc;
            l_tren--;
            return;
        }*/
        Cvor *prije_tr=p_poc;
        
        while(prije_tr->veza!=p_tren)
        prije_tr=prije_tr->veza;
        
        delete p_tren;
        //nisi promijenio pokazivac na kraj, on sada pokazuje na izbrisanu poziciju pa moze doci do krahiranja
        p_tren=prije_tr;
        
        p_kr=p_tren;
        p_tren->veza =nullptr;
        
        l_tren--;
    }
    else if(l_tren>1)
    {
        Cvor *prije_tr=p_poc;
        
        while(prije_tr->veza!=p_tren)
        prije_tr=prije_tr->veza;
        
        prije_tr->veza = p_tren->veza;
        delete p_tren;
        
        p_tren = prije_tr->veza; 
        
        d_tren--;
    }
}

template<typename Tip1>
void JednostrukaLista<Tip1>::dodajIspred(const Tip1 &el)
{
    if(d_tren+l_tren==0)
    {
        p_poc = new Cvor{el, nullptr};
        
        p_tren = p_poc;
        p_kr = p_poc;
    }
    else if(l_tren==0)
        throw std::domain_error ("Nema trenutnog.");
    else if (l_tren==1)                             //ekvivaletno uslovu  "p_tren!=nullptr && p_tren!=p_poc"
    {
        p_poc = new Cvor {el, p_poc};
    }
    else                                       
    {
        Cvor *prije_tr=p_poc;
        
        while(prije_tr->veza!=p_tren)
        prije_tr=prije_tr->veza;
        
        Cvor *novi = new Cvor {el, p_tren};
        
        prije_tr->veza = novi;
    }
    
    l_tren++;
}  

template<typename Tip1>
void JednostrukaLista<Tip1>::dodajIza(const Tip1 &el)
{
    if(d_tren+l_tren==0)
    {
        p_poc = new Cvor {el, nullptr};
        
        p_tren = p_poc;
        p_kr = p_poc;
        
        l_tren++;
    }
    else if(d_tren==0)
    {
        Cvor *novi = new Cvor {el, 0};
        p_tren->veza=novi;
        p_kr=novi;
        d_tren++;
    }
    else
    {
        Cvor *novi = new Cvor {el, p_tren->veza};
        p_tren->veza=novi;
        d_tren++;
    }
    
}

template<typename Tip1>
Tip1 &JednostrukaLista<Tip1>::operator[](int broj) const 
{
    if(broj>=(l_tren+d_tren) || broj<0)
    throw std::domain_error("Unijeli ste nepostojeci indeks");
    
    Cvor *pok=p_poc;
    
    for(int i=0; i<broj; i++)
    pok=pok->veza;
    
    return pok->element;
}

template<typename Tip1>
Tip1 &JednostrukaLista<Tip1>::operator[](int broj)
{
    if(broj>=(l_tren+d_tren) || broj<0)
    throw std::domain_error("Unijeli ste nepostojeci indeks");
    
    Cvor *pok=p_poc;
    
    for(int i=0; i<broj; i++)
    pok=pok->veza;
    
    return pok->element;
}

//////////////////////////////////////testovi

bool Test1() //za dodajIspred(), brojElemenata(), operator[](int), NizLista
{
    NizLista<int> nl;
    bool tacan=true;
    int niz_int[10]={1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    
    for(int i=0; i<10; i++)
        nl.dodajIspred(i);
    
    for(int i=0; i<nl.brojElemenata(); i++)
    std::cout<<nl[i]<<" ";
    
    if(nl.brojElemenata()!=10)
    tacan=false;
    
    std::cout<<std::endl;
    
    nl.pocetak();
    for(int i=0; i<nl.brojElemenata(); i++)
    {
         std::cout<<nl[i]<<" ";            // 1 2 3 4 5 6 7 8 9 0
         
         if(nl.trenutni()!=niz_int[i])
         tacan=false;
         
        nl.sljedeci();
    }
    
    return tacan;
}

bool Test2()  //za dodajIspred(), dodajIza(i), operator[](int), NizLista
{
    NizLista<int> nl;
    bool tacan=true;
    int niz_int[10]={2, 4, 6, 8, 0, 9, 7, 5, 3, 1};
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }
    
    for(int i=0; i<10; i++)
    std::cout<<nl[i]<<" ";
    
    std::cout<<std::endl;
    
    nl.pocetak();
    for(int i=0; i<nl.brojElemenata(); i++)
    {
         std::cout<<nl[i]<<" ";            //2 4 6 8 0 9 7 5 3 1
         
         if(nl.trenutni()!=niz_int[i])
         tacan=false;
         
        nl.sljedeci();
    }
    
    return tacan;
}

bool Test3()
{
    NizLista<int> nl;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
        nl.dodajIspred(i);
    
    int n=nl.brojElemenata();
    
    for(int i=0; i<n; i++)
    std::cout<<nl[i]<<" ";
    
    //1 2 3 4 5 6 7 8 9 0
    std::cout<<std::endl;
    
     std::cout<<"Trenutni prije upotrebe metoda pocetak() i kraj(): "<<nl.trenutni()<<std::endl; //0
    
    if(nl.trenutni()!=0)
    tacan=false;
    
    nl.pocetak();
    std::cout<<"pocetak: "<<nl.trenutni(); //1
    if(nl.trenutni()!=1)
    tacan=false;
    
    nl.kraj();
    std::cout<<", kraj: "<<nl.trenutni()<<std::endl; //0
    if(nl.trenutni()!=0)
    tacan=false;
    
    return tacan;
}

bool Test4()
{
    NizLista<int> nl;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }
    
    for(int i=0; i<10; i++)
    std::cout<<nl[i]<<" ";
    
    std::cout<<std::endl;
    //2 4 6 8 0 9 7 5 3 1
    
    std::cout<<"Trenutni prije upotrebe metoda pocetak() i kraj(): "<<nl.trenutni()<<std::endl; //0
    
    if(nl.trenutni()!=0)
    tacan=false;
    
    nl.pocetak();
    std::cout<<"pocetak: "<<nl.trenutni(); //2
    if(nl.trenutni()!=2)
    tacan=false;
    
    nl.kraj();
    std::cout<<", kraj: "<<nl.trenutni()<<std::endl; //1
    if(nl.trenutni()!=1)
    tacan=false;
    
    return tacan;
}

bool Test5()
{
    NizLista<int> nl;
    int niz_int[10]={2, 4, 6, 8, 0, 9, 7, 5, 3, 1};
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }//2 4 6 8 0 9 7 5 3 1
    
    NizLista<int> nl2;
    for(int i=0; i<10; i++)
        nl2.dodajIspred(i);
        
    std::cout<<"Clanovi nl2 prije upotrebe operatora dodjele: "<<std::endl;
    for(int i=0; i<nl2.brojElemenata(); i++)
        std::cout<<nl2[i]<<" "; // 1 2 3 4 5 6 7 8 9 0
    std::cout<<std::endl;
    
    nl2=nl;
    
    
    std::cout<<"Nakon kopirajuceg operatora dodjele nl2 ima elemente: "<<std::endl;
    nl2.pocetak();
    for(int i=0; i<nl2.brojElemenata(); i++)
    {
         std::cout<<nl2[i]<<" ";            //2 4 6 8 0 9 7 5 3 1
         
         if(nl2.trenutni()!=niz_int[i])
         tacan=false;
         
        nl2.sljedeci();
    }
    std::cout<<std::endl;
    
    std::cout<<"Dokaz da kopirajuci operator dodjele pravi duboku kopiju objekta koji se nalazi s desne strane dodjele: "<<std::endl;
    nl.pocetak();
    int brel=nl.brojElemenata();
    for(int i=0; i<brel; i++)
    nl.obrisi();
    
    std::cout<<"Izgled nl nakon brisanja: "<<std::endl;
    for(int i=0; i<nl.brojElemenata(); i++)
        std::cout<<nl[i]<<" "; //nista
    std::cout<<std::endl;
    
    if(nl.brojElemenata()!=0)
    tacan=false;
    
    std::cout<<"Izgled nl2 nakon brisanja nl: "<<std::endl;
    nl2.pocetak();
    for(int i=0; i<nl2.brojElemenata(); i++)
    {
         std::cout<<nl2[i]<<" ";            //2 4 6 8 0 9 7 5 3 1
         
         if(nl2.trenutni()!=niz_int[i])
         tacan=false;
         
        nl2.sljedeci();
    }
    std::cout<<std::endl;
    
    return tacan;
}

bool Test6()
{
    NizLista<int> nl;
    bool tacan=true;
    int niz_int[10]={2, 4, 6, 8, 0, 9, 7, 5, 3, 1};
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }//2 4 6 8 0 9 7 5 3 1
    
    NizLista<int> nl2(nl);
    
    std::cout<<"Nakon kopirajuceg konstruktora nl2 ima elemente: "<<std::endl;
    for(int i=0; i<nl2.brojElemenata(); i++)
        std::cout<<nl2[i]<<" "; //2 4 6 8 0 9 7 5 3 1
    std::cout<<std::endl;
    
    std::cout<<"Dokaz da kopirajuci konstruktor pravi duboku kopiju objekta pomocu kojeg se vrsi inicijalizacija: "<<std::endl;
    nl.pocetak();
    int brel=nl.brojElemenata();
    for(int i=0; i<brel; i++)
    nl.obrisi();
    
    if(nl2.brojElemenata()!=10 && nl.brojElemenata()!=0)
    tacan=false;
    
    std::cout<<"Izgled nl nakon brisanja: "<<std::endl;
    for(int i=0; i<nl.brojElemenata(); i++)
        std::cout<<nl[i]<<" "; //nista
    std::cout<<std::endl;
    
    std::cout<<"Izgled nl2 nakon brisanja nl: "<<std::endl;
    nl2.pocetak();
    for(int i=0; i<nl2.brojElemenata(); i++)
    {
         std::cout<<nl2[i]<<" ";
         
         if(nl2.trenutni()!=niz_int[i])
         tacan=false;
         
        nl2.sljedeci();
    }
    std::cout<<std::endl;
    
    return tacan;
}

bool Test7()
{
    NizLista<int> nl;
    bool tacan=true;

    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }//2 4 6 8 0 9 7 5 3 1
    
    std::cout<<"nl prije brisanja elemenata: "<<std::endl;
    for(int i=0; i<nl.brojElemenata(); i++)
        std::cout<<nl[i]<<" ";
    std::cout<<std::endl;
    
    nl.pocetak();
    int brel=nl.brojElemenata();
    for(int i=0; i<brel; i++)
    nl.obrisi();
    
    if(nl.brojElemenata()!=0)
    tacan=false;
    
    std::cout<<"Izgled nl nakon brisanja: "<<std::endl;
    for(int i=0; i<nl.brojElemenata(); i++)
        std::cout<<nl[i]<<" "; //nista
    std::cout<<std::endl;
    
    return tacan;
}

bool Test8()
{
    NizLista<int> nl;
    bool tacan=true;
    int niz_int[10]={2, 4, 6, 8, 0, 9, 7, 5, 3, 1};
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }//2 4 6 8 0 9 7 5 3 1
    
    
    std::cout<<"nl prije brisanja elemenata: "<<std::endl;
    for(int i=0; i<nl.brojElemenata(); i++)
        std::cout<<nl[i]<<" "; 
    std::cout<<std::endl;
    
    nl.pocetak();
    
    for(int i=0; i<nl.brojElemenata(); i++)
    {
         std::cout<<nl[i]<<" ";
         
         if(nl.trenutni()!=niz_int[i])
         tacan=false;
         
        nl.sljedeci();
    }
    std::cout<<std::endl;
    
    return tacan;
}
/////////////////////JEDNOSTRUKA LISTA
bool Test9() //za dodajIspred(), brojElemenata(), operator[](int), NizLista
{
    JednostrukaLista<int> nl;
    bool tacan=true;
    int niz_int[10]={1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    
    for(int i=0; i<10; i++)
        nl.dodajIspred(i);
    
    for(int i=0; i<nl.brojElemenata(); i++)
    std::cout<<nl[i]<<" ";
    
    if(nl.brojElemenata()!=10)
    tacan=false;
    
    std::cout<<std::endl;
    
    nl.pocetak();
    for(int i=0; i<nl.brojElemenata(); i++)
    {
         std::cout<<nl[i]<<" ";            // 1 2 3 4 5 6 7 8 9 0
         
         if(nl.trenutni()!=niz_int[i])
         tacan=false;
         
        nl.sljedeci();
    }
    
    return tacan;
}

bool Test10()  //za dodajIspred(), dodajIza(i), operator[](int), NizLista
{
    JednostrukaLista<int> nl;
    bool tacan=true;
    int niz_int[10]={2, 4, 6, 8, 0, 9, 7, 5, 3, 1};
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }
    
    for(int i=0; i<10; i++)
    std::cout<<nl[i]<<" ";
    
    std::cout<<std::endl;
    
    nl.pocetak();
    for(int i=0; i<nl.brojElemenata(); i++)
    {
         std::cout<<nl[i]<<" ";            //2 4 6 8 0 9 7 5 3 1
         
         if(nl.trenutni()!=niz_int[i])
         tacan=false;
         
        nl.sljedeci();
    }
    
    return tacan;
}

bool Test11()
{
    JednostrukaLista<int> nl;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
        nl.dodajIspred(i);
    
    for(int i=0; i<nl.brojElemenata(); i++)
    std::cout<<nl[i]<<" ";
    
    //1 2 3 4 5 6 7 8 9 0
    std::cout<<std::endl;
    
    std::cout<<"Trenutni prije upotrebe metoda pocetak() i kraj(): "<<nl.trenutni()<<std::endl; //0
    
    if(nl.trenutni()!=0)
    tacan=false;
    
    nl.pocetak();
    std::cout<<"pocetak: "<<nl.trenutni(); //1
    if(nl.trenutni()!=1)
    tacan=false;
    
    nl.kraj();
    std::cout<<", kraj: "<<nl.trenutni()<<std::endl; //0
    if(nl.trenutni()!=0)
    tacan=false;
    
    return tacan;
}

bool Test12()
{
    JednostrukaLista<int> nl;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }
    
    for(int i=0; i<10; i++)
    std::cout<<nl[i]<<" ";
    
    std::cout<<std::endl;
    //2 4 6 8 0 9 7 5 3 1
    
    std::cout<<"Trenutni prije upotrebe metoda pocetak() i kraj(): "<<nl.trenutni()<<std::endl; //0
    
    if(nl.trenutni()!=0)
    tacan=false;
    
    nl.pocetak();
    std::cout<<"pocetak: "<<nl.trenutni(); //2
    if(nl.trenutni()!=2)
    tacan=false;
    
    nl.kraj();
    std::cout<<", kraj: "<<nl.trenutni()<<std::endl; //1
    if(nl.trenutni()!=1)
    tacan=false;
    
    return tacan;
}

bool Test13()
{
    JednostrukaLista<int> nl;
    int niz_int[10]={2, 4, 6, 8, 0, 9, 7, 5, 3, 1};
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }//2 4 6 8 0 9 7 5 3 1
    
    JednostrukaLista<int> nl2;
    for(int i=0; i<10; i++)
        nl2.dodajIspred(i);
        
    std::cout<<"Clanovi nl2 prije upotrebe operatora dodjele: "<<std::endl;
    for(int i=0; i<nl2.brojElemenata(); i++)
        std::cout<<nl2[i]<<" "; // 1 2 3 4 5 6 7 8 9 0
    std::cout<<std::endl;
    
    nl2=nl;
    
    std::cout<<"Nakon kopirajuceg operatora dodjele nl2 ima elemente: "<<std::endl;
    nl2.pocetak();
    for(int i=0; i<nl2.brojElemenata(); i++)
    {
         std::cout<<nl2[i]<<" ";            //2 4 6 8 0 9 7 5 3 1
         
         if(nl2.trenutni()!=niz_int[i])
         tacan=false;
         
        nl2.sljedeci();
    }
    std::cout<<std::endl;
    
    std::cout<<"Dokaz da kopirajuci operator dodjele pravi duboku kopiju objekta koji se nalazi s desne strane dodjele: "<<std::endl;
    nl.pocetak();
    int brel=nl.brojElemenata();
    for(int i=0; i<brel; i++)
    nl.obrisi();
    
    std::cout<<"Izgled nl nakon brisanja: "<<std::endl;
    for(int i=0; i<nl.brojElemenata(); i++)
        std::cout<<nl[i]<<" "; //nista
    std::cout<<std::endl;
    
    if(nl.brojElemenata()!=0)
    tacan=false;
    
    std::cout<<"Izgled nl2 nakon brisanja nl: "<<std::endl;
    nl2.pocetak();
    for(int i=0; i<nl2.brojElemenata(); i++)
    {
         std::cout<<nl2[i]<<" ";            //2 4 6 8 0 9 7 5 3 1
         
         if(nl2.trenutni()!=niz_int[i])
         tacan=false;
         
        nl2.sljedeci();
    }
    std::cout<<std::endl;
    
    return tacan;
}

bool Test14()
{
    JednostrukaLista<int> nl;
    bool tacan=true;
    int niz_int[10]={2, 4, 6, 8, 0, 9, 7, 5, 3, 1};
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }//2 4 6 8 0 9 7 5 3 1
    
    JednostrukaLista<int> nl2(nl);
    
    std::cout<<"Nakon kopirajuceg konstruktora nl2 ima elemente: "<<std::endl;
    for(int i=0; i<nl2.brojElemenata(); i++)
        std::cout<<nl2[i]<<" "; //2 4 6 8 0 9 7 5 3 1
    std::cout<<std::endl;
    
    std::cout<<"Dokaz da kopirajuci konstruktor pravi duboku kopiju objekta pomocu kojeg se vrsi inicijalizacija: "<<std::endl;
    nl.pocetak();
    int brel=nl.brojElemenata();
    for(int i=0; i<brel; i++)
    nl.obrisi();
    
    if(nl2.brojElemenata()!=10 && nl.brojElemenata()!=0)
    tacan=false;
    
    std::cout<<"Izgled nl nakon brisanja: "<<std::endl;
    for(int i=0; i<nl.brojElemenata(); i++)
        std::cout<<nl[i]<<" "; //nista
    std::cout<<std::endl;
    
    std::cout<<"Izgled nl2 nakon brisanja nl: "<<std::endl;
    nl2.pocetak();
    for(int i=0; i<nl2.brojElemenata(); i++)
    {
         std::cout<<nl2[i]<<" ";
         
         if(nl2.trenutni()!=niz_int[i])
         tacan=false;
         
        nl2.sljedeci();
    }
    std::cout<<std::endl;
    
    return tacan;
}

bool Test15()
{
    JednostrukaLista<int> nl;
    bool tacan=true;

    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }//2 4 6 8 0 9 7 5 3 1
    
    std::cout<<"nl prije brisanja elemenata: "<<std::endl;
    for(int i=0; i<nl.brojElemenata(); i++)
        std::cout<<nl[i]<<" ";
    std::cout<<std::endl;
    
    nl.pocetak();
    int brel=nl.brojElemenata();
    for(int i=0; i<brel; i++)
    nl.obrisi();
    
    if(nl.brojElemenata()!=0)
    tacan=false;
    
    std::cout<<"Izgled nl nakon brisanja: "<<std::endl;
    for(int i=0; i<nl.brojElemenata(); i++)
        std::cout<<nl[i]<<" "; //nista
    std::cout<<std::endl;
    
    return tacan;
}

bool Test16()
{
    JednostrukaLista<int> nl;
    bool tacan=true;
    int niz_int[10]={2, 4, 6, 8, 0, 9, 7, 5, 3, 1};
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }//2 4 6 8 0 9 7 5 3 1
    
    std::cout<<"nl prije brisanja elemenata: "<<std::endl;
    for(int i=0; i<nl.brojElemenata(); i++)
        std::cout<<nl[i]<<" "; 
    std::cout<<std::endl;
    
    nl.pocetak();
    
    for(int i=0; i<nl.brojElemenata(); i++)
    {
         std::cout<<nl[i]<<" ";
         
         if(nl.trenutni()!=niz_int[i])
         tacan=false;
         
        nl.sljedeci();
    }
    std::cout<<std::endl;
    
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
    
    if(Test8())
    std::cout<<"TACAN TEST 8"<<std::endl;
    else
    std::cout<<"NETACAN TEST 8"<<std::endl;
    
    //JednostrukaLista
    if(Test9())
    std::cout<<"TACAN TEST 9"<<std::endl;
    else
    std::cout<<"NETACAN TEST 9"<<std::endl;
    
    if(Test10())
    std::cout<<"TACAN TEST 10"<<std::endl;
    else
    std::cout<<"NETACAN TEST 10"<<std::endl;
    
    if(Test11())
    std::cout<<"TACAN TEST 11"<<std::endl;
    else
    std::cout<<"NETACAN TEST 11"<<std::endl;
    
    if(Test12())
    std::cout<<"TACAN TEST 12"<<std::endl;
    else
    std::cout<<"NETACAN TEST 12"<<std::endl;
    
    if(Test13())
    std::cout<<"TACAN TEST 13"<<std::endl;
    else
    std::cout<<"NETACAN TEST 13"<<std::endl;
    
    if(Test14())
    std::cout<<"TACAN TEST 14"<<std::endl;
    else
    std::cout<<"NETACAN TEST 14"<<std::endl;
    
    if(Test15())
    std::cout<<"TACAN TEST 15"<<std::endl;
    else
    std::cout<<"NETACAN TEST 15"<<std::endl;
    
    if(Test16())
    std::cout<<"TACAN TEST 16"<<std::endl;
    else
    std::cout<<"NETACAN TEST 16"<<std::endl;

    return 0;
}

