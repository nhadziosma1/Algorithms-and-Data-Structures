#include <iostream>

/*deque*/
template<typename Tip1>
class DvostraniRed 
{
    private:
    
    struct Cvor
    {
        Tip1 element;
        Cvor *sljed, *preth;
    };
    
    Cvor *p_poc;
    Cvor *p_kr;
    
    int duzina;  //u promjenjivu "d_tren" brojimo koliko ima brojeva od [0, tren] (ukljucujuci i cvor na koji ukazuje pok "p_tren")
    
    public:
    DvostraniRed();
    ~DvostraniRed();
    DvostraniRed(const DvostraniRed &el);
    DvostraniRed &operator=(const DvostraniRed &el);
    
    int brojElemenata() const;
  
    void staviNaVrh(const Tip1 &el) ;
    void staviNaCelo(const Tip1 &el);
    
    Tip1 skiniSaCela();
    Tip1 skiniSaVrha();
    
    Tip1 &celo();
    Tip1 &vrh();
    
    void brisi();
    
    Tip1 &operator[](int broj) const;
    Tip1 &operator[](int broj);
    
    
    void IspisiDvRed();
};

template<typename Tip1>
DvostraniRed<Tip1>::~DvostraniRed()
{
    Cvor *sljedeci;
    
    for(Cvor* pok=p_poc; pok!=nullptr; pok=sljedeci)
    {
        sljedeci=pok->sljed;  //mora se prvo preusmjeriti na sljedeci element promjenjiva sljedeci, pa tek onda brisati
        delete pok;
    }
}

template<typename Tip1>
DvostraniRed<Tip1>::DvostraniRed(const DvostraniRed<Tip1> &dv2)
{
    /* AKO VISE STVARI ALOCIRAS U KONSTRUKTORU, MORAS SE POBRINUTI STA CE SE DESITI AKO SE BACI IZUZETAK, JER SE KONSTRUKTOR POZIVA SAMO NAD OBJEKTIMA 
     SU U POTPUNOSTI STVORENI!!!!!!!!!!!!!!!*/
    duzina=0;
    p_poc=nullptr;
    p_kr=nullptr;
    
    try
    {
        Cvor *prethodni = nullptr;
        
        for(int i=0; i<dv2.duzina; i++)
        {
            //nadji element kojim zelis da inicijaliziras
            Tip1 trazeni;
            int brojac(0);
            for(Cvor *pok=dv2.p_poc; pok!=nullptr; pok=pok->sljed)
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
    
    duzina=dv2.duzina;
}

template<typename Tip1>
DvostraniRed<Tip1>::DvostraniRed()
{
    duzina=0;
    p_poc=nullptr;
    p_kr=nullptr;
}

template<typename Tip1>
DvostraniRed<Tip1> &DvostraniRed<Tip1>::operator=(const DvostraniRed<Tip1> &el)
{
   // 1) poksaj alocirati isti sadrzaj kao sto ima element "el" u neki novi prostor u memoriji
   DvostraniRed<Tip1> pom;
   try 
   {
        Cvor *prethodni=nullptr;
        for(int i=0; i<el.duzina; i++)
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
        pom.p_kr=prethodni; //jer na kraju petlje prethodni, i novi pokazuju na zadnji element, ali 
                            //novi je lokalna promjenjiva
   }
   catch (...) 
   {
       //ako alokacija ne uspije gore, ovdje se ova fija zavrsava, a destruktor oslobadja prostor zauzeti,
       //pa je moglo i bez try-catch bloka!!
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
    
    duzina=el.duzina;
   
    return *this;
}

template<typename Tip1>
int DvostraniRed<Tip1>::brojElemenata() const
{
    return duzina;
}

template<typename Tip1>
Tip1 &DvostraniRed<Tip1>::celo()
{
    if(duzina==0)
    throw std::domain_error ("Dostarni red je prazna");
    
    return p_poc->element;
}

template<typename Tip1>
Tip1 &DvostraniRed<Tip1>::vrh()
{
    if(duzina==0)
    throw std::domain_error ("Dvostrani red je prazna");
    
   return p_kr->element;
}

template<typename Tip1>
void DvostraniRed<Tip1>::brisi()
{
    Cvor *sljedeci;
    
    for(Cvor *pok=p_poc; pok!=nullptr; pok=sljedeci)
    {
        sljedeci=pok->sljed;
        delete pok;
    }
    
    duzina=0;
    p_poc=nullptr;
    p_kr=nullptr;
}

template<typename Tip1>
void DvostraniRed<Tip1>::staviNaVrh(const Tip1 &el)
{
    if(duzina==0)
    {
        Cvor *novi = new Cvor{el, nullptr, nullptr};    //el, sljed, preth
        p_poc=novi;
        p_kr=novi;
        
        novi=nullptr;
    }
    else
    {
        Cvor *novi= new Cvor{el, nullptr, p_kr};
        p_kr->sljed=novi;
        
        p_kr=novi;
        
        novi=nullptr;
    }
    
    duzina++;
}

template<typename Tip1>
Tip1 DvostraniRed<Tip1>::skiniSaVrha()
{
    if(duzina==0)
    throw std::domain_error("Dvostarni red je prazan");
    
    Tip1 sacuvaj = p_kr->element;
    
    if(duzina==1)
    {
        delete p_kr;
        p_kr=nullptr;    //jer ako ima samo jedan el. u redu, znaci da "p_poc" i "p_kr" pokazuju na istu stvar
        p_poc=nullptr;
    }
    else
    {
        p_kr=p_kr->preth;
        delete p_kr->sljed;
        p_kr->sljed=nullptr;
    }
    
    duzina--;
    
    return sacuvaj;
}

template <typename Tip1>
void DvostraniRed<Tip1>::staviNaCelo(const Tip1 &el)
{
    if(duzina==0)
    {
        Cvor *novi = new Cvor{el, nullptr, nullptr};
        p_poc=novi;
        p_kr=novi;
        
        novi=nullptr;
    }
    else
    {
        Cvor *novi= new Cvor{el, p_poc, nullptr};
        p_poc->preth=novi;
        
        p_poc=novi;
        
        novi=nullptr;
    }
    
    duzina++;
}

template<typename Tip1>
Tip1 DvostraniRed<Tip1>::skiniSaCela()
{
    if(duzina==0)
    throw std::domain_error("Dvostarni red je prazan");
    
    Tip1 sacuvaj = p_poc->element;
    
    if(duzina==1)
    {
        delete p_poc;
        p_poc=nullptr;
        p_kr=nullptr;
    }
    else
    {
        p_poc=p_poc->sljed;
        delete p_poc->preth;
        p_poc->preth=nullptr;
    }
    
    duzina--;
    
    return sacuvaj;
}


template<typename Tip1>
Tip1 &DvostraniRed<Tip1>::operator[](int broj) const 
{
    if(broj>=duzina || broj<0)
    throw std::domain_error("Unijeli ste nepostojeci indeks");
    
    Cvor *pok=p_poc;
    
    for(int i=0; i<broj; i++)
    pok=pok->sljed;
    
    return pok->element;
}

template<typename Tip1>
Tip1 &DvostraniRed<Tip1>::operator[](int broj)
{
    if(broj>=duzina || broj<0)
    throw std::domain_error("Unijeli ste nepostojeci indeks");
    
    Cvor *pok=p_poc;
    
    for(int i=0; i<broj; i++)
    pok=pok->sljed;
    
    return pok->element;
}

template<typename Tip1>
void DvostraniRed<Tip1>::IspisiDvRed()
{
    for(Cvor *pok=p_poc; pok!=nullptr; pok=pok->sljed)
    std::cout<<pok->element<<" ";
    
    std::cout<<"!!!!!!!!!!!!!!!!!"<<std::endl;
}

///////////////testovi
bool Test1() //za dodajIspred(), brojElemenata(), operator[](int), NizLista
{
    DvostraniRed<int> dv1;
    bool tacan=true;
    int niz_int[10]={0,1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    for(int i=0; i<10; i++)
        dv1.staviNaVrh(i);
    
    for(int i=0; i<dv1.brojElemenata(); i++)
    std::cout<<dv1[i]<<" ";
    
    if(dv1.brojElemenata()!=10)
    tacan=false;
    
    std::cout<<std::endl;
    
    int br_el = dv1.brojElemenata();
    for(int i=0; i<br_el; i++)
    {
         // 1 2 3 4 5 6 7 8 9 0
         
         if(dv1.skiniSaCela()!=niz_int[i])
         tacan=false;
    }
    
    return tacan;
}

bool Test2()  
{
    DvostraniRed<int> dv1;
    bool tacan=true;
    int niz_int[10]={9, 7, 5, 3, 1, 0, 2, 4, 6, 8};
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        dv1.staviNaVrh(i);
        else
        dv1.staviNaCelo(i);
    }
    
    
    int br_el = dv1.brojElemenata();
    for(int i=0; i<br_el; i++)
    {
        //2 4 6 8 0 9 7 5 3 1
         
        if(dv1.skiniSaCela()!=niz_int[i])
        tacan=false;
    }
    
    return tacan;
}

bool Test3()
{
    DvostraniRed<int> dv1;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
        dv1.staviNaVrh(i);
    
    if(dv1.skiniSaCela()!=0 || dv1.skiniSaVrha()!=9)
    tacan=false;
    
    return tacan;
}

bool Test4()
{
    DvostraniRed<int> dv1;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        dv1.staviNaVrh(i);
        else
        dv1.staviNaCelo(i);
    }
    
    if(dv1.skiniSaVrha()!=8 || dv1.skiniSaCela()!=9)
    tacan=false;
    
    return tacan;
}

bool Test5()
{
    DvostraniRed<int> dv1;
    int niz_int[10]={9, 7, 5, 3, 1, 0, 2, 4, 6, 8};
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        dv1.staviNaCelo(i);
        else
        dv1.staviNaVrh(i);
    }//8 6 4 2 0 1 3 5 7 9 
    
    DvostraniRed<int> dv2;
    
    for(int i=0; i<11; i++)
        dv2.staviNaVrh(i);
        
    std::cout<<"Clanovi nl2 prije upotrebe operatora dodjele: "<<std::endl;
    for(int i=0; i<dv2.brojElemenata(); i++)
        std::cout<<dv2[i]<<" "; //0 1 2 3 4 5 6 7 8 9 10
    std::cout<<std::endl;
    
    if(dv2.brojElemenata()==dv1.brojElemenata())
    tacan=false;
    
    dv2=dv1;
    
    std::cout<<"Nakon kopirajuceg operatora dodjele nl2 ima elemente: "<<std::endl;
    int br_el2 =dv2.brojElemenata();
    for(int i=0; i<br_el2; i++)
    {
         // dv2: 8 6 4 2 0 1 3 5 7 9 
         
         if(dv2.skiniSaVrha()!=niz_int[i])
         tacan=false;
    }
    std::cout<<std::endl;
    
    dv2=dv1;
    
    std::cout<<"Dokaz da kopirajuci operator dodjele pravi duboku kopiju objekta koji se nalazi s desne strane dodjele: "<<std::endl;
    dv1.brisi();
    
    std::cout<<"Izgled dv1 nakon brisanja: "<<std::endl;
    for(int i=0; i<dv1.brojElemenata(); i++)
        std::cout<<dv1[i]<<" "; //nista
    std::cout<<std::endl;
    
    if(dv1.brojElemenata()!=0)
    tacan=false;
    
    std::cout<<"Izgled dv2 nakon brisanja dvl: "<<std::endl;
    
    int br_el3 = dv2.brojElemenata();
    for(int i=0; i<br_el3; i++)
    {
        // dv2 8 6 4 2 0 1 3 5 7 9 
         
        if(dv2.skiniSaVrha()!=niz_int[i])
        tacan=false;
    }
    std::cout<<std::endl;
    
    return tacan;
}

bool Test6()
{
    DvostraniRed<int> dv1;
    bool tacan=true;
    int niz_int[10]={8, 6, 4, 2, 0, 1, 3, 5, 7, 9};
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        dv1.staviNaCelo(i);
        else
        dv1.staviNaVrh(i);
    }//8,6,4,2,0,1,3,5,7,9
    
    DvostraniRed<int> dv2(dv1);
    
    std::cout<<"Nakon kopirajuceg konstruktora dv2 ima elemente: "<<std::endl;
    for(int i=0; i<dv2.brojElemenata(); i++)
        std::cout<<dv2[i]<<" "; //2 4 6 8 0 9 7 5 3 1
    std::cout<<std::endl;
    
    std::cout<<"Dokaz da kopirajuci konstruktor pravi duboku kopiju objekta pomocu kojeg se vrsi inicijalizacija: "<<std::endl;
    dv1.brisi();
    
    if(dv2.brojElemenata()!=10 && dv1.brojElemenata()!=0)
    tacan=false;
    
    std::cout<<"Izgled dv1 nakon brisanja: "<<std::endl;
    for(int i=0; i<dv1.brojElemenata(); i++)
        std::cout<<dv1[i]<<" "; //nista
    std::cout<<std::endl;
    
    std::cout<<"Izgled dv2 nakon brisanja dv1: "<<std::endl;
    int br_el2=dv2.brojElemenata();
    for(int i=0; i<br_el2; i++)
    {
        if(dv2[i]!=niz_int[i])
        tacan=false;
    }
    std::cout<<std::endl;
    
    return tacan;
}

bool Test7()
{
    DvostraniRed<int> dv1;
    bool tacan=true;

    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        dv1.staviNaCelo(i);
        else
        dv1.staviNaVrh(i);
    }//8,6,4,2,0,1,3,5,7,9
    
    std::cout<<"dv1 prije brisanja elemenata: "<<std::endl;
    dv1.IspisiDvRed();
    
    dv1.brisi();
    
    if(dv1.brojElemenata()!=0)
    tacan=false;
    
    std::cout<<"Izgled dv1 nakon brisanja: "<<std::endl;
    for(int i=0; i<dv1.brojElemenata(); i++)
        std::cout<<dv1[i]<<" "; //nista
    std::cout<<std::endl;
    
    return tacan;
}

bool Test8()
{
    DvostraniRed<int> dv1;
    bool tacan=true;
    int niz_int[10]={9, 7, 5, 3, 1, 0, 2, 4, 6, 8};
    
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        dv1.staviNaVrh(i);
        else
        dv1.staviNaCelo(i);
    }//9,7,5,3,1,0,2,4,6,8
    
    std::cout<<"dv1 prije brisanja elemenata: "<<std::endl;
    dv1.IspisiDvRed();
    
    int br_el=dv1.brojElemenata();
    for(int i=0; i<br_el; i++)
    {
        if(dv1.skiniSaCela()!=niz_int[i])
        tacan=false;
    }
    
    if(dv1.brojElemenata()!=0)
    tacan=false;
    
    return tacan;
}

int main() 
{
     
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
