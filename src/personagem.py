import pygame
import random
from configs import *

class Personagem:
    def __init__(self):
        self.imagem = pygame.image.load("../personagens/nave_principal.png").convert_alpha()
        self.imagem = pygame.transform.scale(self.imagem, (50, 50))
        self.rect = self.imagem.get_rect(center=(LARGURA // 2, ALTURA - 80))
        self.velocidade = 5
        self.tipo_nave = "nave_principal"
        self.vida = 8
        
        self.lista_tiros = []
        self.ultimo_tiro = 0
        self.cooldown_tiro = 250 
        
    def mudar_nave(self):
        opcoes = ["nave_principal", "nave_tiro_duplo", "nave_tiro_largo"]
        aux = self.tipo_nave

        opcoes.remove(aux)
        self.tipo_nave = random.choice(opcoes)

        opcoes.append(aux)
        
        centro_atual = self.rect.center
        self.imagem = pygame.image.load(f"../personagens/{self.tipo_nave}.png").convert_alpha()
        self.imagem = pygame.transform.scale(self.imagem, (50, 50))
        self.rect = self.imagem.get_rect(center=centro_atual)

    def atualiza(self):
        teclas = pygame.key.get_pressed()
        
        if teclas[pygame.K_LEFT] or teclas[pygame.K_a]:
            self.rect.x -= self.velocidade
        if teclas[pygame.K_RIGHT] or teclas[pygame.K_d]:
            self.rect.x += self.velocidade
        if teclas[pygame.K_UP] or teclas[pygame.K_w]:
            self.rect.y -= self.velocidade
        if teclas[pygame.K_DOWN] or teclas[pygame.K_s]:
            self.rect.y += self.velocidade

        if self.rect.left < 0:
            self.rect.left = 0
        if self.rect.right > LARGURA:
            self.rect.right = LARGURA
        if self.rect.top < 0:
            self.rect.top = 0
        if self.rect.bottom > ALTURA:
            self.rect.bottom = ALTURA

        agora = pygame.time.get_ticks()
        if (teclas[pygame.K_SPACE] or teclas[pygame.K_RETURN]) and agora - self.ultimo_tiro > self.cooldown_tiro:
            self.atirar()
            self.ultimo_tiro = agora

        for tiro in self.lista_tiros[:]:
            tiro.atualiza()
            if tiro.rect.bottom < 0:
                self.lista_tiros.remove(tiro)

    def atirar(self):
        from tiros import Tiro
        
        som_tiro = pygame.mixer.Sound("../sons/tiro_normal.wav")
        som_tiro.set_volume(0.3)
        som_tiro.play()
        
        bico_x = self.rect.centerx
        bico_y = self.rect.top
        
        esq_x = self.rect.left + 5
        esq_y = self.rect.centery
        
        dir_x = self.rect.right - 5
        dir_y = self.rect.centery
        
        if self.tipo_nave == "nave_principal":
            self.lista_tiros.append(Tiro(bico_x, bico_y))
        elif self.tipo_nave == "nave_tiro_duplo":
            self.lista_tiros.append(Tiro(esq_x, esq_y))
            self.lista_tiros.append(Tiro(dir_x, dir_y))
        elif self.tipo_nave == "nave_tiro_largo":
            self.lista_tiros.append(Tiro(bico_x, bico_y))
            self.lista_tiros.append(Tiro(esq_x, esq_y))
            self.lista_tiros.append(Tiro(dir_x, dir_y))

    def desenha(self, tela):
        for tiro in self.lista_tiros:
            tiro.desenha(tela)
        tela.blit(self.imagem, self.rect)
