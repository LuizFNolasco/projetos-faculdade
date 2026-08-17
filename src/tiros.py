import pygame
from configs import *

class Tiro:
    def __init__(self, x, y, dx=0, dy=-10):
        self.imagem = pygame.image.load("../tiros/laserBlue01.png").convert_alpha()
        self.imagem = pygame.transform.scale(self.imagem, (10, 30))
        self.rect = self.imagem.get_rect(center=(x, y))
        self.dx = dx
        self.dy = dy
        
    def atualiza(self):
        self.rect.x += self.dx
        self.rect.y += self.dy
        
    def desenha(self, tela):
        tela.blit(self.imagem, self.rect)


class TiroInimigo:
    def __init__(self, x, y, dx=0, dy=6, largura=10, altura=30, imagem_nome="laserRed01.png"):
        self.imagem = pygame.image.load(f"../tiros/{imagem_nome}").convert_alpha()
        self.imagem = pygame.transform.scale(self.imagem, (largura, altura))
        self.rect = self.imagem.get_rect(center=(x, y))
        self.dx = dx
        self.dy = dy
        
    def atualiza(self):
        self.rect.x += self.dx
        self.rect.y += self.dy
        
    def desenha(self, tela):
        tela.blit(self.imagem, self.rect)
