#include "BaseInclude.h"

sf::TcpSocket soquete;

std::atomic<bool> running=true;
std::atomic<bool> enviando=false;
std::atomic<bool> idnaoexiste=false;
std::atomic<int> scrollcontatos=0;
std::mutex travachat,travaconversas;

std::list<Texto> fila;
std::list<int> ordemconversas;
int idsconversas[8];
int id;
float offset=705;
std::unordered_map<int,std::pair<std::wstring,std::wstring>> jogadores;

int ncarac=0;
int nlinhas=1;
std::wstring stringmensagem;
std::wstring nome,envianome;
sf::Font fonte;
sf::Text textomensagem;
sf::Text textosidsconversas[8];
sf::Text textosjogo[8];
sf::Text antesdigitarnome;
sf::Text digitarnome;
sf::Text nomefixo;
sf::Text idfixo;

sf::RectangleShape retpadrao1(sf::Vector2f(300,30));
sf::RectangleShape retpadrao2(sf::Vector2f(300,30));
sf::RectangleShape caixa(sf::Vector2f(1000,90));
sf::RectangleShape cobre(sf::Vector2f(1000,680));
sf::RectangleShape retcontato[8];
sf::RectangleShape info;
sf::RectangleShape retnome(sf::Vector2f(365,30));
sf::RectangleShape retcvatual(sf::Vector2f(1000,30));

sf::FloatRect limpesquisa;


bool Inicializa()
{
    std::fstream pegaip;
    pegaip.open("config.txt",std::fstream::in);
    std::string getip;
    std::getline(pegaip,getip);
    pegaip.close();
    //testa conexao
    sf::Socket::Status estado=soquete.connect(getip,1000,sf::seconds(15.f));
    if(estado!=sf::Socket::Done)
        return 0;
    sf::Packet pacote,pacote2;
    soquete.receive(pacote);
    int tipo;
    pacote>>tipo;
    if(tipo==-1)
        return 0;
    else
        pacote>>id;
    fonte.loadFromFile("sides/consola.ttf");
    antesdigitarnome.setFont(fonte);
    antesdigitarnome.setCharacterSize(20);
    antesdigitarnome.setFillColor(sf::Color(0,0,0));
    digitarnome=antesdigitarnome;
    antesdigitarnome.setString("Digite seu nome");
    retnome.setFillColor(sf::Color::White);
    retnome.setPosition(0,30);
    digitarnome.setPosition(0,30);
    sf::RenderWindow window(sf::VideoMode(300,60),"Nome",sf::Style::None);
    window.setFramerateLimit(20);
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type==sf::Event::KeyPressed)
            {
                if(event.key.code==sf::Keyboard::Enter)
                {
                    window.close();
                    break;
                }
            }
            if(event.type==sf::Event::TextEntered)
            {
                auto c=event.text.unicode;
                if(c==8&&nome.size()!=0)
                {
                    nome.pop_back();
                    digitarnome.setString(nome);
                    break;
                }
                if(c!=13&&c!=8&&c!=27&&nome.size()<15)
                {
                    nome+=c;
                    digitarnome.setString(nome);
                    break;
                }
            }
        }
        window.clear(sf::Color(232,111,130));
        window.draw(antesdigitarnome);
        window.draw(retnome);
        window.draw(digitarnome);
        window.display();
    }
//troca info com soquete
    pacote2<<nome;
    soquete.send(pacote2);
//formata textos
    Texto::textopadrao.setFillColor(sf::Color(0,0,0));
    Texto::textopadrao.setCharacterSize(20);
    Texto::textopadrao.setFont(fonte);
    textomensagem=Texto::textopadrao;
    textomensagem.setPosition(300,710);
    antesdigitarnome.setString("Digite o novo nome ou esc");
    nomefixo=Texto::textopadrao;
    nomefixo.setString(nome);
    nomefixo.setPosition(5,5);
    idfixo=Texto::textopadrao;
    idfixo.setCharacterSize(18);
    idfixo.setFillColor(sf::Color(255,0,0));
    idfixo.setString("#"+std::to_string(id));
    idfixo.setPosition(5,30);
//formata sprites
    retpadrao1.setFillColor(sf::Color(81,91,250));
    retpadrao2=retpadrao1;
    retpadrao2.setFillColor(sf::Color(120,151,200));
    caixa.setPosition(sf::Vector2f(300,710));
    cobre.setFillColor(sf::Color(232,111,130));
    cobre.setPosition(300,30);
    for(int i=0; i<8; i++)
    {
        retcontato[i]=sf::RectangleShape(sf::Vector2f(298,80));
        retcontato[i].setFillColor(sf::Color(84,186,232));
        retcontato[i].setOutlineColor(sf::Color(190,196,187));
        retcontato[i].setOutlineThickness(2);
        retcontato[i].setPosition(0,160+80*i);
        idsconversas[i]=-1;
        textosidsconversas[i]=Texto::textopadrao;
        textosidsconversas[i].setPosition(5,165+80*i);
        textosjogo[i]=Texto::textopadrao;
        textosjogo[i].setPosition(5,195+80*i);
    }
    info=sf::RectangleShape(sf::Vector2f(300,160));
    info.setFillColor(sf::Color(232,207,130));
    info.setPosition(0,0);
    retcvatual.setFillColor(sf::Color(188,235,196));
    retcvatual.setPosition(300,0);

//inicializa limites

    return 1;

}

int DesfazEnter()
{
    auto it=stringmensagem.rbegin();
    int ret=0;
    while((*it)!='\n')
    {
        it++;
        ret++;
        if(it==stringmensagem.rend())
            return std::size(stringmensagem);
    }
    return ret;
}

void MudaNome(int user)
{
    std::wstring novonome=jogadores[user].first;
    digitarnome.setString(novonome);
    bool mudou=false;
    sf::RenderWindow window(sf::VideoMode(300,60),"Nome",sf::Style::None);
    window.setFramerateLimit(20);
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type==sf::Event::KeyPressed)
            {
                if(event.key.code==sf::Keyboard::Enter)
                {
                    mudou=true;
                    window.close();
                    break;
                }
                if(event.key.code==sf::Keyboard::Escape)
                {
                    window.close();
                    break;
                }
            }
            if(event.type==sf::Event::LostFocus)
            {
                window.close();
            }
            if(event.type==sf::Event::TextEntered)
            {
                auto c=event.text.unicode;
                if(c==8&&novonome.size()!=0)
                {
                    novonome.pop_back();
                    digitarnome.setString(novonome);
                }
                else if(c!=13&&c!=8&&novonome.size()<20)
                {
                    novonome+=c;
                    digitarnome.setString(novonome);
                }
            }
        }
        window.clear(sf::Color(200,191,231));
        window.draw(antesdigitarnome);
        window.draw(retnome);
        window.draw(digitarnome);
        window.display();
    }
    if(mudou)
    {
        int a=0;
        jogadores[user].first=novonome;
        for(auto i:ordemconversas)
        {
            idsconversas[a]=i;
            textosidsconversas[a].setString(jogadores[i].first);
            a++;
            if(a==8)
                break;
        }
    }
    return;
}
