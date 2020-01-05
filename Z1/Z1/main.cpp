/*Zadatak 1. Implementirati klasu DvostrukaLista koja sadrži dinamički alociranu dvostruko
povezanu listu. Ova klasa treba biti naslijeđena iz abstraktne generičke klase Lista i potrebno je
implementirati sve metode (sve metode koje je trebalo implementirati i za klasu JednostrukaLista u
pripremnoj zadaći). Za svaku metodu klase je potrebno napisati funkciju koja će dokazati ispravnost
te metode.
Zadatak 2. Napraviti funkciju koja prima konstantnu referencu na klasu Lista a vraća njen najveći
član:
template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& n)
Funkcija treba da radi za klasu DvostrukaLista. Pri realizaciji ovog zadatka nije dozvoljeno uvoditi
nove metode odnosno mijenjati način rada postojećih funkcija. Potrebno je napisati funkciju koja će
dokazati ispravnost funkcije dajMaksimum.
Broj bodova će biti umanjen ukoliko postoje bitne greške u implementaciji kao što su: nije
korištena dinamička alokacija, DvostrukaLista implementirana preko niza umjesto preko strukture
Cvor, neke metode su neoptimalne (metoda je O(n) a trebala bi biti O(1)) itd. Moguće je da od
ukupnog broja bodova bude naknadno oduzet određeni dio bodova ukoliko funkcije ne posjeduju
potrebnu vremensku i memorijsku složenost, čak i ako prolaze svi testovi.*/

#include <iostream>
#include <vector>
#include <stdexcept>
#include <type_traits>

using namespace std;

template<typename Tip1>
class Iterator;  //predefinisana kalsa "Iterator"

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

template<typename Tip1>
class DvostrukaLista : public Lista<Tip1>
{
    private:
    
    struct Cvor
    {
        Tip1 element;
        Cvor *sljed, *preth;
    };
    
    Cvor *p_poc;
    Cvor *p_kr;
    Cvor *p_tren;
    
    int d_tren, l_tren;  //u promjenjivu "d_tren" brojimo koliko ima brojeva od [0, tren] (ukljucujuci i cvor na koji ukazuje pok "p_tren")
    
    public:
    DvostrukaLista();
    ~DvostrukaLista();
    DvostrukaLista(const DvostrukaLista &el);
    DvostrukaLista &operator=(const DvostrukaLista &el);
    
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
    
    friend class Iterator<Tip1>;
};

template<typename Tip1>
DvostrukaLista<Tip1>::~DvostrukaLista()
{
    Cvor *sljedeci;
    
    for(Cvor* pok=p_poc; pok!=nullptr; pok=sljedeci)
    {
        sljedeci=pok->sljed;
        delete pok;
    }
}

template<typename Tip1>
DvostrukaLista<Tip1>::DvostrukaLista(const DvostrukaLista<Tip1> &el)
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
        Cvor *prethodni = nullptr;
        
        for(int i=0; i<el.d_tren+el.l_tren; i++)
        {
            //nadji element kojim zelis da inicijaliziras
            Tip1 trazeni;
            int brojac(0);
            for(Cvor *pok=el.p_poc; pok!=nullptr; pok=pok->sljed)
            {
                if(brojac==i)
                {
                    trazeni = pok->element;
                    break;
                }
                brojac++;
            }
            
            Cvor *novi=new Cvor{trazeni, nullptr, nullptr};
            
            if(p_poc==nullptr)
                p_poc=novi;
            else
            {
                prethodni->sljed=novi;
                novi->preth=prethodni;
            }
            
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
            sljedeci=pok->sljed;
            delete pok;
        }
        
        p_poc=nullptr;
        p_kr=nullptr;
        
        throw;
    }
    
    if(p_poc==nullptr)
    p_tren=nullptr;
    
    int brojac(1);
    for(Cvor *pok=p_poc; pok!=nullptr; pok=pok->sljed)
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
DvostrukaLista<Tip1>::DvostrukaLista()
{
    d_tren=0;
    l_tren=0;
    p_poc=nullptr;
    p_tren=nullptr;
    p_kr=nullptr;
}

template<typename Tip1>
DvostrukaLista<Tip1> &DvostrukaLista<Tip1>::operator=(const DvostrukaLista<Tip1> &el)
{
   // 1) poksaj alocirati isti sadrzaj kao sto ima element "el" u neki novi prostor u memoriji
   DvostrukaLista<Tip1> pom;
   try 
   {
        Cvor *prethodni=nullptr;
        for(int i=0; i<el.d_tren+el.l_tren; i++)
        {
            //nadji element kojim zelis da inicijaliziras
            Tip1 trazeni;
            int brojac(0);
            for(Cvor *pok=el.p_poc; pok!=nullptr; pok=pok->sljed)
            {
                if(brojac==i)
                {
                    trazeni=pok->element;
                    break;
                }
                brojac++;
            }
            
            Cvor *novi=new Cvor{trazeni, nullptr, nullptr};
            
            if(pom.p_poc==nullptr)
                pom.p_poc=novi;
            else
            {
                prethodni->sljed=novi;
                novi->preth=prethodni;
            }
            
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
        sljedeci=pok->sljed;
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
        p_tren=p_tren->sljed;
        brojac++;
    }
        
    d_tren=el.d_tren;
    l_tren=el.l_tren;
   
    return *this;
}

template<typename Tip1>
int DvostrukaLista<Tip1>::brojElemenata() const
{
    return d_tren+l_tren;
}

template<typename Tip1>
Tip1 &DvostrukaLista<Tip1>::trenutni() const
{
    if(d_tren+l_tren==0 || l_tren==0)
    throw std::domain_error ("Lista je prazna");
    
    return p_tren->element;
}

template<typename Tip1>
bool DvostrukaLista<Tip1>::prethodni() 
{
    if(d_tren+l_tren==0)
    throw std::domain_error ("Lista je prazna");
    
    if(l_tren>1)                   //mora imati svor iza trenutnog, tj. da "p_tren" ne bude nullprt, da bi ga pomakao nazad
    {
        p_tren=p_tren->preth;
        
        d_tren++;
        l_tren--;
        
        return true;
    }
    else
    return false;
}

template<typename Tip1>
bool DvostrukaLista<Tip1>::sljedeci()
{
    if(d_tren+l_tren==0)
    throw std::domain_error ("Lista je prazna");
    
    if(d_tren!=0)                  //mora imat jedan cvor s desne strane da bi se pomakao na sljedeci
    {
        if(l_tren==0)
            p_tren=p_poc;
        else
            p_tren=p_tren->sljed;
        
        d_tren--;
        l_tren++;
        
        return true;
    }
    else
    return false;
}

template<typename Tip1>
void DvostrukaLista<Tip1>::pocetak()
{
    if(d_tren+l_tren==0)
    throw std::domain_error ("Lista je prazna");
    
    d_tren=d_tren+l_tren-1;
    l_tren=1;
    
    p_tren=p_poc;
}

template<typename Tip1>
void DvostrukaLista<Tip1>::kraj()
{
    if(d_tren+l_tren==0)
    throw std::domain_error ("Lista je prazna");
    
    l_tren=d_tren+l_tren;
    d_tren=0;
    
    p_tren=p_kr;
}

template<typename Tip1>
void DvostrukaLista<Tip1>::obrisi()
{
    if(d_tren+l_tren==0 || l_tren==0)
    throw std::domain_error ("Lista je prazna");
    
    if(l_tren==1)           
    {
        Cvor *pomocni = p_poc;
        
        p_poc=p_poc->sljed;
        delete pomocni;
        
        if(p_poc!=nullptr)
        p_poc->preth = nullptr;
        
        p_tren=p_poc;  
        
        d_tren--;  
    }
    else if(d_tren==0)   
    {
        Cvor *prije_tr = p_tren->preth;
        
        delete p_tren;
        
        p_tren=prije_tr;
        p_kr=p_tren;
        p_tren->sljed =nullptr;
        
        l_tren--;
    }
    else if(l_tren>1)
    {
        Cvor *prije_tr = p_tren->preth;
        
        prije_tr->sljed = p_tren->sljed;
        delete p_tren;
        
        p_tren = prije_tr->sljed;
        p_tren->preth = prije_tr;
        
        d_tren--;
    }
}

template<typename Tip1>
void DvostrukaLista<Tip1>::dodajIspred(const Tip1 &el)
{
    if(d_tren+l_tren==0)
    {
        p_poc = new Cvor{el, nullptr, nullptr};
        
        p_tren = p_poc;
        p_kr = p_poc;
    }
    else if(l_tren==0)
        throw std::domain_error ("Nema trenutnog.");
    else if (l_tren==1)                             //ekvivaletno uslovu  "p_tren!=nullptr && p_tren!=p_poc"
    {
        Cvor *novi = new Cvor {el, p_poc, nullptr};
        p_poc->preth = novi;
        p_poc = novi;
    }
    else                                       
    {
        Cvor *prije_tr=p_tren->preth;
        
        Cvor *novi = new Cvor {el, p_tren, p_tren->preth};
        
        prije_tr->sljed = novi;
        p_tren->preth = novi;
    }
    
    l_tren++;
}  

template<typename Tip1>
void DvostrukaLista<Tip1>::dodajIza(const Tip1 &el)
{
    if(d_tren+l_tren==0)
    {
        p_poc = new Cvor {el, nullptr, nullptr};
        
        p_tren = p_poc;
        p_kr = p_poc;
        
        l_tren++;
    }
    else if(d_tren==0)
    {
        Cvor *novi = new Cvor {el, nullptr, nullptr};
        p_tren->sljed = novi;
        novi->preth = p_tren;
        
        p_kr=novi;
        d_tren++;
    }
    else
    {
        Cvor *novi = new Cvor {el, p_tren->sljed, p_tren};
        p_tren->sljed = novi;
        
        Cvor *poslije_tren = p_tren->sljed;
        poslije_tren->preth=novi;
        
        d_tren++;
    }
    
}

template<typename Tip1>
Tip1 &DvostrukaLista<Tip1>::operator[](int broj) const 
{
    if(broj>=(l_tren+d_tren) || broj<0)
    throw std::domain_error("Unijeli ste nepostojeci indeks");
    
    Cvor *pok=p_poc;
    
    for(int i=0; i<broj; i++)
    pok=pok->sljed;
    
    return pok->element;
}

template<typename Tip1>
Tip1 &DvostrukaLista<Tip1>::operator[](int broj)
{
    if(broj>=(l_tren+d_tren) || broj<0)
    throw std::domain_error("Unijeli ste nepostojeci indeks");
    
    Cvor *pok=p_poc;
    
    for(int i=0; i<broj; i++)
    pok=pok->sljed;
    
    return pok->element;
}

/*Zadatak 2. Napraviti funkciju koja prima konstantnu referencu na klasu Lista, a vraća njen najveći član:

template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& n)

Funkcija treba da radi za klasu DvostrukaLista. Pri realizaciji ovog zadatka nije dozvoljeno uvoditi
nove metode, odnosno mijenjati način rada postojećih funkcija. Potrebno je napisati funkciju koja će
dokazati ispravnost funkcije dajMaksimum*/


template<typename Tip1>
class Iterator
{
    private:
    const DvostrukaLista<Tip1> *it_dvostr;
    typename DvostrukaLista<Tip1>::Cvor *tren_cvor;
    
    public:
    Iterator(const DvostrukaLista<Tip1> &dv);
    Tip1 &trenutni();
    void prethodni();
    void sljedeci();
    void pocetak();
    void kraj();
};

template<typename Tip1>
Iterator<Tip1>::Iterator(const DvostrukaLista<Tip1> &dv)
{
    it_dvostr=&dv;
    
    tren_cvor=dv.p_poc;
}

template<typename Tip1>
Tip1 &Iterator<Tip1>::trenutni()
{
    return tren_cvor->element;
}

template<typename Tip1>
void Iterator<Tip1>::prethodni()
{
     if(tren_cvor->preth != nullptr)
     tren_cvor=tren_cvor->preth;
}

template<typename Tip1>
void Iterator<Tip1>::sljedeci()
{
     if( tren_cvor->sljed != nullptr)
     tren_cvor=tren_cvor->sljed;
}

template<typename Tip1>
void Iterator<Tip1>::pocetak()
{
    tren_cvor = it_dvostr->p_poc;
}

template<typename Tip1>
void Iterator<Tip1>::kraj()
{
    tren_cvor= it_dvostr->p_kr;
}

template<typename Tip1>
Tip1 dajMaksimum(const Lista<Tip1> &n)
{
    Iterator<Tip1> it = dynamic_cast<const DvostrukaLista<Tip1> &> (n);
    
    Tip1 max{};
    
    for(int i=0; i<n.brojElemenata(); i++)
    {
        if(it.trenutni() > max)
        max=it.trenutni();
        
        it.sljedeci();
    }
    
    return max;
}


////////////////testovi
bool Test1() //za dodajIspred(), brojElemenata(), operator[](int), NizLista
{
    DvostrukaLista<int> nl;
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
    DvostrukaLista<int> nl;
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
    DvostrukaLista<int> nl;
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

bool Test4()
{
    DvostrukaLista<int> nl;
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
    DvostrukaLista<int> nl;
    int niz_int[10]={2, 4, 6, 8, 0, 9, 7, 5, 3, 1};
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }//2 4 6 8 0 9 7 5 3 1
    
    DvostrukaLista<int> nl2;
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
    DvostrukaLista<int> nl;
    bool tacan=true;
    int niz_int[10]={2, 4, 6, 8, 0, 9, 7, 5, 3, 1};
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        nl.dodajIspred(i);
        else
        nl.dodajIza(i);
    }//2 4 6 8 0 9 7 5 3 1
    
    DvostrukaLista<int> nl2(nl);
    
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
    DvostrukaLista<int> nl;
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
    DvostrukaLista<int> nl;
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
    

    return 0;
}
