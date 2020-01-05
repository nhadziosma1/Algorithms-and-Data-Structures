#include <iostream>
#include <stdexcept>

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
    pomocni.top=nullptr;
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
        //ovo ce obaviti destruktor svakako
        /*Cvor *sljedeci;
        
        for(Cvor pok=pomocni.top; pok!=nullptr; pok=sljedeci)
        {
            sljedeci=pok->veza;
            delete pok;
        }*/
        
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

//////////////////////////testovi

bool Test1()
{
    Stek<int> st;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    st.stavi(i);
    
    int n=st.brojElemenata();
    if(n!=10)
    tacan=false;
    
    int broj=9;
    for(int i=0; i<n; i++)
    {
        if(st.vrh()!=broj)
        tacan=false;
        
        st.skini();
        if(st.brojElemenata()!=broj && st.brojElemenata()==n)
        tacan=false;
        
        broj--;
    }
    
    return tacan;
}

bool Test2()
{
    Stek<int> st;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    st.stavi(i);
    
    int n=st.brojElemenata();
    if(n!=10)
    tacan=false;

    for(int i=0; i<n; i++)
    st.skini();
    
    if(st.brojElemenata()!=0)
    tacan=false;
    
    return tacan;
}

bool Test3()
{
    Stek<int> st;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    {
        st.stavi(i);
        
        if(st.vrh()!=i)
        tacan=false;
    }
    
    return tacan;
}

bool Test4()
{
    Stek<int> st;
    bool tacan=true;
     
    if(st.brojElemenata()!=0)
    tacan=false;
    
    return tacan;
}

bool Test5()
{
    Stek<int> st;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    st.stavi(i);
    
    Stek<int> st2(st);
    
    if(st.brojElemenata()!=st2.brojElemenata())
    tacan=false;
    
    int spasena_vel_st=st.brojElemenata(), spasena_vel_st2=st2.brojElemenata();

    for(int i=0; i<spasena_vel_st; i++)
    st.skini();
    
    if(st.brojElemenata()==spasena_vel_st)
    tacan=false;
    
    if(st2.brojElemenata()!=spasena_vel_st2)
    tacan=false;
    
    return tacan;
}

bool Test6()
{
    Stek<int> st;
    bool tacan=true;
    
    for(int i=0; i<10; i++)
    st.stavi(i);
    
    Stek<int> st2;
    for(int i=0; i<10; i++)
    {
        if(i%2==0)
        st2.stavi(i);
    }
    
    st2=st;
    
    if(st.brojElemenata()!=st2.brojElemenata())
    tacan=false;
    
    int spasena_vel_st=st.brojElemenata(), spasena_vel_st2=st2.brojElemenata();

    for(int i=0; i<spasena_vel_st; i++)
    st.skini();
    
    if(st.brojElemenata()==spasena_vel_st)
    tacan=false;
    
    if(st2.brojElemenata()!=spasena_vel_st2)
    tacan=false;
    
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
    
    return 0;
}
