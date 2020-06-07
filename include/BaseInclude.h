#ifndef BASEINCLUDE_H_INCLUDED
#define BASEINCLUDE_H_INCLUDED

#include <memory>
#include <thread>
#include <string>
#include <list>
#include <unordered_map>
#include <atomic>
#include <mutex>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <windows.h>
#include "Texto.h"
#include <fstream>
#include <iostream>
#include "Contato.h"

//soquete
extern sf::TcpSocket soquete;

//controle de fluxo
extern std::atomic<bool> running;//bool para parar o recebe e o check
extern std::atomic<bool> enviando;//bool pra lançar thread de envio
extern std::atomic<bool> idnaoexiste;//estado de tentou conectar com id inexistente
extern std::atomic<int> scrollcontatos;
extern std::mutex travachat,travaconversas;//mutex pra travar mudancas no chat atual

//armazenamento de mensagens
extern std::list<Texto> fila;//fila de mensagens para envio
extern std::list<int> ordemconversas;//ordem das conversas da barra lateral
extern int idsconversas[8];
extern int id;
extern float offset;
extern std::unordered_map<int,std::pair<std::wstring,std::wstring>> jogadores;

//manipulacao de texto
extern int ncarac,nlinhas; //numero de caracteres e de linhas da mensagem a enviar, para fins de formatacao
extern std::wstring stringmensagem; //wstring da mensagem a enviar
extern std::wstring nome,envianome;
extern sf::Font fonte;//fonte padrao
extern sf::Text textomensagem;//texto da mensagem a enviar
extern sf::Text textosidsconversas[8];
extern sf::Text textosjogo[8];
extern sf::Text antesdigitarnome;//info para digitar nome
extern sf::Text digitarnome;//texto enquanto digita o nome
extern sf::Text nomefixo;
extern sf::Text idfixo;

//botoes e caixas de texto
extern sf::RectangleShape retpadrao1,retpadrao2;//retangulos das mensagens esquerda e direita
extern sf::RectangleShape caixa;//caixa de texto da mensagem
extern sf::RectangleShape cobre;//retangulo para cobrir mensagens grandes
extern sf::RectangleShape retcontato[8];//conversas
extern sf::RectangleShape info;//parte de cima
extern sf::RectangleShape retnome;//barra para digitar nome
extern sf::RectangleShape retcvatual;//barra verde com o nome da cvatual

//limites de sprites

//manipulacao de imagens

bool Inicializa();

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
};
int DesfazEnter();
void MudaNome(int user);
#endif // BASEINCLUDE_H_INCLUDED

