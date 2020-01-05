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
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti>
{
    private: 
    
    struct Cvor
    {
        Cvor* pok_d;
        Cvor* pok_l;
        Cvor* pok_roditelj;
        
        TipKljuca c_kljuc;
        TipVrijednosti c_vrijednost;
        
        int balans;
    };
    
    int br_el;
    Cvor* korijen;
    
    
    void BrisiCijeloStablo(Cvor *pok);
    
    TipVrijednosti &Ubaci(const TipKljuca &k);
    
    void PovecajBrEl(){br_el++;}
    public:
    
    ~AVLStabloMapa();
    
    TipVrijednosti &operator[](TipKljuca tk);
    TipVrijednosti operator[](TipKljuca tk) const;
    int brojElemenata() const;
    void obrisi();
    void obrisi(const TipKljuca& k);
    
    //AVLStabloMapa &operator=(const AVLStabloMapa &bs2);
    //AVLStabloMapa(const AVLStabloMapa &bs2);
    AVLStabloMapa();
    
    void azurirajBalans(Cvor *ubaceni);
    //void IskopirajSVE(Cvor *pok);
};

template<typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa<TipKljuca, TipVrijednosti>::~AVLStabloMapa()
{
    BrisiCijeloStablo(korijen);
    
    br_el=0;
    korijen=nullptr;
}

template<typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::BrisiCijeloStablo(Cvor *pok)
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
void AVLStabloMapa<TipKljuca, TipVrijednosti>::obrisi()
{
    BrisiCijeloStablo(korijen);
    
    br_el=0;
    korijen=nullptr;
}

template<typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa<TipKljuca, TipVrijednosti>::AVLStabloMapa()
{
    korijen=nullptr;
    br_el=0;
}

/*template<typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::IskopirajSVE(Cvor *pok)
{
    if(pok != nullptr)
    {
        Ubaci(pok->c_kljuc);
        
        IskopirajSVE(pok->pok_d);
        IskopirajSVE(pok->pok_l);
    }
}

template<typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa<TipKljuca, TipVrijednosti>::AVLStabloMapa(const AVLStabloMapa &avl_bs)
{
    IskopirajSVE(avl_bs.korijen);
}

template<typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa<TipKljuca, TipVrijednosti> &AVLStabloMapa<TipKljuca, TipVrijednosti>::operator=(const AVLStabloMapa &bs)
{
    IskopirajSVE(bs.korijen);
    
    return *this;
}*/

template<typename TipKljuca, typename TipVrijednosti>
int AVLStabloMapa<TipKljuca, TipVrijednosti>::brojElemenata() const
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
void AVLStabloMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca& k)
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

//aktivira se jedino u slucaju da je je novoumetnuti elemnt jedino dijete svog roditelja, a u slucaj da nije je ne treba vrsiti nikakvo azuriranje 
template<typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::azurirajBalans(Cvor *trenutni)
{
    
    while( trenutni->pok_roditelj != nullptr)
    {
        Cvor* A = trenutni;
        Cvor* B;
        Cvor* C;
        
        if(trenutni->balans == 2)
        {
            B = trenutni->pok_d;
            
            //uslov da naginju na istu stbranu je da balansirnaost kriticnog cvora i jegovog djeteta budu istog predznaka
            if(trenutni->pok_d->balans > 0)
            {
                //naginju na istu stranu, desnu
                B->pok_roditelj = A->pok_roditelj;
                
                //roditelj od A pokazuje na B
                if(A->pok_roditelj->pok_d == A)
                A->pok_roditelj->pok_d = B;
                else if(A->pok_roditelj->pok_l == A)
                A->pok_roditelj->pok_l = B;
                
                A->pok_roditelj = B;
                B->pok_l = A;
                
                
                A->pok_d = B->pok_l;
                B->pok_l->pok_roditelj = A;
                
                
                A->balans = 0;
                B->balans = 0;
            }
            else
            {
                //ne naginju na istu stranu, vec A na desno, a B na lijevo
                C = B->pok_l;
                
                C->pok_roditelj = B->pok_roditelj;
                
                if(C->pok_roditelj->pok_d == C)
                C->pok_roditelj->pok_d = B;
                else if(C->pok_roditelj->pok_l == C )
                C->pok_roditelj->pok_l = B;
    
    
                B->pok_roditelj = C;
                C->pok_d = B;
                
    
                B->pok_l = C->pok_d;
                C->pok_d->pok_roditelj = A; 
                
                
                C->balans = -2;
                B->balans = 0;
                
                //A-C-B
                C->pok_roditelj = A->pok_roditelj;
                
                if(A->pok_roditelj->pok_d == A)
                A->pok_roditelj->pok_d = C;
                else if(A->pok_roditelj->pok_l == A)
                A->pok_roditelj->pok_d = C;
                
                A->pok_roditelj = C;
                C->pok_l = A;
                
                A->pok_d = C->pok_l;
                C->pok_l->pok_roditelj = A;
                
            }
        }
        else if(trenutni->balans == -2)
        {
            B = trenutni->pok_l;
            
            //ako je kriticni cvor i njegovo dijete nageti na istu stranu (tj ako imaju balans istog predznaka)
            if(trenutni->pok_l->balans < 0)
            {
                //nageti na lijevu stranu, pa treba primijenti desnu rotaciju
                B->pok_roditelj = A->pok_roditelj;
                
                if(A->pok_roditelj->pok_d == A)
                A->pok_roditelj->pok_d = B;
                else if (A->pok_roditelj->pok_l == A)
                A->pok_roditelj->pok_l = B;
                
                A->pok_roditelj = B;
                B->pok_d = A;
                
                
                A->pok_l = B->pok_d;
                B->pok_d->pok_roditelj = A;
                
                A->balans = 0;
                B->balans = 0;
            }
            else
            {
                C = B->pok_d;
                
                A->pok_l = C;
                C->pok_roditelj = A;
                
                C->pok_l = B;
                B->pok_roditelj = C;
                B->pok_d=nullptr;
                
               //A-C-B
               A->pok_roditelj ->pok_l = C;
               C->pok_roditelj = A->pok_roditelj;
               
               C->pok_d = A;
               A->pok_roditelj = C;
               
               C->pok_l = B;
               B->pok_roditelj = C;
            }
        }
        
        
        if(trenutni->pok_roditelj ->pok_l == trenutni)
        trenutni->pok_roditelj->balans--;
        else if(trenutni->pok_roditelj ->pok_d == trenutni)
        trenutni->pok_roditelj->balans++;
        
        
        trenutni = trenutni->pok_roditelj;
    }
    
    
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &AVLStabloMapa<TipKljuca, TipVrijednosti>::Ubaci(const TipKljuca &k)
{
    /*KAD GOD SE UBACUJE ELEMENT, ON SE UBACUJE NA DNO STABLA*/
    
    Cvor *trazi = korijen;
    Cvor *rod_trazi = nullptr;
    
    /*nalazimo mjesto za element koji treba da se ubaci*/
    while(trazi != nullptr)
    {
        rod_trazi = trazi;
        
        if( k > trazi->c_kljuc)
        {
            trazi = trazi->pok_d;
        }
        else
        {
            trazi = trazi->pok_l;
        }
    }
    /* "trazeni" je na kraju nullptr, a "rod_trazeni" pokazuje na roditelja koji treba preuzeti 
        element kao svoje dijete */
    
    Cvor *novi = new Cvor{nullptr, nullptr, nullptr, k, TipVrijednosti(), 0 };
    
    bool mijenjaj = true;
    
    /*ako se radi o prvom cvoru u stablu*/
    if( rod_trazi == nullptr)
    {
        korijen = novi;
        mijenjaj = false;
    }
    else
    {
        if(k < rod_trazi->c_kljuc)
        {
            rod_trazi->pok_l = novi;
        }
        else
        {
            rod_trazi->pok_d = novi;
        }
    }
    
    br_el++;
    
    /*ako je poremecen balans moras azurirati stablo, pozivajuci "azurirajBalans(Cvor * ubaceni)" kojem se salje novoubaceni cvor*/
    if(mijenjaj == true)
    azurirajBalans(novi);
    
    return novi->c_vrijednost;
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti AVLStabloMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca k) const
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
TipVrijednosti &AVLStabloMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca k) 
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

int main() 
{
    // Da li je "AVLStabloMapa" ustvari obicno binarno stablo?
    AVLStabloMapa <int, int> m;
    int vel(500000);
    int progress(1000);
    for (int i(0); i<vel; i++) {
        m[i] = i;
        if (i==progress) { 
           cout<<"Dodano "<<i<<endl;
           progress *= 2;
        }
    }
    // Obicno bin. stablo ce uspjeti dodati oko 30000-70000 el. za 10 sekundi
    // Korektno AVL stablo ce zavrsiti kompletan test za <1 sekundu
    
    for (int i(0); i>=-vel; i--)
        m[i] = i;
    for (int i(-vel); i<vel; i++)
        if (m[i] != i) { cout << i << " NOT OK"; break; }
    for (int i(-vel); i<vel; i++)
        m.obrisi(i);
    cout << "OK";
    return 0;
}

