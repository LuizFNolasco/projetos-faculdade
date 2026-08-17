import pygame
import random
from configs import *

class Inimigo:
    def __init__(self, tipo=1):
        self.tipo = tipo
        if tipo == 1:
            self.imagem = pygame.image.load("../personagens/inimigo1.png").convert_alpha()
        else:
            self.imagem = pygame.image.load("../personagens/inimigo2.png").convert_alpha()
            
        self.imagem = pygame.transform.scale(self.imagem, (100, 100))
            
        x_aleatorio = random.randint(50, LARGURA - 50)
        self.rect = self.imagem.get_rect(center=(x_aleatorio, -50))
        
        self.y_alvo = random.randint(50, ALTURA // 3)
        self.velocidade = 1.25
        self.vida = 10
        
        self.ultimo_tiro = pygame.time.get_ticks() + random.randint(500, 2000)
        self.cooldown_tiro = 1500 
        
        self.tiros_pendentes = 0
        self.ultimo_sub_tiro = 0
        self.cooldown_sub_tiro = 200 

    def atualiza(self, tiros_inimigos=None):
        if self.rect.y < self.y_alvo:
            self.rect.y += self.velocidade
        
        agora = pygame.time.get_ticks()
        
        if agora - self.ultimo_tiro > self.cooldown_tiro:
            if self.tipo == 1:
                if tiros_inimigos is not None:
                    from tiros import TiroInimigo
                    bico_x = self.rect.centerx
                    bico_y = self.rect.bottom
                    esq_x = self.rect.left + 10
                    esq_y = self.rect.centery
                    dir_x = self.rect.right - 10
                    dir_y = self.rect.centery
                    
                    tiros_inimigos.append(TiroInimigo(bico_x, bico_y))
                    tiros_inimigos.append(TiroInimigo(esq_x, esq_y))
                    tiros_inimigos.append(TiroInimigo(dir_x, dir_y))
                self.ultimo_tiro = agora
            elif self.tipo == 2:
                self.tiros_pendentes = 2
                self.ultimo_tiro = agora
                
        if self.tipo == 2 and self.tiros_pendentes > 0:
            if agora - self.ultimo_sub_tiro > self.cooldown_sub_tiro:
                if tiros_inimigos is not None:
                    from tiros import TiroInimigo
                    bico_x = self.rect.centerx
                    bico_y = self.rect.bottom
                    tiros_inimigos.append(TiroInimigo(bico_x, bico_y))
                self.tiros_pendentes -= 1
                self.ultimo_sub_tiro = agora

    def desenha(self, tela):
        tela.blit(self.imagem, self.rect)


class Chefao:
    def __init__(self):
        self.imagem = pygame.image.load("../personagens/chefao1.png").convert_alpha()
        self.imagem = pygame.transform.rotozoom(self.imagem, 0, 0.8)
        self.rect = self.imagem.get_rect(center=(LARGURA // 2, 160))
        self.velocidade_x = 3
        self.vida = 150
        self.vida_maxima = 150
        
        self.ultimo_tiro = pygame.time.get_ticks()
        self.cooldown_tiro = 1000 
        self.origem_atual = 0 

    def atualiza(self, tiros_inimigos=None):
        self.rect.x += self.velocidade_x
        if self.rect.right >= LARGURA - 120 or self.rect.left <= 120:
            self.velocidade_x *= -1
            
        agora = pygame.time.get_ticks()
        if agora - self.ultimo_tiro > self.cooldown_tiro:
            if tiros_inimigos is not None:
                from tiros import TiroInimigo
                base_y = self.rect.bottom
                
                if self.origem_atual == 0:
                    base_x = self.rect.centerx - 50
                    for i in range(8):
                        tiros_inimigos.append(TiroInimigo(base_x + (i * 15), base_y, dy=8, largura=15, altura=45))
                else:
                    base_x_esq = self.rect.left + 20
                    for i in range(4):
                        tiros_inimigos.append(TiroInimigo(base_x_esq + (i * 15), base_y, dy=8, largura=15, altura=45))
                        
                    base_x_dir = self.rect.right - 80
                    for i in range(4):
                        tiros_inimigos.append(TiroInimigo(base_x_dir + (i * 15), base_y, dy=8, largura=15, altura=45))
                
            self.origem_atual = 1 - self.origem_atual
            self.ultimo_tiro = agora

    def desenha(self, tela):
        tela.blit(self.imagem, self.rect)
        largura_barra_max = self.rect.width
        largura_barra_atual = max(0, int((self.vida / self.vida_maxima) * largura_barra_max))
        pygame.draw.rect(tela, (50, 50, 50), (self.rect.x, self.rect.y - 15, largura_barra_max, 10))
        if largura_barra_atual > 0:
            pygame.draw.rect(tela, (255, 0, 0), (self.rect.x, self.rect.y - 15, largura_barra_atual, 10))


class Chefao2:
    def __init__(self):
        self.imagem = pygame.image.load("../personagens/chefao2.png").convert_alpha()
        self.imagem = pygame.transform.rotozoom(self.imagem, 0, 0.8)
        self.rect = self.imagem.get_rect(center=(LARGURA // 2, 160))
        self.velocidade_x = 4
        self.vida = 250
        self.vida_maxima = 250
        
        self.ultimo_tiro = pygame.time.get_ticks()
        self.cooldown_tiro = 1600

    def atualiza(self, tiros_inimigos=None):
        self.rect.x += self.velocidade_x
        if self.rect.right >= LARGURA - 120 or self.rect.left <= 120:
            self.velocidade_x *= -1
            
        agora = pygame.time.get_ticks()
        
        if agora - self.ultimo_tiro > self.cooldown_tiro:
            if tiros_inimigos is not None:
                from tiros import TiroInimigo
                base_y = self.rect.bottom
                centro_x = self.rect.centerx
                
                espacamento = 25
                for i in range(-2, 3):
                    tiros_inimigos.append(TiroInimigo(centro_x + (i * espacamento), base_y, imagem_nome="laserRed08.png"))
                    
            self.ultimo_tiro = agora
            
    def desenha(self, tela):
        tela.blit(self.imagem, self.rect)
        largura_barra_max = self.rect.width
        largura_barra_atual = max(0, int((self.vida / self.vida_maxima) * largura_barra_max))
        pygame.draw.rect(tela, (50, 50, 50), (self.rect.x, self.rect.y - 15, largura_barra_max, 10))
        if largura_barra_atual > 0:
            pygame.draw.rect(tela, (255, 0, 0), (self.rect.x, self.rect.y - 15, largura_barra_atual, 10))


class ChefaoFinal:
    def __init__(self):
        self.imagem = pygame.image.load("../personagens/chefao3.png").convert_alpha()
        self.imagem = pygame.transform.rotozoom(self.imagem, 0, 0.8)
        self.rect = self.imagem.get_rect(center=(LARGURA // 2, 160))
        self.velocidade_x = 5
        self.vida = 400
        self.vida_maxima = 400
        
        self.ultimo_tiro = pygame.time.get_ticks()
        self.cooldown_tiro = 1100 
        self.tiros_pendentes = 0
        self.ultimo_sub_tiro = 0
        self.cooldown_sub_tiro = 150

    def atualiza(self, tiros_inimigos=None):
        self.rect.x += self.velocidade_x
        if self.rect.right >= LARGURA - 120 or self.rect.left <= 120:
            self.velocidade_x *= -1
            
        agora = pygame.time.get_ticks()
        
        if agora - self.ultimo_tiro > self.cooldown_tiro:
            self.tiros_pendentes = 3
            self.ultimo_tiro = agora
            
        if self.tiros_pendentes > 0:
            if agora - self.ultimo_sub_tiro > self.cooldown_sub_tiro:
                if tiros_inimigos is not None:
                    from tiros import TiroInimigo
                    bico_x = self.rect.centerx
                    bico_y = self.rect.bottom
                    
                    tiros_inimigos.append(TiroInimigo(bico_x, bico_y, dx=0, imagem_nome="laserRed10.png"))
                    
                    dx_oposto = -self.velocidade_x
                    tiros_inimigos.append(TiroInimigo(bico_x, bico_y, dx=dx_oposto, imagem_nome="laserRed10.png"))
                    
                self.tiros_pendentes -= 1
                self.ultimo_sub_tiro = agora
            
    def desenha(self, tela):
        tela.blit(self.imagem, self.rect)
        largura_barra_max = self.rect.width
        largura_barra_atual = max(0, int((self.vida / self.vida_maxima) * largura_barra_max))
        pygame.draw.rect(tela, (50, 50, 50), (self.rect.x, self.rect.y - 15, largura_barra_max, 10))
        if largura_barra_atual > 0:
            pygame.draw.rect(tela, (255, 0, 0), (self.rect.x, self.rect.y - 15, largura_barra_atual, 10))
