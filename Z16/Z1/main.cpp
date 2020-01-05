#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <utility> //treba za tip "std::pair<>"

#include <string> //zbog testova

using namespace std;

//mapa zamisljena kao jedan element, a NizMapa, kao stavrana mapa koja je kontejnerskog tipa 
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
        {
            pomocni[i] = m2.niz_parova[i];
            
            //pomocni[i].first = m2.niz_parova[i].first;
            //pomocni[i].second = m2.niz_parova[i].second;
            
            //pomocni[i] = std::make_pair(m2.niz_parova[i].first, m2.niz_parova[i].second);
        }
            
        /*for(int i=0; i<m2.velicina; i++)
        std::cout<<i+1<<". "<<m2.niz_parova[i].first<<" "<<m2.niz_parova[i].second<<"!"<<std::endl;
        
        for(int i=0; i<m2.velicina; i++)
        std::cout<<i+1<<". "<<pomocni[i].first<<" "<<pomocni[i].second<<"!"<<std::endl;*/
        
        
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
////////////////////////////////////////
bool Test1() 
{
    NizMapa<int, char> ml1, ml2;
    bool tacan=true;
    
    //int niz_l_vrijednosti[]={1,2,3,4,5,6,7,8,9}
    
    char slovo = 'A';
    
    for(int i=0; i<9; i++)
    ml1[i] = slovo+i;
    
    for(int i=0; i<ml1.brojElemenata(); i++)
    {
        if(ml1[i] != slovo+i)
        tacan=false;
    }
    
    
    if(ml1.brojElemenata()!= 9)
    tacan=false;
    
    ml2 = ml1;
    
    ml1.obrisi(0);
    ml1.obrisi(1);
    
    if(ml1.brojElemenata() == ml2.brojElemenata())
    tacan=false;
    
    NizMapa<int, char> ml3(ml2);
    
    ml2.obrisi(1);
    ml2.ispisi();
    
    ml2.obrisi(2);
    ml2.ispisi();
    //ml2.obrisi(3);
    
    if(ml3.brojElemenata() == ml2.brojElemenata())
    tacan = false;
    
    return tacan;
}

bool Test2()  
{
    NizMapa<char, int> ml1, ml2;
    bool tacan=true;
    
    //int niz_l_vrijednosti[]={1,2,3,4,5,6,7,8,9}
    
    char slovo = 'A';
    
    for(int i=0; i<9; i++)
    ml1[slovo+i] = i;
    
    for(int i=0; i<ml1.brojElemenata(); i++)
    {
        if(ml1[slovo+i] != i)
        tacan=false;
    }
    
    
    if(ml1.brojElemenata()!= 9)
    tacan=false;
    
    ml2 = ml1;
    
    ml1.obrisi('A');
    ml1.obrisi('B');
    
    if(ml1.brojElemenata() == ml2.brojElemenata())
    tacan=false;
    
    NizMapa<char, int> ml3(ml2);
    
    ml2.obrisi('A');
    ml2.ispisi();
    
    ml2.obrisi('C');
    ml2.ispisi();
    ml2.obrisi('D');
    
    if(ml3.brojElemenata() == ml2.brojElemenata())
    tacan = false;
    
    return tacan;
}

bool Test3()
{
    NizMapa<char, std::string> ml1, ml2;
    bool tacan=true;
    
    //int niz_l_vrijednosti[]={1,2,3,4,5,6,7,8,9}
    
    char slovo = 'A';
    
    std::string vrijednost= "Sifra";
    
    for(int i=0; i<9; i++)
    ml1[slovo+i] = vrijednost;
    
    for(int i=0; i<ml1.brojElemenata(); i++)
    {
        if(ml1[slovo+i] != vrijednost)
        tacan=false;
    }
    
    
    if(ml1.brojElemenata()!= 9)
    tacan=false;
    
    ml2 = ml1;
    
    ml1.obrisi('A');
    ml1.obrisi('B');
    
    if(ml1.brojElemenata() == ml2.brojElemenata())
    tacan=false;
    
    NizMapa<char, std::string> ml3(ml2);
    
    ml2.obrisi('A');
    ml2.ispisi();
    
    ml2.obrisi('C');
    ml2.ispisi();
    ml2.obrisi('D');
    
    if(ml3.brojElemenata() == ml2.brojElemenata())
    tacan = false;
    
    return tacan;
}

bool Test4()
{
    NizMapa<std::string, int> ml1, ml2;
    bool tacan=true;
    
    std::vector< std::string> v={"Sifra1", "Sifra2", "Sifra3", "Sifra4", "Sifra5"};
    
    for(int i=0; i<5; i++)
    ml1[v.at(i)] = i;
    
    for(int i=0; i<ml1.brojElemenata(); i++)
    {
        if(ml1[v.at(i)] != i)
        tacan=false;
    }
    
    
    if(ml1.brojElemenata()!= 5)
    tacan=false;
    
    ml2 = ml1;
    
    ml1.obrisi("Sifra4");
    ml1.obrisi("Sifra2");
    
    if(ml1.brojElemenata() == ml2.brojElemenata())
    tacan=false;
    
    NizMapa<std::string,int> ml3(ml2);
    
    ml2.obrisi("Sifra1");
    ml2.ispisi();
    
    ml2.obrisi("Sifra3");
    ml2.ispisi();
    ml2.obrisi("Sifra4");
    
    if(ml3.brojElemenata() == ml2.brojElemenata())
    tacan = false;
    
    return tacan;
}

bool Test5()
{
    NizMapa<std::string, int> ml1, ml2;
    bool tacan=true;
    
    std::vector< std::string> v={"Sifra1", "Sifra2", "Sifra3", "Sifra4", "Sifra5"};
    
    char slovo='A';
    
    for(int i=0; i<5; i++)
    ml1[v.at(i)] = slovo+i;
    
    for(int i=0; i<ml1.brojElemenata(); i++)
    {
        if(ml1[v.at(i)] != slovo+i)
        tacan=false;
    }
    
    
    if(ml1.brojElemenata()!= 5)
    tacan=false;
    
    ml2 = ml1;
    
    ml1.obrisi("Sifra4");
    ml1.obrisi("Sifra2");
    
    if(ml1.brojElemenata() == ml2.brojElemenata())
    tacan=false;
    
    NizMapa<std::string,int> ml3(ml2);
    
    ml2.obrisi("Sifra1");
    ml2.ispisi();
    
    ml2.obrisi("Sifra3");
    ml2.ispisi();
    ml2.obrisi("Sifra4");
    
    if(ml3.brojElemenata() == ml2.brojElemenata())
    tacan = false;
    
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
    
    return 0;
}
