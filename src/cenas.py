import pygame
import random
from configs import *
from personagem import Personagem
from inimigos import Inimigo, Chefao, Chefao2, ChefaoFinal
from itens import Moeda

class CenaControle:
    def __init__(self):
        self.fundo = pygame.image.load("../imagens_principais/cena_instrucoes.png").convert_alpha()
        self.fundo = pygame.transform.scale(self.fundo, (LARGURA, ALTURA))
        self.fundo_rect = self.fundo.get_rect(center=(LARGURA // 2, ALTURA // 2))
        
        self.fonte_titulo = pygame.font.Font("../fontes/8bitoperator.ttf", 45)
        self.texto_titulo = self.fonte_titulo.render("CONTROLES", True, PRETO)
        self.titulo_rect = self.texto_titulo.get_rect(center=(LARGURA // 2, 120))
        
        self.fonte_normal = pygame.font.Font("../fontes/8bitoperator.ttf", 20)
        self.texto_ou = self.fonte_normal.render("ou", True, PRETO)
        
        def carrega(nome):
            img = pygame.image.load(f"../instrucoes/{nome}").convert_alpha()
            return pygame.transform.scale(img, (32, 32))
            
        self.img_w = carrega("tile_0358.png")
        self.img_a = carrega("tile_0392.png")
        self.img_s = carrega("tile_0393.png")
        self.img_d = carrega("tile_0394.png")
        
        self.img_up = carrega("tile_0438.png")
        self.img_down = carrega("tile_0439.png")
        self.img_left = carrega("tile_0440.png")
        self.img_right = carrega("tile_0441.png")
        self.img_seta = carrega("seta.png")
        
        space_surf = pygame.Surface((96, 32), pygame.SRCALPHA)
        space_surf.blit(carrega("tile_0507.png"), (0, 0))
        space_surf.blit(carrega("tile_0508.png"), (32, 0))
        space_surf.blit(carrega("tile_0509.png"), (64, 0))
        self.img_space = space_surf
        
        enter_surf = pygame.Surface((64, 64), pygame.SRCALPHA)
        enter_surf.blit(carrega("tile_0372.png"), (0, 0))
        enter_surf.blit(carrega("tile_0373.png"), (32, 0))
        enter_surf.blit(carrega("tile_0406.png"), (0, 32))
        enter_surf.blit(carrega("tile_0407.png"), (32, 32))
        self.img_enter = enter_surf
        
        self.img_esc = carrega("tile_0289.png")
        
        self.esq_linhas = [
            (self.img_w, self.img_up, ["Mover a nave", "para cima"]),
            (self.img_s, self.img_down, ["Mover a nave", "para baixo"]),
            (self.img_a, self.img_left, ["Mover a nave", "para a esquerda"]),
            (self.img_d, self.img_right, ["Mover a nave", "para a direita"]),
            (self.img_esc, None, ["Pausar o jogo", "/ Voltar"])
        ]
        
        self.dir_linhas = [
            (self.img_space, None, ["Atirar /", "Confirmar"]),
            (self.img_enter, None, ["Atirar /", "Confirmar"])
        ]
        
        self.esc_pressionado = False

    def atualiza(self):
        teclas = pygame.key.get_pressed()
        if teclas[pygame.K_ESCAPE]:
            self.esc_pressionado = True
        elif self.esc_pressionado:
            som_click = pygame.mixer.Sound("../sons/click.wav")
            som_click.set_volume(0.2)
            som_click.play()
            return "VOLTAR"
        return None

    def desenha(self, tela):
        tela.blit(self.fundo, self.fundo_rect)
        tela.blit(self.texto_titulo, self.titulo_rect)
        
        def desenha_coluna(linhas, x_base, y_inicio, passo_y=80):
            y_atual = y_inicio
            for img1, img2, texto_linhas in linhas:
                tela.blit(img1, (x_base, y_atual))
                
                offset_x = x_base + img1.get_width() + 5
                center_y = y_atual + 16
                
                if img2 is not None:
                    rect_ou = self.texto_ou.get_rect(midleft=(offset_x, center_y))
                    tela.blit(self.texto_ou, rect_ou)
                    offset_x += self.texto_ou.get_width() + 5
                    
                    rect_img2 = img2.get_rect(midleft=(offset_x, center_y))
                    tela.blit(img2, rect_img2)
                    offset_x += img2.get_width() + 5
                    
                    rect_seta = self.img_seta.get_rect(midleft=(offset_x, center_y))
                    tela.blit(self.img_seta, rect_seta)
                    offset_x += self.img_seta.get_width() + 5
                else:
                    rect_seta = self.img_seta.get_rect(midleft=(offset_x, center_y))
                    tela.blit(self.img_seta, rect_seta)
                    offset_x += self.img_seta.get_width() + 5
                    
                for i, linha in enumerate(texto_linhas):
                    txt_render = self.fonte_normal.render(linha, True, PRETO)
                    tela.blit(txt_render, (offset_x + 5, y_atual + (i * 25) - 5))
                    
                y_atual += passo_y

        desenha_coluna(self.esq_linhas, 110, 150, 80)
        desenha_coluna(self.dir_linhas, 450, 197, 160)


class CenaMenu:
    def __init__(self):
        self.pergaminho = pygame.image.load("../imagens_principais/pergaminho.png").convert_alpha()
        self.pergaminho = pygame.transform.scale(self.pergaminho, (400, 500))
        self.perg_rect = self.pergaminho.get_rect(center=(LARGURA // 2, ALTURA // 2))
        
        self.fonte_titulo = pygame.font.Font("../fontes/8bitoperator.ttf", 45)
        self.texto_nome_1 = self.fonte_titulo.render("ANCIENT", True, PRETO)
        self.nome_rect_1 = self.texto_nome_1.get_rect(center=(LARGURA // 2, ALTURA // 2 - 120))
        self.texto_nome_2 = self.fonte_titulo.render("SHOOTER", True, PRETO)
        self.nome_rect_2 = self.texto_nome_2.get_rect(center=(LARGURA // 2, ALTURA // 2 - 70))

        self.fonte_botao = pygame.font.Font("../fontes/8bitoperator.ttf", 20)
        
        self.texto_iniciar = self.fonte_botao.render("Iniciar", True, PRETO) 
        self.iniciar_rect = self.texto_iniciar.get_rect(center=(LARGURA // 2, ALTURA // 2 + 10))
        self.botao_rect = self.iniciar_rect.inflate(40, 20) 
        
        self.texto_teste = self.fonte_botao.render("Modo Teste", True, PRETO)
        self.teste_rect = self.texto_teste.get_rect(center=(LARGURA // 2, ALTURA // 2 + 70))
        self.botao_teste_rect = self.teste_rect.inflate(40, 20)
        
        self.texto_controle = self.fonte_botao.render("Controles", True, PRETO) 
        self.controle_rect = self.texto_controle.get_rect(center=(LARGURA // 2, ALTURA // 2 + 130))
        self.botao_ctrl_rect = self.controle_rect.inflate(40, 20) 
        
        self.opcao_selecionada = 0
        self.debounce_up = False
        self.debounce_down = False
        self.debounce_enter = True
        
        self.saindo = False

    def atualiza(self):
        teclas = pygame.key.get_pressed()
        
        if teclas[pygame.K_UP] or teclas[pygame.K_w]:
            if not self.debounce_up:
                self.opcao_selecionada -= 1
                if self.opcao_selecionada < 0:
                    self.opcao_selecionada = 2
                self.debounce_up = True
        else:
            self.debounce_up = False
            
        if teclas[pygame.K_DOWN] or teclas[pygame.K_s]:
            if not self.debounce_down:
                self.opcao_selecionada += 1
                if self.opcao_selecionada > 2:
                    self.opcao_selecionada = 0
                self.debounce_down = True
        else:
            self.debounce_down = False
            
        if teclas[pygame.K_SPACE] or teclas[pygame.K_RETURN]:
            if not self.debounce_enter:
                som_click = pygame.mixer.Sound("../sons/click.wav")
                som_click.set_volume(0.2)
                som_click.play()
                if self.opcao_selecionada == 0:
                    return "CENA_JOGO"
                elif self.opcao_selecionada == 1:
                    return "CENA_JOGO_TESTE"
                elif self.opcao_selecionada == 2:
                    return "CENA_CONTROLE"
                self.debounce_enter = True
        else:
            self.debounce_enter = False
            
        return None

    def desenha(self, tela):
        tela.blit(self.pergaminho, self.perg_rect)
        tela.blit(self.texto_nome_1, self.nome_rect_1)
        tela.blit(self.texto_nome_2, self.nome_rect_2)
        
        if self.opcao_selecionada == 0:
            pygame.draw.rect(tela, (139, 100, 50), self.botao_rect, 3)
        elif self.opcao_selecionada == 1:
            pygame.draw.rect(tela, (139, 100, 50), self.botao_teste_rect, 3)
        elif self.opcao_selecionada == 2:
            pygame.draw.rect(tela, (139, 100, 50), self.botao_ctrl_rect, 3)
            
        tela.blit(self.texto_iniciar, self.iniciar_rect)
        tela.blit(self.texto_teste, self.teste_rect)
        tela.blit(self.texto_controle, self.controle_rect)


class CenaJogo:
    def __init__(self, modo_teste=False):
        self.modo_teste = modo_teste
        self.img_vida_cheia = pygame.image.load("../imagens_principais/vida_cheia.png").convert_alpha()
        self.img_vida_cheia = pygame.transform.scale(self.img_vida_cheia, (35, 35))
        self.img_vida_metade = pygame.image.load("../imagens_principais/vida_metade.png").convert_alpha()
        self.img_vida_metade = pygame.transform.scale(self.img_vida_metade, (35, 35))
        
        self.jogador = Personagem()
        self.tiros_inimigos = []
        self.lista_inimigos = []
        self.chefao = None
        self.chefao_spawnado = False
        self.inimigos_mortos = 0
        
        self.onda_atual = 1
        
        self.moeda = None
        
        self.pergaminho = pygame.image.load("../imagens_principais/pergaminho.png").convert_alpha()
        self.pergaminho = pygame.transform.scale(self.pergaminho, (400, 500))
        self.perg_rect = self.pergaminho.get_rect(center=(LARGURA // 2, ALTURA // 2))
        
        self.pausado = False
        self.esc_pressionado_antes = False
        
        self.fonte_pause = pygame.font.Font("../fontes/8bitoperator.ttf", 45)
        self.texto_pause = self.fonte_pause.render("PAUSADO", True, PRETO)
        self.pause_rect = self.texto_pause.get_rect(center=(LARGURA // 2, ALTURA // 2 - 120))
        
        self.fonte_botao = pygame.font.Font("../fontes/8bitoperator.ttf", 20)
        self.texto_opt1 = self.fonte_botao.render("Menu", True, PRETO)
        self.opt1_rect = self.texto_opt1.get_rect(center=(LARGURA // 2, ALTURA // 2))
        self.botao_opt1_rect = self.opt1_rect.inflate(40, 20)
        
        self.texto_opt2 = self.fonte_botao.render("Reiniciar", True, PRETO)
        self.opt2_rect = self.texto_opt2.get_rect(center=(LARGURA // 2, ALTURA // 2 + 50))
        self.botao_opt2_rect = self.opt2_rect.inflate(40, 20)
        
        self.texto_opt3 = self.fonte_botao.render("Controles", True, PRETO)
        self.opt3_rect = self.texto_opt3.get_rect(center=(LARGURA // 2, ALTURA // 2 + 100))
        self.botao_opt3_rect = self.opt3_rect.inflate(40, 20)
        
        self.opcao_pause = 0
        self.debounce_up_pause = False
        self.debounce_down_pause = False
        self.debounce_enter_pause = True
        
        self.iniciar_onda()

    def iniciar_onda(self):
        self.lista_inimigos = []
        self.chefao = None
        self.chefao_spawnado = False
        
        if self.onda_atual in [1, 4, 7]:
            i1 = Inimigo(1)
            i1.rect.x = 100
            i1.rect.y = -100
            i1.y_alvo = 100
            
            i2 = Inimigo(2)
            i2.rect.x = LARGURA - 150
            i2.rect.y = -100
            i2.y_alvo = 100
            
            self.lista_inimigos.extend([i1, i2])
            
        elif self.onda_atual in [2, 5, 8]:
            for i in range(3):
                inim = Inimigo(random.choice([1, 2]))
                inim.rect.x = (LARGURA // 2 - 100) + (i * 100)
                inim.rect.y = -100
                inim.y_alvo = 150
                self.lista_inimigos.append(inim)
                
        elif self.onda_atual == 3:
            self.chefao = Chefao()
            self.chefao_spawnado = True
            
            i1 = Inimigo(1)
            i1.rect.x = 50
            i1.rect.y = -100
            i1.y_alvo = 50
            
            i2 = Inimigo(2)
            i2.rect.x = LARGURA - 100
            i2.rect.y = -100
            i2.y_alvo = 50
            self.lista_inimigos.extend([i1, i2])
            
        elif self.onda_atual == 6:
            self.chefao = Chefao2()
            self.chefao_spawnado = True
            
            i1 = Inimigo(1)
            i1.rect.x = 50
            i1.rect.y = -100
            i1.y_alvo = 50
            
            i2 = Inimigo(2)
            i2.rect.x = LARGURA - 100
            i2.rect.y = -100
            i2.y_alvo = 50
            self.lista_inimigos.extend([i1, i2])
            
        elif self.onda_atual == 9:
            self.chefao = ChefaoFinal()
            self.chefao_spawnado = True
            
            i1 = Inimigo(1)
            i1.rect.x = 50
            i1.rect.y = -100
            i1.y_alvo = 50
            
            i2 = Inimigo(2)
            i2.rect.x = LARGURA - 100
            i2.rect.y = -100
            i2.y_alvo = 50
            self.lista_inimigos.extend([i1, i2])

    def atualiza(self):
        teclas = pygame.key.get_pressed()
        
        if teclas[pygame.K_ESCAPE]:
            if not self.esc_pressionado_antes:
                self.pausado = not self.pausado
                self.esc_pressionado_antes = True
        else:
            self.esc_pressionado_antes = False
            
        if self.pausado:
            if teclas[pygame.K_UP] or teclas[pygame.K_w]:
                if not self.debounce_up_pause:
                    self.opcao_pause -= 1
                    if self.opcao_pause < 0:
                        self.opcao_pause = 2
                    self.debounce_up_pause = True
            else:
                self.debounce_up_pause = False
                
            if teclas[pygame.K_DOWN] or teclas[pygame.K_s]:
                if not self.debounce_down_pause:
                    self.opcao_pause += 1
                    if self.opcao_pause > 2:
                        self.opcao_pause = 0
                    self.debounce_down_pause = True
            else:
                self.debounce_down_pause = False
                
            if teclas[pygame.K_SPACE] or teclas[pygame.K_RETURN]:
                if not self.debounce_enter_pause:
                    som_click = pygame.mixer.Sound("../sons/click.wav")
                    som_click.set_volume(0.2)
                    som_click.play()
                    if self.opcao_pause == 0:
                        return "CENA_MENU"
                    elif self.opcao_pause == 1:
                        return "CENA_JOGO"
                    elif self.opcao_pause == 2:
                        return "CENA_CONTROLE"
                    self.debounce_enter_pause = True
            else:
                self.debounce_enter_pause = False
            
            return None
            
        self.jogador.atualiza()
        
        if self.moeda is None:
            if random.randint(1, 1000) <= 5:
                self.moeda = Moeda()
        else:
            self.moeda.atualiza()
            if self.moeda.rect.top > ALTURA:
                self.moeda = None
            elif self.jogador.rect.colliderect(self.moeda.rect):
                som_upgrade = pygame.mixer.Sound("../sons/upgrade.wav")
                som_upgrade.set_volume(0.3)
                som_upgrade.play()
                self.jogador.mudar_nave()
                self.moeda = None

        if self.chefao is not None:
            self.chefao.atualiza(self.tiros_inimigos)
            
            for tiro in self.jogador.lista_tiros[:]:
                if tiro.rect.colliderect(self.chefao.rect):
                    if tiro in self.jogador.lista_tiros:
                        self.jogador.lista_tiros.remove(tiro)
                    self.chefao.vida -= 1
                    
            if self.chefao.vida <= 0:
                self.chefao = None
                self.chefao_spawnado = False
                    
        for tiro in self.tiros_inimigos[:]:
            tiro.atualiza()
            if tiro.rect.top > ALTURA:
                self.tiros_inimigos.remove(tiro)
            elif tiro.rect.colliderect(self.jogador.rect):
                if not self.modo_teste: self.jogador.vida -= 1
                self.tiros_inimigos.remove(tiro)
        
        for inimigo in self.lista_inimigos[:]:
            inimigo.atualiza(self.tiros_inimigos)
            
            for tiro in self.jogador.lista_tiros[:]:
                if tiro.rect.colliderect(inimigo.rect):
                    if tiro in self.jogador.lista_tiros:
                        self.jogador.lista_tiros.remove(tiro)
                    inimigo.vida -= 1
                    
            if inimigo.vida <= 0:
                self.inimigos_mortos += 1
                self.lista_inimigos.remove(inimigo)
                continue
                    
            if self.jogador.rect.colliderect(inimigo.rect):
                if not self.modo_teste: self.jogador.vida -= 1
                self.lista_inimigos.remove(inimigo)
                continue
                    
            if inimigo.rect.top > ALTURA:
                inimigo.rect.y = -100
                    
        if self.jogador.vida <= 0:
            return "CENA_GAME_OVER"
            
        if len(self.lista_inimigos) == 0 and self.chefao is None:
            self.onda_atual += 1
            if self.onda_atual > 9:
                return "CENA_VITORIA"
            else:
                self.iniciar_onda()
                
        return None

    def desenha(self, tela):
        self.jogador.desenha(tela)
        
        if self.moeda is not None:
            self.moeda.desenha(tela)
        
        if self.chefao is not None:
            self.chefao.desenha(tela)
        
        for inimigo in self.lista_inimigos:
            inimigo.desenha(tela)
            
        for tiro in self.tiros_inimigos:
            tiro.desenha(tela)

        espaco_x = 10
        for i in range(4):
            if self.jogador.vida >= (i + 1) * 2:
                tela.blit(self.img_vida_cheia, (espaco_x + (i * 40), 10))
            elif self.jogador.vida == (i * 2) + 1:
                tela.blit(self.img_vida_metade, (espaco_x + (i * 40), 10))

        if self.pausado:
            s = pygame.Surface((LARGURA, ALTURA))
            s.set_alpha(150)
            s.fill((0, 0, 0))
            tela.blit(s, (0,0))
            
            tela.blit(self.pergaminho, self.perg_rect)
            tela.blit(self.texto_pause, self.pause_rect)
            
            if self.opcao_pause == 0:
                pygame.draw.rect(tela, (139, 100, 50), self.botao_opt1_rect, 3)
            elif self.opcao_pause == 1:
                pygame.draw.rect(tela, (139, 100, 50), self.botao_opt2_rect, 3)
            elif self.opcao_pause == 2:
                pygame.draw.rect(tela, (139, 100, 50), self.botao_opt3_rect, 3)
                
            tela.blit(self.texto_opt1, self.opt1_rect)
            tela.blit(self.texto_opt2, self.opt2_rect)
            tela.blit(self.texto_opt3, self.opt3_rect)

class CenaGameOver:
    def __init__(self):
        self.pergaminho = pygame.image.load("../imagens_principais/pergaminho.png").convert_alpha()
        self.pergaminho = pygame.transform.scale(self.pergaminho, (400, 500))
        self.perg_rect = self.pergaminho.get_rect(center=(LARGURA // 2, ALTURA // 2))
        
        self.fonte_titulo = pygame.font.Font("../fontes/8bitoperator.ttf", 45)
        self.texto_titulo = self.fonte_titulo.render("DERROTA", True, VERMELHO)
        self.titulo_rect = self.texto_titulo.get_rect(center=(LARGURA // 2, ALTURA // 2 - 80))

        self.fonte_botao = pygame.font.Font("../fontes/8bitoperator.ttf", 20)
        
        self.texto_jogar = self.fonte_botao.render("Jogar", True, PRETO) 
        self.jogar_rect = self.texto_jogar.get_rect(center=(LARGURA // 2, ALTURA // 2 + 20))
        self.botao_jogar_rect = self.jogar_rect.inflate(40, 20) 
        
        self.texto_menu = self.fonte_botao.render("Menu", True, PRETO) 
        self.menu_rect = self.texto_menu.get_rect(center=(LARGURA // 2, ALTURA // 2 + 80))
        self.botao_menu_rect = self.menu_rect.inflate(40, 20) 
        
        self.opcao_selecionada = 0
        self.debounce_up = False
        self.debounce_down = False
        self.debounce_enter = True
        
        som_morte = pygame.mixer.Sound("../sons/morte.wav")
        som_morte.play()

    def atualiza(self):
        teclas = pygame.key.get_pressed()
        
        if teclas[pygame.K_UP] or teclas[pygame.K_w]:
            if not self.debounce_up:
                self.opcao_selecionada -= 1
                if self.opcao_selecionada < 0:
                    self.opcao_selecionada = 2
                self.debounce_up = True
        else:
            self.debounce_up = False
            
        if teclas[pygame.K_DOWN] or teclas[pygame.K_s]:
            if not self.debounce_down:
                self.opcao_selecionada += 1
                if self.opcao_selecionada > 2:
                    self.opcao_selecionada = 0
                self.debounce_down = True
        else:
            self.debounce_down = False
            
        if teclas[pygame.K_SPACE] or teclas[pygame.K_RETURN]:
            if not self.debounce_enter:
                som_click = pygame.mixer.Sound("../sons/click.wav")
                som_click.set_volume(0.2)
                som_click.play()
                if self.opcao_selecionada == 0:
                    return "CENA_JOGO"
                elif self.opcao_selecionada == 1:
                    return "CENA_MENU"
                self.debounce_enter = True
        else:
            self.debounce_enter = False
            
        return None

    def desenha(self, tela):
        tela.fill(PRETO)
        tela.blit(self.pergaminho, self.perg_rect)
        tela.blit(self.texto_titulo, self.titulo_rect)
        
        if self.opcao_selecionada == 0:
            pygame.draw.rect(tela, (139, 100, 50), self.botao_jogar_rect, 3)
        elif self.opcao_selecionada == 1:
            pygame.draw.rect(tela, (139, 100, 50), self.botao_menu_rect, 3)
            
        tela.blit(self.texto_jogar, self.jogar_rect)
        tela.blit(self.texto_menu, self.menu_rect)

class CenaVitoria:
    def __init__(self):
        self.pergaminho = pygame.image.load("../imagens_principais/pergaminho.png").convert_alpha()
        self.pergaminho = pygame.transform.scale(self.pergaminho, (400, 500))
        self.perg_rect = self.pergaminho.get_rect(center=(LARGURA // 2, ALTURA // 2))
        
        self.fonte_titulo = pygame.font.Font("../fontes/8bitoperator.ttf", 45)
        self.texto_titulo = self.fonte_titulo.render("VITORIA", True, VERDE)
        self.titulo_rect = self.texto_titulo.get_rect(center=(LARGURA // 2, ALTURA // 2 - 80))

        self.fonte_botao = pygame.font.Font("../fontes/8bitoperator.ttf", 20)
        
        self.texto_jogar = self.fonte_botao.render("Jogar", True, PRETO) 
        self.jogar_rect = self.texto_jogar.get_rect(center=(LARGURA // 2, ALTURA // 2 + 20))
        self.botao_jogar_rect = self.jogar_rect.inflate(40, 20) 
        
        self.texto_menu = self.fonte_botao.render("Menu", True, PRETO) 
        self.menu_rect = self.texto_menu.get_rect(center=(LARGURA // 2, ALTURA // 2 + 80))
        self.botao_menu_rect = self.menu_rect.inflate(40, 20) 
        
        self.opcao_selecionada = 0
        self.debounce_up = False
        self.debounce_down = False
        self.debounce_enter = True
        
        som_vitoria = pygame.mixer.Sound("../sons/vitoria.wav")
        som_vitoria.set_volume(0.4)
        som_vitoria.play()

    def atualiza(self):
        teclas = pygame.key.get_pressed()
        
        if teclas[pygame.K_UP] or teclas[pygame.K_w]:
            if not self.debounce_up:
                self.opcao_selecionada -= 1
                if self.opcao_selecionada < 0:
                    self.opcao_selecionada = 2
                self.debounce_up = True
        else:
            self.debounce_up = False
            
        if teclas[pygame.K_DOWN] or teclas[pygame.K_s]:
            if not self.debounce_down:
                self.opcao_selecionada += 1
                if self.opcao_selecionada > 2:
                    self.opcao_selecionada = 0
                self.debounce_down = True
        else:
            self.debounce_down = False
            
        if teclas[pygame.K_SPACE] or teclas[pygame.K_RETURN]:
            if not self.debounce_enter:
                som_click = pygame.mixer.Sound("../sons/click.wav")
                som_click.set_volume(0.2)
                som_click.play()
                if self.opcao_selecionada == 0:
                    return "CENA_JOGO"
                elif self.opcao_selecionada == 1:
                    return "CENA_MENU"
                self.debounce_enter = True
        else:
            self.debounce_enter = False
            
        return None

    def desenha(self, tela):
        tela.blit(self.pergaminho, self.perg_rect)
        tela.blit(self.texto_titulo, self.titulo_rect)
        
        if self.opcao_selecionada == 0:
            pygame.draw.rect(tela, (139, 100, 50), self.botao_jogar_rect, 3)
        elif self.opcao_selecionada == 1:
            pygame.draw.rect(tela, (139, 100, 50), self.botao_menu_rect, 3)
            
        tela.blit(self.texto_jogar, self.jogar_rect)
        tela.blit(self.texto_menu, self.menu_rect)
