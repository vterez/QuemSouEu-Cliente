#define SFML_DEFINE_DISCRETE_GPU_PREFERENCE
#include "Envio.h"
#include "Recebimento.h"
#include <iostream>

int main()
{
    SetConsoleCP(1252);
    SetConsoleOutputCP(1252);

    if(!Inicializa())
        return -1;

    std::thread t1(Recebimento::Recebe);
    std::thread t2(Envio::EnviaCheck);

    float alturabase=0;
    bool scrolling=false;
    bool controledesenho=0;
    sf::Vector2f posimouse;
    float delta;

    sf::RenderWindow window(sf::VideoMode(1300,800),"Chat");
    window.setFramerateLimit(30);
    float dt = 1.f/100.f; // Modify this to change physics rate.
    float accumulator = 0.f;
    bool drawn = false;
    sf::Clock clock;
    while(window.isOpen())
    {
        accumulator += clock.restart().asSeconds();
        while(accumulator>=dt)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                switch(event.type)
                {
                case sf::Event::Closed:
                {
                    window.close();
                    running=false;
                    break;
                }
                case sf::Event::KeyPressed:
                {
                    switch(event.key.code)
                    {
                    case sf::Keyboard::F1:
                    {
                        sf::Packet pacote;
                        pacote<<2;
                        soquete.send(pacote);
                        break;
                    }
                    case sf::Keyboard::F2:
                    {
                        sf::Packet pacote;
                        pacote<<3;
                        soquete.send(pacote);
                        break;
                    }
                    case sf::Keyboard::F3:
                    {
                        sf::Packet pacote;
                        pacote<<8;
                        soquete.send(pacote);
                        break;
                    }
                    case sf::Keyboard::Enter:
                    {
                        if(!event.key.shift)
                        {
                            if(nlinhas==1&&ncarac==0)
                                break;
                            Envio::FormataTexto();
                            Texto tx(stringmensagem,retpadrao2);
                            float off=tx.offset;
                            for(auto &a:fila)
                                a.Enquadra(off);
                            fila.push_back(std::move(tx));
                            nlinhas=1;
                            ncarac=0;
                            textomensagem.setPosition(300,710);
                            stringmensagem.clear();
                        }
                        else if(nlinhas!=1 || (nlinhas == 1 && ncarac>0))
                        {
                            stringmensagem+='\n';
                            ncarac=0;
                            nlinhas++;
                            if(nlinhas>4)
                                textomensagem.move(0,-23);
                        }
                        textomensagem.setString(stringmensagem);
                        break;
                    }
                    case sf::Keyboard::Escape:
                    {

                        Envio::EnviaFinal();
                        window.close();
                        running=false;
                        break;
                    }
                    case sf::Keyboard::Q:
                    {
                        if(event.key.alt)
                        {
                            {
                                if(ncarac==86)
                                {
                                    stringmensagem+='\t';
                                    stringmensagem+='\n';
                                    nlinhas++;
                                    if(nlinhas>4)
                                        textomensagem.move(0,-23);
                                    ncarac=0;
                                }
                                stringmensagem+='/';
                                ncarac++;
                            }
                            textomensagem.setString(stringmensagem);
                            break;
                        }
                    }
                    case sf::Keyboard::W:
                    {
                        if(event.key.alt)
                        {
                            {
                                if(ncarac==86)
                                {
                                    stringmensagem+='\t';
                                    stringmensagem+='\n';
                                    ncarac=0;
                                    nlinhas++;
                                    if(nlinhas>4)
                                        textomensagem.move(0,-23);
                                }
                                stringmensagem+='?';
                                ncarac++;
                            }
                            textomensagem.setString(stringmensagem);
                        }
                        break;
                    }
                    case sf::Keyboard::Up:
                    {
                        if(nlinhas>4&&textomensagem.getPosition().y<710)
                            textomensagem.move(0,23);
                        break;
                    }
                    case sf::Keyboard::Down:
                    {
                        sf::FloatRect retmensagem=textomensagem.getGlobalBounds();
                        retmensagem.top-=92;
                        if(nlinhas>4&&caixa.getGlobalBounds().intersects(retmensagem))
                            textomensagem.move(0,-23);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                    }
                    break;
                }
                case sf::Event::TextEntered:
                {
                    if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)&&!sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                    {
                        auto c=event.text.unicode;
                        if(c==27)
                            break;
                        alturabase=0;
                        scrolling=false;

                        if(c==8)
                        {
                            if(std::size(stringmensagem)>0)
                            {
                                if(stringmensagem.back()=='\n')
                                {
                                    stringmensagem.pop_back();
                                    nlinhas--;
                                    if(nlinhas>3)
                                        textomensagem.move(0,+23);
                                    if(stringmensagem.back()=='\t')
                                    {
                                        stringmensagem.pop_back();
                                        ncarac=86;
                                    }
                                    else
                                    {
                                        ncarac=DesfazEnter();
                                    }
                                }
                                else
                                {
                                    stringmensagem.pop_back();
                                    ncarac--;
                                    if(stringmensagem.back()=='\n')
                                    {
                                        stringmensagem.pop_back();
                                        nlinhas--;
                                        if(nlinhas>3)
                                            textomensagem.move(0,+23);
                                        if(stringmensagem.back()=='\t')
                                        {
                                            stringmensagem.pop_back();
                                            ncarac=86;
                                        }
                                        else
                                        {
                                            ncarac=DesfazEnter();
                                        }
                                    }
                                }
                            }
                        }
                        else if(c!=13)
                        {
                            if(ncarac==86)
                            {
                                stringmensagem+='\t';
                                stringmensagem+='\n';
                                ncarac=0;
                                nlinhas++;
                                if(nlinhas>4)
                                    textomensagem.move(0,-23);
                            }
                            stringmensagem+=c;
                            ncarac++;
                        }
                        textomensagem.setString(stringmensagem);

                    }
                    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::V))
                    {
                        Envio::GetFromClipboard();
                        textomensagem.setString(stringmensagem);
                    }
                    break;
                }
                case sf::Event::MouseWheelScrolled:
                {
                    delta=event.mouseWheelScroll.delta;
                    posimouse.x=event.mouseWheelScroll.x;
                    posimouse.y=event.mouseWheelScroll.y;
                    if(posimouse.x>300)
                    {
                        if(posimouse.y>710)
                        {
                            if(delta<0)
                                textomensagem.move(0,3*delta);
                            else if(textomensagem.getPosition().y<710)
                                textomensagem.move(0,3*delta);
                        }
                        else
                        {
                            if(delta>0&&offset<0&&alturabase<-offset)
                            {
                                alturabase+=15*delta;
                                while(alturabase>-offset)
                                    alturabase-=15;
                            }
                            else if(delta<0)
                            {
                                alturabase+=15*delta;
                            }
                            textomensagem.setPosition(300,710);
                            if(alturabase<0)
                                alturabase=0;
                            if(alturabase!=0)
                                scrolling=true;
                            else
                                scrolling=false;
                        }
                    }
                    else
                    {
                        if(delta>0)
                        {
                            scrollcontatos-=delta;
                            if(scrollcontatos<0)
                                scrollcontatos=0;
                        }
                        else
                        {
                            int tlist=ordemconversas.size();
                            if(tlist>8)
                            {
                                scrollcontatos-=delta;
                                while(tlist-scrollcontatos<8)
                                    scrollcontatos--;
                            }
                        }
                        auto ite=ordemconversas.begin();
                        std::advance(ite,scrollcontatos.load());
                        int a=0;
                        for(auto it=ite; it!=ordemconversas.end(); ++it)
                        {
                            idsconversas[a]=(*it);
                            auto& contatoatual=jogadores[(*it)];
                            textosidsconversas[a].setString(contatoatual.first);
                            a++;
                            if(a==8)
                                break;
                        }
                    }
                    break;
                }
                case sf::Event::MouseButtonPressed:
                {
                    posimouse=(sf::Vector2f)sf::Mouse::getPosition(window);
                    if(posimouse.x<300)
                    {
                        if(posimouse.y<480)
                        {
                            if(posimouse.y<320)
                            {
                                if(posimouse.y<240)
                                    MudaNome(idsconversas[0]);
                                else
                                    MudaNome(idsconversas[1]);
                            }
                            else
                            {
                                if(posimouse.y<400)
                                    MudaNome(idsconversas[2]);
                                else
                                    MudaNome(idsconversas[3]);
                            }
                        }
                        else
                        {
                            if(posimouse.y<640)
                            {
                                if(posimouse.y<560)
                                    MudaNome(idsconversas[4]);
                                else
                                    MudaNome(idsconversas[5]);
                            }
                            else
                            {
                                if(posimouse.y<720)
                                    MudaNome(idsconversas[6]);
                                else
                                    MudaNome(idsconversas[7]);
                            }
                        }
                    }
                    break;
                }
                default:
                {
                    break;
                }
                }
            }
            accumulator-=dt;
            drawn=false;
        }
        if(drawn)
            sf::sleep(sf::seconds(0.01f));
        else
        {
            drawn=1;
            window.clear(sf::Color(232,111,130));
            if(scrolling)
            {

                travachat.lock();
                for(auto it=fila.rbegin(); it!=fila.rend(); it++)
                {
                    if(it->posi>-alturabase)
                    {
                        controledesenho=true;
                        it->Desenha(window,alturabase);
                    }
                    else if(controledesenho)
                    {
                        controledesenho=false;
                        break;
                    }
                }
                travachat.unlock();

                window.draw(caixa);
                window.draw(textomensagem);
                for(int i=0; i<8; i++)
                {
                    window.draw(retcontato[i]);
                    window.draw(textosidsconversas[i]);
                    window.draw(textosjogo[i]);
                }
                window.draw(info);
                window.draw(idfixo);
                window.draw(nomefixo);
                window.draw(retcvatual);
            }
            else
            {
                window.draw(caixa);
                window.draw(textomensagem);
                window.draw(cobre);

                travachat.lock();

                for(auto it=fila.rbegin(); it!=fila.rend(); it++)
                {
                    if(it->posi>-alturabase)
                    {
                        controledesenho=true;
                        it->Desenha(window,alturabase);
                    }
                    else if(controledesenho)
                    {
                        controledesenho=false;
                        break;
                    }
                }
                travachat.unlock();
                for(int i=0; i<8; i++)
                {
                    window.draw(retcontato[i]);
                    window.draw(textosidsconversas[i]);
                    window.draw(textosjogo[i]);
                }
                window.draw(info);
                window.draw(idfixo);
                window.draw(nomefixo);
                window.draw(retcvatual);
            }
            window.display();
        }
    }
    t1.join();
    t2.join();
    return 0;
}
