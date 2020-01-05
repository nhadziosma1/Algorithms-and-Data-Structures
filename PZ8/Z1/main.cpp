#include <iostream>

using namespace std;

template<typename TipKljuca, typename TipVrijednosti>
class Mapa
{
    public:
    virtual ~Mapa(){};
    
    virtual TipVrijednosti &operator[](TipKljuca tk) =0;
    virtual TipVrijednosti operator[](TipKljuca tk) const =0;
    virtual int brojElemenata() const=0;
    virtual void obrisi() =0;
    virtual void obrisi(const TipKljuca& k) =0;
};

template<typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti>
{
    private:
    std::pair<TipKljuca, TipVrijednosti> *niz_parova;
    int velicina, kapacitet;
    
    public:
    NizMapa();
    
    ~NizMapa();
    
    TipVrijednosti &operator[](TipKljuca tk) override;
    TipVrijednosti operator[](TipKljuca tk) const override;
    int brojElemenata() const override;
    void obrisi() override;
    void obrisi(const TipKljuca& k) override;
    
    void ubaci(std::pair<TipKljuca, TipVrijednosti> p, bool provjeri);
    void ispisi();
    
    NizMapa &operator=(const NizMapa &m2);
    NizMapa(const NizMapa &m2);
};

template<typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti> &NizMapa<TipKljuca, TipVrijednosti>::operator=(const NizMapa<TipKljuca, TipVrijednosti> &m2)
{
    try 
    {
        std::pair<TipKljuca, TipVrijednosti> *pomocni = new std::pair<TipKljuca, TipVrijednosti>[m2.kapacitet];
        
        for(int i=0; i<m2.velicina; i++)
            pomocni[i] = m2.niz_parova[i];

        velicina = m2.velicina;
        kapacitet = m2.kapacitet;
        
        delete [] niz_parova;
        niz_parova = pomocni;
        pomocni = nullptr;
    }
    catch (...)
    {
        std::cout << "izuzetak u konstruktoru" << std::endl;
    }
    
    return *this;
}

template<typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>::NizMapa(const NizMapa<TipKljuca, TipVrijednosti> &m2)
{
    niz_parova = new std::pair<TipKljuca, TipVrijednosti> [m2.kapacitet];
    
    for(int i=0; i<m2.velicina; i++)
    niz_parova[i] = m2.niz_parova[i];
    
    velicina = m2.velicina;
    kapacitet = m2.kapacitet;
}

template<typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>::NizMapa()
{
    niz_parova = nullptr; 
    velicina = 0;
    kapacitet = 0;
}

template<typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>::~NizMapa()
{
    velicina=0;
    delete[] niz_parova;
}

template<typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::obrisi()
{
    velicina=0;
    delete[] niz_parova;
    niz_parova = new pair<TipKljuca,TipVrijednosti>[kapacitet]; 
    // nakon brisanja treba da ostane isti alocirani prostor, ali bez ikakvih elemenata u njemu
}

template<typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &k)
{
    bool ima=false;
    for(int i=0; i<velicina; i++)
    {
        if(niz_parova[i].first == k)
        {
            for(int j=i; j<velicina; j++)
            {
                niz_parova[j] = niz_parova[j+1];
                //zadnji i predzadnji elemnt su nakon petlje jednaki, medjutim smanjivanjem atributa "velicina" to 'zataskavamo' od korisnika
            }
            ima=true;
            break;
        }
    }
    
    if(ima==false)
    throw std::domain_error("Ne postoji clan mape za unesenim kljucem");
    
    velicina--;
}

template<typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::ubaci(std::pair<TipKljuca, TipVrijednosti> p, bool provjeravaj)
{
    bool dodaj=true;
    
    if(provjeravaj==true)
    {
        for(int i=0; i<velicina; i++)
        {
            if(niz_parova[i].first == p.first && niz_parova[i].second == p.second)
            {
                dodaj=false;
                break;
            }
            else if(niz_parova[i].first == p.first && niz_parova[i].second != p.second)
            {
                niz_parova[i].second = p.second;
                dodaj=false;
                break;
            }
        }
    }
    
    if(dodaj==true)
    {
        if(kapacitet==0) //alociraj dodatni prostor
        {
            niz_parova = new std::pair<TipKljuca, TipVrijednosti> [25];
            
            kapacitet=25;
            niz_parova[0] = p;
        }
        else if(velicina+1 == kapacitet)
        {
                                                         //"new" vraca pokazivac na alocirani prostor
            std::pair<TipKljuca, TipVrijednosti> *pomocni = new std::pair<TipKljuca, TipVrijednosti> [kapacitet*2];
            
            kapacitet*=2;
            
            for(int i=0; i<velicina; i++)
                pomocni[i] = niz_parova[i];
            
            pomocni[velicina]=p;
            
            delete[] niz_parova;
            niz_parova = pomocni;
            pomocni=nullptr;
        }
        else
        {
            niz_parova[velicina] = p;
        }
        
        velicina++;
    }
    
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &NizMapa<TipKljuca, TipVrijednosti>::operator[] (TipKljuca tk)
{
    //pretraga je kompleksnosi O(n), dok je umetanje kompleksnosti O(1)
    
    for(int i=0; i<velicina; i++)
    {
        if(niz_parova[i].first == tk)
        return niz_parova[i].second;
    }
    
    std::pair<TipKljuca, TipVrijednosti> novi = std::make_pair(tk, TipVrijednosti());
    
    ubaci(novi, false);
    
    return niz_parova[velicina-1].second;
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti NizMapa<TipKljuca, TipVrijednosti>::operator[] (TipKljuca tk) const
{
    //pretraga je kompleksnosi O(n), dok je umetanje kompleksnosti O(1)
    
    for(int i=0; i<velicina; i++)
    {
        if(niz_parova[i].first == tk)
        return niz_parova[i].second;
    }
    
    /* S OBZIROM DA JE FUNCKIJA DEKLARISANA SA "...const" U NJON NE MOZEMO MIJENJATI SADRZAJ ATRIBUTA, PRI CEMU AKO POKUSAMO NAILAZIMO
       NA GRESKU, TE STOGA U KONSTANTNOJ VERZIJI OVE FUNKCIJE PRI PRETRAZI NECEMO MOCI UBACITI NOVI ELEMNT AKO SE ON NE NALAZI U ATRIBUTU
       
    std::pair<TipKljuca, TipVrijednosti> novi = std::make_pair(tk, TipVrijednosti() );
    
    ubaci(novi, false);*/
    
    return TipVrijednosti();
}

template<typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::ispisi()
{
    for(int i=0; i<velicina; i++)
        std::cout<<i+1<<". "<<niz_parova[i].first<<" "<<niz_parova[i].second<<"!"<<std::endl;
    
    std::cout<<std::endl;
}

template<typename TipKljuca, typename TipVrijednosti>
int NizMapa<TipKljuca, TipVrijednosti>::brojElemenata() const
{
    return velicina;
}
///////////////////////////////////////////////////////////////////

template<typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti>
{
    private: 
    
    struct Cvor
    {
        Cvor* pok_d;
        Cvor* pok_l;
        Cvor* pok_roditelj;
        
        TipKljuca c_kljuc;
        TipVrijednosti c_vrijednost;
    };
    
    int br_el;
    Cvor* korijen;
    
    
    void BrisiCijeloStablo(Cvor *pok);
    
    TipVrijednosti &Ubaci(const TipKljuca &k);
    
    void PovecajBrEl(){br_el++;}
    void kopiraj(BinStabloMapa &bs2, Cvor *p){
        if(p){
            bs2[p->c_kljuc] = p->c_vrijednost;
            kopiraj(bs2,p->pok_l);
            kopiraj(bs2,p->pok_d);
        }
    }
    public:
    
    ~BinStabloMapa();
    
    TipVrijednosti &operator[](TipKljuca tk);
    TipVrijednosti operator[](TipKljuca tk) const;
    int brojElemenata() const;
    void obrisi();
    void obrisi(const TipKljuca& k);
    
    BinStabloMapa &operator=(BinStabloMapa bs2);
    BinStabloMapa(const BinStabloMapa &bs2);
    BinStabloMapa();
};

template<typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>::~BinStabloMapa()
{
    BrisiCijeloStablo(korijen);
    
    br_el=0;
    korijen=nullptr;
}

template<typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa<TipKljuca, TipVrijednosti>::BrisiCijeloStablo(Cvor *pok)
{
    /*potreban uslov "pok!=nullptr", jer se u sljedecoj liniji pokusava dobiti dokuciti atribut "pok_d" tog
      pokazivaca, koji u slucaju da se radi o nullpointeru ne postoji, pa bi dovelo do greske*/
    if(pok!=nullptr)
    {
        BrisiCijeloStablo( pok->pok_d );
        BrisiCijeloStablo( pok->pok_l );
        delete pok;
    }
    
    pok=nullptr;
}

template<typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa<TipKljuca, TipVrijednosti>::obrisi()
{
    BrisiCijeloStablo(korijen);
    
    br_el=0;
    korijen=nullptr;
}

template<typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>::BinStabloMapa()
{
    korijen=nullptr;
    br_el=0;
}

/*template<typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>::BinStabloMapa(const BinStabloMapa &bs)
{
    
}

template<typename TipKljuca, typename TipVrijednosti>
BinStabloMapa &BinStabloMapa<TipKljuca, TipVrijednosti>::operator=(const BinStabloMapa &bs)
{
    
}*/

template<typename TipKljuca, typename TipVrijednosti>
int BinStabloMapa<TipKljuca, TipVrijednosti>::brojElemenata() const
{
    return br_el;
}

/*template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti BinStabloMapa<TipKljuca, TipVrijednosti>::Pretraga(Cvor* c, const TipKljuca& k) const
{
    //kljuc je jedinstven
    
    while(c!=nullptr && c->c_kljuc != k)
    {
        if(k < c->c_kljuc)
        c = c->pok_l;
        else
        c = c->pok_d;
    }
    
    if(c == nullptr)
    return TipVrijednosti();
    else
    return c->c_vrijednost;
    
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &BinStabloMapa<TipKljuca, TipVrijednosti>::Pretraga(Cvor* c, const TipKljuca& k)
{
    while(c!=nullptr && c->c_kljuc != k)
    {
        if(k < c->c_kljuc)
        c = c->pok_l;
        else
        c = c->pok_d;
    }
    
    if(c == nullptr)
    return TipVrijednosti();
    else
    return c->c_vrijednost;

}*/

template<typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca& k)
{
    Cvor *p = korijen;
    
    Cvor *roditelj= nullptr;
    
    while(p!=nullptr && k != p->c_kljuc)
    {
        roditelj=p;
        
        if(p->c_kljuc > k)
        p = p->pok_l;
        else
        p = p->pok_d;
    }
    
    if(p==nullptr)
    throw std::domain_error("Ne postoji clan u stablu s kljucem kojeg ste unijeli");
    
    Cvor *m = nullptr;
    Cvor *m_rod = nullptr;
    
    if(p->pok_l == nullptr)
    {
        /*cvor koji se brise nema lijevo dijete*/
        m = p->pok_d;
    }
    else
    {
        if(p->pok_d == nullptr)
        {
            /*cvor koji se brise nema desno dijete, a ima lijevo*/
            m = p->pok_l;
        }
        else
        {
            /*cvor koji se brise ima oba djeteta*/
            m_rod = p;
            m = p->pok_l;
            
            /*"tmp" promjenjiva sluzi samo za se provjeri da li postoji jos densne djece ui lijevom podstablu*/
            Cvor *tmp = m->pok_d;
            
            while(tmp!=nullptr)
            {
                m_rod = m;
                m = m->pok_d; //isto sto ti "m = tmp"
                
                tmp = m->pok_d;
            }
            
            /*ako cvor koji se brise nije roditelj najdesnijeg cvora iz svog lijevog podstabla (sto ce biti slucaj
              za kada god se petlja iznad izvrsi bar jendom), trebaju se */
            if(m_rod!=p)
            {
                m_rod->pok_d = m->pok_l;
                
                m->pok_l = p->pok_l;
            }
            //else
            m->pok_d = p->pok_d;
        }
    }
    
    /*preusmjeravanje pokazivaca na korijen, ukoliko je cvor koji se brise korijeni*/
    if(roditelj == nullptr)
    {
        korijen = m;
    }
    else
    {
        /*ako cvor koji se brise nije korijeni, potrbeno je preusmjeriti pokazivac koji je prije pokazivao na 
          cvor koji se brise, ali se prije toga mora ustanoviti da li je bio desno ili lijevo dijete*/
        if(p == roditelj->pok_l)
            roditelj->pok_l = m;
        else
            roditelj->pok_d = m;
    }
    
    delete p;
    br_el--;
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &BinStabloMapa<TipKljuca, TipVrijednosti>::Ubaci(const TipKljuca &k)
{
    /*KAD GOD SE UBACUJE ELEMENT, ON SE UBACUJE NA DNO STABLA*/
    
    Cvor *trazi = korijen;
    Cvor *rod_trazi = nullptr;
    
    /*nalazimo mjesto za element koji treba da se ubaci*/
    while(trazi!=nullptr)
    {
        rod_trazi = trazi;
        
        if( k > trazi->c_kljuc)
        trazi = trazi->pok_d;
        else
        trazi = trazi->pok_l;
    }
    /* "trazeni" je na kraju nullptr, a "rod_trazeni" pokazuje na roditelja koji treba preuzeti 
        element kao svoje dijete */
    
    Cvor *novi = new Cvor{nullptr, nullptr, nullptr, k, TipVrijednosti() };
    
    if( rod_trazi == nullptr)
    {
        korijen = novi;
    }
    else
    {
        if(k < rod_trazi->c_kljuc)
        rod_trazi->pok_l = novi;
        else
        rod_trazi->pok_d = novi;
    }
    
    br_el++;
    return novi->c_vrijednost;
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti BinStabloMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca k) const
{
    Cvor *c = korijen;
    
    while(c!=nullptr && c->c_kljuc != k)
    {
        if(k < c->c_kljuc)
        c = c->pok_l;
        else
        c = c->pok_d;
    }
    
    if(c == nullptr)
    return TipVrijednosti();
    else
    return c->c_vrijednost;
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &BinStabloMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca k) 
{
     Cvor *c = korijen;
    
    while(c!=nullptr && c->c_kljuc != k)
    {
        if(k < c->c_kljuc)
        c = c->pok_l;
        else
        c = c->pok_d;
    }
    
    if(c == nullptr)
    return Ubaci(k);
    else
    return c->c_vrijednost;
}

template<typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti> &BinStabloMapa<TipKljuca, TipVrijednosti>::operator=(BinStabloMapa bs2)
{
    swap(br_el,bs2.br_el);
    swap(korijen,bs2.korijen);
    return *this;
}

template<typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>::BinStabloMapa(const BinStabloMapa &bs2)
{
    korijen = 0;
    br_el = 0;
    kopiraj(*this,bs2.korijen);
}

int main() 
{   NizMapa <int,int> m;
    BinStabloMapa<int,int> b;
    for(int i=1;i<=1000;i++){
        m[rand()%i];
        b[rand()%i];
    }
    clock_t vrijeme1 = clock();
    m[55]=3;
    clock_t vrijeme2 = clock();
    clock_t vrijeme3 = clock();
    b[55]=3;
    clock_t vrijeme4 = clock();
    cout<<vrijeme2-vrijeme1<<" "<<vrijeme4-vrijeme3<<endl;
    /*Pretraga/dodavaje u BinStabloMapa je O( log(n) ) kompleknosti, dok je kod NizMapa kompleksnost O(n), 
    jer moramo proci kroz svaki clan da bismo nasli trazeni.
    Kod dodavanja komplleksnost je O(n^2) ukoliko zelimo NizMapu odrzavati u sortiranom poretku, dok ako ne zelimo, onda je kompleksnost 1.
    BinStabloMapa mnogo efikasnije, sto se vidi iz primjera.*/
    return 0;
}
