import pygame
from configs import *
from cenas import CenaMenu, CenaJogo, CenaControle, CenaGameOver, CenaVitoria

class Jogo:
    def __init__(self):
        self.executando = True
        
        self.fundo = pygame.image.load("../cenario/fundo3.jpg").convert()
        self.fundo = pygame.transform.scale(self.fundo, (LARGURA, ALTURA))
        self.bg_y = 0 
        
        self.cena_atual = CenaMenu()
        self.cena_anterior = None

    def atualiza(self):
        nome_cena = self.cena_atual.__class__.__name__
        
        if nome_cena == "CenaJogo" and self.cena_atual.pausado:
            pass
        elif nome_cena == "CenaGameOver":
            pass
        else:
            self.bg_y += 1
            if self.bg_y >= ALTURA:
                self.bg_y = 0

        proxima_cena = self.cena_atual.atualiza()
        
        if proxima_cena == "CENA_JOGO":
            self.cena_atual = CenaJogo()
        elif proxima_cena == "CENA_JOGO_TESTE":
            self.cena_atual = CenaJogo(modo_teste=True)
        elif proxima_cena == "CENA_MENU":
            self.cena_atual = CenaMenu()
        elif proxima_cena == "CENA_CONTROLE":
            self.cena_anterior = self.cena_atual
            self.cena_atual = CenaControle()
        elif proxima_cena == "CENA_GAME_OVER":
            self.cena_atual = CenaGameOver()
        elif proxima_cena == "CENA_VITORIA":
            self.cena_atual = CenaVitoria()
        elif proxima_cena == "VOLTAR":
            if self.cena_anterior is not None:
                self.cena_atual = self.cena_anterior
                self.cena_anterior = None
            else:
                self.cena_atual = CenaMenu()

    def desenha(self, tela):
        tela.blit(self.fundo, (0, self.bg_y))
        tela.blit(self.fundo, (0, self.bg_y - ALTURA))
        
        self.cena_atual.desenha(tela)