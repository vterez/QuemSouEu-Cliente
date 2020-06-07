#include "Texto.h"
#include <iostream>
Texto::Texto(std::wstring& s,sf::RectangleShape& r)
{
    texto=textopadrao;
    texto.setString(s);
    ret=r;
    sf::Rect recttext=texto.getLocalBounds();
    ret.setSize(sf::Vector2f(recttext.width+20,recttext.height+15));
    this->posi=705;

    ret.setOrigin(0,recttext.height+15);
    ret.setPosition(305,705);
    texto.setOrigin(0,recttext.top+recttext.height);
    texto.setPosition(313,698);
    offset=recttext.height+15;
}

void Texto::Enquadra(int off)
{
    texto.move(0,-10-off);
    ret.move(0,-10-off);
    this->posi-=(10+off);
}

void Texto::Desenha(sf::RenderWindow& window,float off)
{
    if(off!=0)
    {
        ret.move(0,off);
        texto.move(0,off);
    }
    window.draw(ret);
    window.draw(texto);
    if(off!=0)
    {
        ret.move(0,-off);
        texto.move(0,-off);
    }
};

