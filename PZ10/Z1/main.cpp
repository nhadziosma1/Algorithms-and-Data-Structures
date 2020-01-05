#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

//prvo ide deklaracija protipa klasa zbog njihove ciklicne povezanosti
template <typename TipOznake>
class Grana;

template <typename TipOznake>
class Cvor;

template <typename TipOznake>
class UsmjereniGraf;

template <typename TipOznake>
class GranaIterator;

//KLASE
template<typename TipOznake>
class Cvor
{
    private:
    UsmjereniGraf<TipOznake> *cv_gr_pok;
    int redni_br;
    bool posjecen;
    
    public:
    Cvor(UsmjereniGraf<TipOznake> *gr_ug_pok, int redni_br)
    {
        cv_gr_pok = gr_ug_pok;
        this->redni_br = redni_br;
    }
    
    TipOznake dajOznaku() const
    {
        return cv_gr_pok->dajOznakuCvora(redni_br);
    }
    
    void postaviOznaku(TipOznake oz)
    {
        cv_gr_pok->postaviOznakuCvora(redni_br, oz);
    }
    
    int dajRedniBroj() const
    {
        return redni_br;
    }
};

template<typename TipOznake>
class Grana
{
  private:
  UsmjereniGraf<TipOznake> *gr_ug_pok;
  int polazni;
  int dolazni;
  
  public:
  
  Grana(UsmjereniGraf<TipOznake> *gr_ug_pok, int polazni, int dolazni)
  {
      this->gr_ug_pok = gr_ug_pok;
      this->polazni = polazni;
      this->dolazni = dolazni;
  }
  
  double dajTezinu() const
  {
      return gr_ug_pok->dajTezinuGrane(polazni, dolazni);
  }
  
  void postaviTezinu(double tezina)
  {
      gr_ug_pok->postaviTezinuGrane(polazni, dolazni, tezina);
  }
  
  TipOznake dajOznaku() const
  {
      return gr_ug_pok->dajOznakuGrane(polazni, dolazni);
  }
  
  void postaviOznaku(TipOznake oznaka)
  {
      gr_ug_pok->postaviOznakuGrane(polazni, dolazni, oznaka);
  }
  
  Cvor<TipOznake> dajPolazniCvor() const
  {
      return gr_ug_pok->dajCvor(polazni);    
  } 
  
  Cvor<TipOznake> dajDolazniCvor() const
  {
      return gr_ug_pok->dajCvor(dolazni);    
  }
};

template<typename TipOznake>
class UsmjereniGraf
{
    public:
    //grana se oznacava polaznim i odlaznim cvorom
    
    /*UsmjereniGraf(const UsmjereniGraf &ug);
    UsmjereniGraf &operator=(const UsmjereniGraf &ug);*/
    
    UsmjereniGraf(int br_cv){};
    virtual ~UsmjereniGraf(){};
    
    virtual int DajBrojCvorova() const =0;
    virtual void postaviBrojCvorora(int br_cv) =0;
    virtual void dodajGranu(int polazni, int dolazni, double tezina_gr) =0;
    virtual void obrisiGranu(int polazni, int dolazni) =0;
    virtual void postaviTezinuGrane(int polazni, int dolazni, double tezina_gr) =0;
    virtual double dajTezinuGrane(int polazni, int dolazni) const =0;
    virtual bool postojiGrana(int polazni, int dolazni) =0;
    
    virtual void postaviOznakuCvora(int br_cv, TipOznake oznaka) =0;
    virtual TipOznake dajOznakuCvora(int c) const =0;
    virtual void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka) =0;
    virtual TipOznake dajOznakuGrane(int polazni, int dolazni) const =0;
    
    //nisu inspektori, jer ce se posliej pokazati da pozivaju u metodma koje mijenjaju this
    Grana<TipOznake> dajGranu(int polazni, int dolazni)
    {
        return Grana<TipOznake>(this, polazni, dolazni);
    }
    Cvor<TipOznake> dajCvor(int cvor) 
    {
        return Cvor<TipOznake>(this, cvor);
    }
    
    //ove metode ne mogu biti oznacene sa conts, ejr ce se korisiti u fijama u kojima 
    // se mijenja pokazivac this
    virtual GranaIterator<TipOznake> dajGranePocetak() =0;
    //vraca pokzazivac na jedan elemnt poslije kraja kao i metoda "end()"
    virtual GranaIterator<TipOznake> dajGraneKraj() =0;
    
    //ON STAVIO DA MU JE CONST
    virtual GranaIterator<TipOznake> dajSljedecuGranu(int polazni, int dolazni) =0;
};

template <typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake>
{
    private:
    
    struct PodaciGrane
    {
        bool postoji;
        TipOznake oznaka;
        double tezina;
    };
    
    std::vector<std::vector<PodaciGrane>> matrica;
    std::vector<TipOznake> oznake_cvorova;
    
    
    public:
    MatricaGraf(int br_cv) : UsmjereniGraf<TipOznake>(br_cv)
    {
        matrica.resize(br_cv);
        
        PodaciGrane nepostojeca;
        nepostojeca.postoji = false;
        
        for(int i=0; i<matrica.size(); i++)
        matrica.at(i).resize(br_cv, nepostojeca); //resajza redove matrice i postavlja defaultne vrijednsoti
        
        oznake_cvorova.resize(matrica.size());
        
        //postaviBrojCvorora(br_cv);
    }
    
    int DajBrojCvorova() const
    {
        return matrica.size();
    }
    
    void postaviBrojCvorora(int br_cv)
    {
        if(br_cv < matrica.size())
        throw std::domain_error("Nije dozvoljeno da se smanji broj cvorova");
        
        matrica.resize(br_cv); // resajzaj broj redova matrice
        
        PodaciGrane nepostojeca;
        nepostojeca.postoji = false;
        
        for(int i=0; i<matrica.size(); i++)
            matrica.at(i).resize(br_cv, nepostojeca); //resajzaj svaki red i sve elemnete koji se dodaju postavi na "nepostojeca"
         
        oznake_cvorova.resize(matrica.size());
    }
    
    void dodajGranu(int polazni, int dolazni, double tezina_gr)
    {
        if(polazni<0 || dolazni<0 || polazni > matrica.size() || dolazni > matrica.size())
        throw std::domain_error("pogresno unesene koordinate matrice");
        
        matrica.at(polazni).at(dolazni).postoji = true;
        matrica.at(polazni).at(dolazni).tezina = tezina_gr;
    }
    
    void obrisiGranu(int polazni, int dolazni)
    {
        if(polazni<0 || dolazni<0 || polazni > matrica.size() || dolazni > matrica.size())
        throw std::domain_error("pogresno unesene koordinate matrice");
        
        matrica.at(polazni).at(dolazni).postoji = false;
        matrica.at(polazni).at(dolazni).tezina = 0;
    }
    
    void postaviTezinuGrane(int polazni, int dolazni, double tezina_gr)
    {
        if(polazni<0 || dolazni<0 || polazni > matrica.size() || dolazni > matrica.size())
        throw std::domain_error("pogresno unesene koordinate matrice");
        
        matrica.at(polazni).at(dolazni).tezina = tezina_gr;
    }
    
    double dajTezinuGrane(int polazni, int dolazni) const
    {
        if(polazni<0 || dolazni<0 || polazni > matrica.size() || dolazni > matrica.size())
        throw std::domain_error("pogresno unesene koordinate matrice");
        
        return matrica.at(polazni).at(dolazni).tezina;
    }
    
    bool postojiGrana(int polazni, int dolazni)
    {
        return matrica.at(polazni).at(dolazni).postoji;
    }
    
    void postaviOznakuCvora(int br_cv, TipOznake oznaka)
    {
        if(br_cv<0 || br_cv > oznake_cvorova.size() )
        throw std::domain_error("pogresno unesene koordinate matrice");
        
        oznake_cvorova.at(br_cv) = oznaka;
    }
    
    TipOznake dajOznakuCvora(int br_cv) const
    {
        if(br_cv<0 || br_cv > oznake_cvorova.size() )
        throw std::domain_error("pogresno unesene koordinate matrice");
        
        return oznake_cvorova.at(br_cv);
    }
    
    void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka)
    {
        if(polazni<0 || dolazni<0 || polazni > matrica.size() || dolazni > matrica.size())
        throw std::domain_error("pogresno unesene koordinate matrice");
        
        matrica.at(polazni).at(dolazni).oznaka = oznaka;
    }
    
    TipOznake dajOznakuGrane(int polazni, int dolazni) const
    {
        if(polazni<0 || dolazni<0 || polazni > matrica.size() || dolazni > matrica.size())
        throw std::domain_error("pogresno unesene koordinate matrice");
        
        return matrica.at(polazni).at(dolazni).oznaka;
    }
    
    
    GranaIterator<TipOznake> dajGranePocetak()
    {
        GranaIterator<TipOznake> it(this, 0, -1);
        return ++it;
    }
    
    //vraca pokzazivac na jedan elemnt poslije kraja kao i metoda "end()"
    GranaIterator<TipOznake> dajGraneKraj()
    {
        GranaIterator<TipOznake> it(this, -1, -1);
        return it;
    }
    
    //ON STVAIO  DA JE CONST
    GranaIterator<TipOznake> dajSljedecuGranu(int polazni, int dolazni)
    {
        //std::cout<<"prije poziva petlji"<<std::endl;
        for(int i=polazni; i<matrica.size(); i++)
        {
            //std::cout<<"i="<<i<<" ";
            for(int j=0; j<matrica.size(); j++)
            {
                //std::cout<<"j="<<j<<std::endl;
                
                if(i==polazni && j<=dolazni)
                continue;
                
                if(matrica.at(i).at(j).postoji == true)
                {
                    return GranaIterator<TipOznake>(this, i, j);
                }
            }
        }
        
        return GranaIterator<TipOznake>(this, -1, -1);
    }
};

template<typename TipOznake>
class GranaIterator
{
    private:
    UsmjereniGraf<TipOznake> *gr_ug_pok;
    int polazni;
    int dolazni;
  
    public:
    // TRAZI DA SE OVDJE POSTAVI PARAMETAR NA CONST, JER JE THIS KOJI MU SE SALJE CONT
    
    GranaIterator(UsmjereniGraf<TipOznake> *gr_it_ug_pok, int polazni, int dolazni)
    {
        this->gr_ug_pok = gr_it_ug_pok;
        this->polazni = polazni;
        this->dolazni = dolazni;
    }
    
    int getPolazni()
    {
        return polazni;
    }
    int getDolazni()
    {
        return dolazni;
    }
    
    Grana<TipOznake> operator*()
    {
        return gr_ug_pok->dajGranu(polazni, dolazni);
    }
    
    bool operator==(const GranaIterator& it) const
    {
        //std::cout<<"pozvan iterator";
        if( gr_ug_pok==it.gr_ug_pok && polazni == it.polazni && dolazni == it.dolazni)
        return true;
        
        return false;
    }
     
    bool operator!=(const GranaIterator& it) const
    {
        //I nacin
        if( gr_ug_pok == it.gr_ug_pok && polazni == it.polazni && dolazni == it.dolazni)
        return false;
        
        return true;
        
        //II nacin
        //return !(this == it);
    }
    
    //prefiksni
    GranaIterator &operator++()
    {
        if(polazni ==-1 && dolazni==-1)
        throw "nije validna grana";
        
        //treba promijeniti this
        GranaIterator g = gr_ug_pok->dajSljedecuGranu(polazni, dolazni);
        polazni = g.polazni;
        dolazni = g.dolazni;
        
        return *this;
    }
    
    GranaIterator operator++(int postfikcni)
    {
        GranaIterator tmp = *this;
        
        ++(*this);
        return tmp;
    }
 
};

//POSTO I DFS I BFS PRETRAGE MORAJU PROCI KROZ SVE CVOROVE, MORAJU SE REALIZIRATI REKURZIVNO
/*template<typename TipOznake>
void DFS(UsmjereniGraf<TipOznake> *ug, std::vector< Cvor<TipOznake> > &v, int od_ovog, std::stack<int> &stek GranaIterator<TipOznake> g_it)
{
    GranaIterator g_it2(ug, g_it.getPolazni(); g_it.getDolazni());
    
    g_it2 = ug->dajSljedecuGranu(g_it.getPolazni(), 0);
    
    if(g_it2.getDolazni() == g_it.getPolazni())
    {
        g_it = g_it2;
        
        stek.push(g_it.getPolazni());
        
        DFS(ug, v, g_it.getPolazni(), stek, g_it);
    }
    
    
}

//pocetni cvor od kojeg se zeli da pocne pretraga mora biti zadat
template<typename TipOznake>
void obidjiPoDubini(UsmjereniGraf<TipOznake> *ug, std::vector< Cvor<TipOznake> > &v, int pocetni)
{
    GranaIterator<TipOznake> g_it =  ug->dajGranePocetak();
    
    for(int i =0; i<ug->dajBrojCvorova(); i++)
    {
        ug->postaviOznakuCvora(i, TipOznake);
        v.at(i).posjecen = false;
    }
    
    std::stack<int> stek;
    stek.push(pocetni);
    
    while(stek.empty() == false)
    {
        DFS(ug, v, stek, 0, pocetni, g_it);
    }
}*/

int main() 
{
    try 
    {
        UsmjereniGraf<bool> *g = new MatricaGraf<bool>(3);
        
        g->dodajGranu(0, 1, 2.5);
        g->dodajGranu(1, 0, 1.2);
        g->dodajGranu(1, 2, 0.1);
        g->dodajGranu(0, 0, 3.14);
        
        for (GranaIterator<bool> iter = g->dajGranePocetak(); iter != g->dajGraneKraj(); ++iter)
        cout << (*iter).dajPolazniCvor().dajRedniBroj() << " " << (*iter).dajDolazniCvor().dajRedniBroj() << " "<< (*iter).dajTezinu() << endl;
        
        delete g;
    } 
    catch (const char izuzetak[]) 
    {
        cout << izuzetak << endl;
    }
    
     return 0;
}
