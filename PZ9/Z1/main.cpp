#include <iostream>
//#include <utility> //zbog tuple
#include <string>
//#include <vector>

using namespace std;

/*unsigned int djbhash(std::string ulaz, unsigned int max) 
{
	unsigned int suma=5381; 
// 5381 je pocetna vrijednost 
// koja poboljsava distribuciju
	for (int i(0); i<ulaz.length(); i++)
		suma = suma*33 + ulaz[i];
	return suma % max;
}*/

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
class HashMapa : public Mapa<TipKljuca, TipVrijednosti>
{
    //private:
    
    struct Tri
    {
        TipKljuca tri_k;
        TipVrijednosti tri_v;
        bool tri_b;
        
        Tri(TipKljuca k, TipVrijednosti v, bool b)
        {
            tri_k = k;
            tri_v = v;
            tri_b = b;
        }
    };
    
    Tri **niz_tri;
    int broj_upisanih, kapacitet;
    
    unsigned int (*pok_na_fun)(TipVrijednosti, unsigned int);
    
    public:
    HashMapa();
    
    ~HashMapa();
    
    TipVrijednosti &operator[](TipKljuca tk) override;
    TipVrijednosti operator[](TipKljuca tk) const override;
    int brojElemenata() const override;
    void obrisi() override;
    void obrisi(const TipKljuca& k) override;
    
    void ubaci( Tri t, bool provjeri);
    void ispisi();
    
    HashMapa &operator=(const HashMapa &m2);
    HashMapa(const HashMapa &m2);
    
    void definisiHashFunkciju(unsigned int (*p_n_fun)(TipKljuca, unsigned int));
    int hashTrazi(const TipKljuca &k);
    int hashUmetni(Tri *niz[], TipKljuca k, int niz_kap);
};

template<typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::HashMapa()
{
    niz_tri = nullptr; 
    broj_upisanih = 0;
    kapacitet = 0;
    pok_na_fun = nullptr;
}

template<typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::HashMapa(const HashMapa<TipKljuca, TipVrijednosti> &m2)
{
    niz_tri = new Tri *[m2.kapacitet]{};
    broj_upisanih = m2.broj_upisanih;
    kapacitet = m2.kapacitet;
    
    for(int i=0; i<m2.kapacitet; i++)
    {
        if(m2.niz_tri[i] != nullptr)
        {
            //1par-ovdje ubaci, 2par-ovaj kljuc ubaci, 3par- kapacitet onong u par1
            /*
                ZASTO OVO NE RADI??????????????????????????
            std::cout<< m2.niz_tri[i]->tri_k<<std::endl;   //OVDJEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
            int ovdje = hashUmetni(this->niz_tri, m2.niz_tri[i]->tri_k, this->kapacitet);
        
            if(ovdje != -1)
            niz_tri[ovdje] = new Tri (m2.niz_tri[i]->tri_k, m2.niz_tri[i]->tri_v, false);*/
            
            niz_tri[i] = new Tri (m2.niz_tri[i]->tri_k, m2.niz_tri[i]->tri_v, false);
        }
    }
    
    pok_na_fun = m2.pok_na_fun;
}

template<typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::~HashMapa()
{
    broj_upisanih=0;
    
    for(int i=0; i<kapacitet; i++)
    delete niz_tri[i];
    
    delete[] niz_tri;
    
    niz_tri = nullptr;
}

template<typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti> &HashMapa<TipKljuca, TipVrijednosti>::operator=(const HashMapa<TipKljuca, TipVrijednosti> &m2)
{
    try 
    {
        Tri **pomocni = new Tri *[m2.kapacitet]{};
        
        for(int i=0; i<m2.kapacitet; i++)
        {
            if(m2.niz_tri[i] != nullptr)
            {
                int ovdje = hashUmetni(pomocni, m2.niz_tri[i]->tri_k, m2.kapacitet);
                
                if(ovdje != -1)
                pomocni[ovdje] = new Tri (m2.niz_tri[i]->tri_k, m2.niz_tri[i]->tri_v, m2.niz_tri[i]->tri_b);   
                
            }
        }
        
        for(int i=0; i<kapacitet; i++)
        delete niz_tri[i];
        
        delete [] niz_tri;
        
        broj_upisanih = m2.broj_upisanih;
        kapacitet = m2.kapacitet;
        pok_na_fun = m2.pok_na_fun;
        
        niz_tri = pomocni;
        pomocni = nullptr;
    }
    catch (...)
    {
        std::cout << "izuzetak u konstruktoru" << std::endl;
    }
    
    return *this;
}

/*par1- gdje treba umetnut; par2- kljuc elemnta koji se treba umetnut*/
template<typename TipKljuca, typename TipVrijednosti>
int HashMapa<TipKljuca, TipVrijednosti>::hashUmetni(Tri **niz, TipKljuca k, int niz_kap)
{
    
    //std::cout<<k<<std::endl;
    unsigned int hes_pozicija = pok_na_fun(k, niz_kap);
        
    for(int j=hes_pozicija; j<niz_kap; j++)
    {
        if(niz[j] == nullptr || niz[j]->tri_b == true)
        return j;
    }
        
    for(int j=0; j<hes_pozicija; j++)
    {
        if( niz[j] == nullptr || niz[j]->tri_b == true)
        return j;
    }
    
    std::cout<<"Tabela je puna"<<std::endl;
    return -1;
}

//varca poziciju trazenog elemnta, cija pretraga se prekida kada se naidje na vec obrisani el, ili na praznu poziciju
template<typename TipKljuca, typename TipVrijednosti>
int HashMapa<TipKljuca, TipVrijednosti>::hashTrazi(const TipKljuca &k)
{
    if(kapacitet == 0)
    return -1;
    
    unsigned int hes_pozicija = pok_na_fun(k, kapacitet);
        
    for(int j=hes_pozicija; j<kapacitet; j++)
    {
        if( niz_tri[j] == nullptr || niz_tri[j]->tri_b == true)
        return -1;
            
        if( niz_tri[j] != nullptr && niz_tri[j]->tri_k == k)
        return j;
    }
        
    for(int j=0; j<hes_pozicija; j++)
    {
        if( niz_tri[j] == nullptr || niz_tri[j]->tri_b == true)
        return -1;
            
        if(niz_tri[j] != nullptr && niz_tri[j]->tri_k == k)
        return j;
    }
    
    return -1;
}

template<typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::obrisi()
{
    broj_upisanih = 0;
    
    for(int i=0; i<kapacitet; i++)
    delete niz_tri[i];
    
    for(int i=0; i<kapacitet; i++)
    niz_tri[i] = nullptr;
}

template<typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &k)
{
    bool ima=false;
    for(int i=0; i<kapacitet; i++)
    {
        if( niz_tri[i]!=nullptr && niz_tri[i]->tri_k == k)
        {
            
            niz_tri[i]->tri_b = true;
            
            ima=true;
            break;
        }
    }
    
    if(ima==false)
    throw std::domain_error("Ne postoji clan mape za unesenim kljucem");
    
    broj_upisanih--;
}

template<typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::definisiHashFunkciju( unsigned int (*pok_na_fun)(TipKljuca, unsigned int) )
{
    this->pok_na_fun = pok_na_fun;
}

template<typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::ubaci(Tri tu, bool provjeravaj)
{
    bool dodaj=true;
    
    if(provjeravaj == true)
    {
        if(hashTrazi(tu.tri_k) != -1)
        dodaj = false;
    }
    
    if(dodaj==true)
    {
        if(kapacitet==0) //alociraj dodatni prostor
        {
            this->niz_tri = new Tri*[25]{};
            
            this->kapacitet = 25;
            
            unsigned int pozicija = pok_na_fun(tu.tri_k, kapacitet);
            
            niz_tri[pozicija] = new Tri(tu.tri_k, tu.tri_v, false);
        }
        else if( this->broj_upisanih+1 >= (this->kapacitet*0.01) )
        {
            //ako se samo 1% popunjenost hesh tabele, alocira se novi prostor 
            
                                        //"new" vraca pokazivac na alocirani prostor
            Tri **pomocni = new Tri *[this->kapacitet*2]{};
            
            for(int i=0; i<this->kapacitet; i++)
            {
                int ovdje = -1;
                if( this->niz_tri[i] != nullptr)
                ovdje = hashUmetni(pomocni, this->niz_tri[i]->tri_k, this->kapacitet*2);
                
                if(ovdje != -1)
                {
                    //std::cout<<"nova pozicija: "<< ovdje<<", a kapacitet je"<<this->kapacitet*2<<" "<<niz_tri[i]->tri_k<<std::endl;
                    //u slucaju da de ovdje nalazio neki elemnt koji je 'izbrisan', tj. kojem je atribut "tri->b" bio true, mora se izbrisati prije ubacivanja novog el
                    delete pomocni[ovdje];
                    
                    pomocni[ovdje] = new Tri(niz_tri[i]->tri_k, niz_tri[i]->tri_v, niz_tri[i]->tri_b);
                }
            }
            
            int pozicija_novog = hashUmetni(pomocni, tu.tri_k, this->kapacitet*2);
            
            if(pozicija_novog != -1)
            {
                delete pomocni[pozicija_novog];
                
                pomocni[pozicija_novog] = new Tri(tu.tri_k, tu.tri_v, tu.tri_b);
            }
            else
                std::cout<<"NIJE SMIJELO DOCI DO OVDJE"<<std::endl;
            
            for(int i=0; i<this->kapacitet; i++)
            delete this->niz_tri[i];
            
            delete[] this->niz_tri;
            
            this->kapacitet*=2;
            this->niz_tri = pomocni;
            pomocni = nullptr;
        }
        else
        {
            int ovdje = hashUmetni(niz_tri, tu.tri_k, this->kapacitet);
            
            if(ovdje != -1)
            {
                delete niz_tri[ovdje];
                
                niz_tri[ovdje] = new Tri (tu.tri_k, tu.tri_v, tu.tri_b);
            }
            else
                std::cout<<"NIJE SMIJELO DOCI DO OVDJE"<<std::endl;
        }
        
        broj_upisanih++;
    }
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &HashMapa<TipKljuca, TipVrijednosti>::operator[] (TipKljuca k)
{
    //pretraga je kompleksnosi O(n), dok je umetanje kompleksnosti O(1)
    if(pok_na_fun == nullptr)
    throw std::domain_error("nije definisana hash funkcija!");
    
    int poz_nadjenog = hashTrazi(k);
    
    //ako ne nadje element koji ste zeljeli  
    if(poz_nadjenog == -1)
    {
        Tri t( k, TipVrijednosti(), false);
        
        ubaci(t , false);
        
        poz_nadjenog = hashTrazi(k);
    }
    
    return niz_tri[poz_nadjenog]->tri_v;
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti HashMapa<TipKljuca, TipVrijednosti>::operator[] (TipKljuca tk) const
{
    //pretraga je kompleksnosi O(n), dok je umetanje kompleksnosti O(1)
    
    int poz_nadjenog = hashTrazi(tk);
    
    if(poz_nadjenog != -1)
    return niz_tri[poz_nadjenog]->tri_v;
    
    // S OBZIROM DA JE FUNCKIJA DEKLARISANA SA "...const" U NJON NE MOZEMO MIJENJATI SADRZAJ ATRIBUTA, PRI CEMU AKO POKUSAMO NAILAZIMO
    // NA GRESKU, TE STOGA U KONSTANTNOJ VERZIJI OVE FUNKCIJE PRI PRETRAZI NECEMO MOCI UBACITI NOVI ELEMNT AKO SE ON NE NALAZI U ATRIBUTU
    //    std::pair<TipKljuca, TipVrijednosti> novi = std::make_pair(tk, TipVrijednosti() );
    //    ubaci(novi, false);
    
    return TipVrijednosti();
}

template<typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::ispisi()
{
    for(int i=0; i<kapacitet; i++)
    {
        if(this->niz_parova[i] != nullptr )
        std::cout<<i+1<<". "<<niz_tri[i]->tri_k<<" "<<niz_tri[i].tri_v<<"!"<<std::endl;
    }
       
    
    std::cout<<std::endl;
}

template<typename TipKljuca, typename TipVrijednosti>
int HashMapa<TipKljuca, TipVrijednosti>::brojElemenata() const
{
    return broj_upisanih;
}

int main() 
{
    
    /*HashMapa<std::string,std::string> m;
    m.definisiHashFunkciju(djbhash);
    m["Sarajevo"] = "BiH";
    m["Zagreb"] = "Hrvatska";
    {
            // Konstruktor kopije
    	HashMapa<std::string,std::string> m2(m);
            // Operator dodjele
    	HashMapa<std::string,std::string> m3;
    	m3.definisiHashFunkciju(djbhash);
    	m3["Kiseljak"] = "BiH";
    	cout << (m3=m).brojElemenata() << " "; // 2
            // Destruktivna samododjela
    	cout << (m=m).brojElemenata() << " "; // 2
    
            // Plitka kopija
    	m.obrisi("Sarajevo");
    	cout << m2.brojElemenata() << " "; // 2
            cout << m2["Sarajevo"] << " "; // BiH
            cout << m3.brojElemenata() << " "; // 2
            cout << m3["Sarajevo"] << " "; // BiH
    }
    cout << m.brojElemenata(); // 1
    cout << " '" << m["Sarajevo"] << "' "; // ''
    m.obrisi();
    cout << m.brojElemenata(); // 0
    cout << " '" << m["Zagreb"] << "'"; // ''*/

    return 0;
}
