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


//testovi
void Test1()
{
    Stek<vector<int> > s;
    
    vector<int> a;
    for (int i=1; i<10; i++)
    a.push_back(i);
    
    vector<int> b;
    for (int i=15; i<40; i++)
    b.push_back(i);
    
    
    vector<int> c;
    for(int i=100; i<500; i+=15)
    c.push_back(i);
    
    vector<int> d;
    for(int i=600; i<150; i*=4)
    d.push_back(i);
    
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);
    pretraga(s,35);       // "20 1" 
    std::cout<<std::endl;
}

void Test2()
{
    Stek<vector<int> > s;
    
    vector<int> a;
    for (int i=1; i<10; i++)
    a.push_back(i);
    
    vector<int> b;
    for (int i=15; i<40; i++)
    b.push_back(i);
    
    
    vector<int> c;
    for(int i=100; i<500; i+=15)
    c.push_back(i);
    
    vector<int> d;
    for(int i=600; i<150; i*=4)
    d.push_back(i);
    
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);
    pretraga(s,45);       // "Nema elementa" 
    std::cout<<std::endl;
}

void Test3()
{
    Stek<vector<int> > s;
    
    vector<int> a;
    for (int i=1; i<10; i++)
    a.push_back(i);
    
    vector<int> b;
    for (int i=15; i<40; i++)
    b.push_back(i);
    
    
    vector<int> c;
    for(int i=100; i<500; i+=15)
    c.push_back(i);
    
    vector<int> d;
    
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);
    pretraga(s,2000);       // "Nema elementa" 
    std::cout<<std::endl;
}

void Test4()
{
    Stek<vector<int> > s;
    
    vector<int> a;
    for (int i=1; i<10; i++)
    a.push_back(i);
    
    vector<int> b;
    for (int i=15; i<40; i++)
    b.push_back(i);
    
    
    vector<int> c;
    for(int i=100; i<500; i+=15)
    c.push_back(i);
    
    vector<int> d;
    
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);
    pretraga(s,-5);       // "Nema elementa" 
    std::cout<<std::endl;
}

void Test5()
{
    Stek<vector<int> > s;
    
    vector<int> a;
    for (int i=1; i<10; i++)
    a.push_back(i);
    
    vector<int> b;
    for (int i=15; i<40; i++)
    b.push_back(i);
    
    vector<int> c;
    
    vector<int> d;
    
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);
    pretraga(s, 1);       // "0 0" 
    std::cout<<std::endl;
}

int main()
{
    /*
    //ovo je rejsenje kad je "pretraga" fija clanica, previdio sam to, pa cu je napisati 
    //i kao obicnu fiju, ali ni ovu verziju necu brisati necu brisati
    Stek<std::vector<int>> s;
    
    std::vector<int> prvi{1,2,3,4,5}, drugi{6,8}, treci{9,10,130}, cetvrti{157, 226, 3424};
    
    s.stavi(cetvrti);
    s.stavi(treci);
    s.stavi(drugi);
    s.stavi(prvi);
    
    s.pretraga(s, 10);*/
    
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    return 0;
}
