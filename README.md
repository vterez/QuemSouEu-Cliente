# QuemSouEu-Cliente
Juntamente com o QuemSouEu-Server, utiliza a biblioteca SFML para implementar o jogo Quem Sou Eu (escolhe um personagem, escreve e cola na testa e tem que adivinhar).

Inicialize o servidor. No console, serão informados os IP's público e privado. Na pasta do cliente, edite o arquivo config.txt com o IP adequado: se o cliente estiver na mesma rede do servidor, coloque o IP privado, caso contrário, coloque o público (lembrando de configurar o redirecionamento de porta).
Inicialize o cliente. Será solicitado um nome de usuário. Após inserir o nome, a tela do jogo abrirá. Ela parece um chat, pois o jogo tem uma interface parecida com um chat.
Cada usuário do jogo vai aparecer num bloquinho do lado esquerdo. Quando todos os usuários tiverem conectado, algum deles deve pressionar F1 para o jogo solicitar, para cada usuário, o nome do personagem de outro jogador para a rodada atual e para bloquear acessos novos à sala.
Após todos os jogadores escreverem, um deles deve apertar F2 para os personagens serem enviados para todos os jogadores. Cada jogador tem acesso a todos os personagens, menos o próprio.
Quando a rodada terminar, um jogador deve apertar F3 para abrir a sala para novos jogadores. Então basta pressionar F1 para começar uma nova rodada.
Ao final do jogo, basta fechar a janela. Quando todos os jogadores sairem, o servidor se encerrará após alguns segundos.

