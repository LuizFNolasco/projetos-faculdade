import pygame
import random
from configs import *

class Moeda:
    def __init__(self):
        self.imagem = pygame.image.load("../cenario/moeda.gif").convert_alpha()
        self.imagem = pygame.transform.scale(self.imagem, (32, 32))
        
        x_aleatorio = random.randint(50, LARGURA - 50)
        self.rect = self.imagem.get_rect(center=(x_aleatorio, -50))
        
        self.velocidade = 3

    def atualiza(self):
        self.rect.y += self.velocidade

    def desenha(self, tela):
        tela.blit(self.imagem, self.rect)
