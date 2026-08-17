import pygame
from configs import *
from logica import Jogo

pygame.init()
pygame.display.init()
janela = pygame.display.set_mode((LARGURA, ALTURA))
pygame.display.set_caption(TITULO_JOGO)

relogio = pygame.time.Clock()
jogo = Jogo()

while jogo.executando:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            jogo.executando = False

    jogo.atualiza()

    jogo.desenha(janela)

    pygame.display.update()
    relogio.tick(FPS)

pygame.quit()