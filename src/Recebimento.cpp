#include "Recebimento.h"
#include <iostream>

int Recebimento::EscrevePapel(int dest)
{
    int retorno=(id+dest)%(jogadores.size()+1);
    std::wcout<<L"valor="<<retorno<<L"\n";
    envianome.clear();
    digitarnome.setString(envianome);
    antesdigitarnome.setString(L"Digite o nome para "+jogadores[retorno].first);
    sf::RenderWindow window(sf::VideoMode(365,60),"Nome",sf::Style::None);
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
                if(c==8&&envianome.size()!=0)
                {
                    envianome.pop_back();
                    digitarnome.setString(envianome);
                    break;
                }
                if(c!=13&&c!=8&&c!=27&&envianome.size()<15)
                {
                    envianome+=c;
                    digitarnome.setString(envianome);
                    break;
                }
            }
        }
        window.clear(sf::Color(200,191,231));
        window.draw(antesdigitarnome);
        window.draw(retnome);
        window.draw(digitarnome);
        window.display();
    }
    return retorno;
}

void Recebimento::Recebe ()
{
    sf::Packet pacote;
    sf::Socket::Status status;
    while(running)
    {
        sf::sleep(sf::seconds(0.2f));
        status=soquete.receive(pacote);
        if(status==sf::Socket::Done)
        {
            int tipo;
            pacote>>tipo;
            int a=0;
            switch(tipo)
            {
            case 0:
            {
                //recebe todos os nomes
                int quant;
                int dest;
                pacote>>quant;
                for(int i=0; i<quant; i++)
                {
                    pacote>>dest>>envianome;
                    if(dest!=id)
                    {
                        if(jogadores.count(dest)==0)
                        {
                            jogadores.emplace(std::piecewise_construct,std::forward_as_tuple(dest),std::forward_as_tuple(L"=ID:"+std::to_wstring(dest),envianome));
                        }
                        else
                            jogadores[dest].second=envianome;
                    }
                }
                int i=0;
                for (auto &a:ordemconversas)
                {
                    textosjogo[i].setString(jogadores[a].second);
                    i++;
                    if(i==8)
                        break;
                }
                break;
            }
            case 1:
            {
                //digita papel
                int roll;
                std::wcout<<jogadores.size()<<std::endl;
                pacote>>roll;
                int dest=EscrevePapel(roll);
                sf::Packet pacote2;
                std::wcout<<L"envio o nome:"<<envianome<<std::endl;
                pacote2<<1<<dest<<envianome;
                soquete.send(pacote2);
                break;
            }
            case 7:
            {
                //recebe novo nome de jogador
                int dest;
                unsigned int numnomes;
                pacote>>numnomes;
                for(unsigned int i=0; i<numnomes; i++)
                {
                    pacote>>dest>>envianome;
                    jogadores.emplace(std::piecewise_construct,std::forward_as_tuple(dest),std::forward_as_tuple(envianome,L""));
                    ordemconversas.push_back(dest);
                }
                std::lock_guard<std::mutex> lk(travaconversas);
                int i=0;
                for(auto &a:ordemconversas)
                {
                    textosidsconversas[i].setString(jogadores[a].first);
                    i++;
                    if(i==8)
                        break;
                }
                break;
            }
            default:
                break;

            }
            status=sf::Socket::NotReady;
        }
    }

}
