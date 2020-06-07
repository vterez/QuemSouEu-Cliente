#ifndef TEXTO_H
#define TEXTO_H

#include "Mensagem.h"
#include <string>
#include <windows.h>
#include <shellapi.h>

class Texto
{
public:
    Texto(std::wstring& s,sf::RectangleShape& r);
    virtual ~Texto() {};

    inline static sf::Text textopadrao;
    sf::RectangleShape ret;
    sf::Text texto;
    float posi;
    float offset;
    void Enquadra(int off);
    void Desenha(sf::RenderWindow& window,float off);
    template <typename T>
    sf::Vector2f poenomeio(T t)
    {
        if constexpr (std::is_base_of<sf::Transformable,T>::value)
        {
            sf::Vector2f posi;
            sf::FloatRect rect;
            rect=t.getGlobalBounds();
            posi.x=rect.left+(rect.width)/2;
            posi.y=rect.top+(rect.height)/2;
            return posi;
        }
        return sf::Vector2f(0,0);
    }
protected:

private:
};

#endif // TEXTO_H
