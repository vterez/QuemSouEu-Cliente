#include "Envio.h"
#include <iostream>
#include <fstream>
using namespace Envio;

void Envio::FormataTexto()
{
    std::wstring formatado;
    int tam=std::size(stringmensagem);
    ncarac=0;
    nlinhas=1;
    int ncaracfinal=0;
    short int ultimoespaco=-1;
    for(int i=0; i<tam; i++)
    {
        wchar_t c;
        if(ncarac==27)
        {
            if(ultimoespaco!=-1)
            {
                formatado[ultimoespaco]='\n';
                ncarac=26-ultimoespaco;
                nlinhas++;
            }
            else
            {
                formatado+='\n';
                ncaracfinal++;
                ncarac=0;
                nlinhas++;
            }
        }
        c=stringmensagem[i];
        if(c=='\t'&&stringmensagem[i+1]=='\n')
            i++;
        else
        {
            formatado+=c;
            ncarac++;
            ncaracfinal++;
            if(c=='\n')
            {
                ncarac=0;
                nlinhas++;
            }
            if(c==' ')
                ultimoespaco=ncaracfinal-1;
        }
    }
    stringmensagem=formatado;
}


bool Envio::GetFromClipboard()
{
    std::wstring s=sf::Clipboard::getString();
    if(!s.empty())
    {
        for(auto &c:s)
        {
            if(ncarac>=86)
            {
                stringmensagem+='\t';
                stringmensagem+='\n';
                nlinhas++;
                ncarac=0;
            }
            if(c=='\n')
            {
                stringmensagem+='\n';
                nlinhas++;
                ncarac=0;
            }
            else if(c=='\t')
            {
                stringmensagem+=' ';
                stringmensagem+=' ';
                stringmensagem+=' ';
                stringmensagem+=' ';
                ncarac+=4;
            }
            else if(c>9)
            {
                stringmensagem+=c;
                ncarac++;
            }
        }
        return 0;
    }
    return 0;
}
void Envio::EnviaFinal()
{
    sf::Packet pacote;
    pacote<<6;
    soquete.send(pacote);
}

void Envio::EnviaCheck()
{
    while(running)
    {
        sf::sleep(sf::seconds(5.f));
        sf::Packet pacote;
        pacote<<5;
        soquete.send(pacote);
    }
}
