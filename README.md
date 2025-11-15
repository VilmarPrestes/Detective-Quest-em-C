# 🕵️‍♂️ Detective Quest em C — Árvore Binária, BST e Tabela Hash

Este projeto foi desenvolvido como parte da disciplina de **Estruturas de Dados** em **C**, simulando a lógica de investigação do jogo **Detective Quest** (inspirado em Detetive/Clue).

O objetivo é implementar, de forma integrada:

- Uma **árvore binária** para representar o mapa da mansão (cômodos).
- Uma **árvore de busca binária (BST)** para armazenar as **pistas coletadas**.
- Uma **tabela hash** para associar cada **pista** a um **suspeito**.
- Um sistema de **julgamento final**, que avalia se há pistas suficientes para acusar alguém.

---

## 🎯 Objetivos Atendidos

1. **Implementar uma árvore binária em C**  
   - Representa a mansão: cada nó é um cômodo.
   - Permite navegar interativamente entre salas (esquerda/direita).
   - Estrutura montada de forma fixa e automática no `main()`.

2. **Manipular uma árvore de busca binária (BST) para pistas**  
   - Cada pista coletada é inserida em uma BST.
   - Ao final, as pistas são exibidas em **ordem alfabética** (in-order traversal).

3. **Integrar árvore binária, BST e tabela hash**  
   - Tabela hash associa **pista → suspeito**.
   - No julgamento final, o jogador acusa um suspeito.
   - O sistema verifica se **pelo menos 2 pistas** apontam para essa pessoa.

---

## 🧱 Estruturas de Dados

### Struct `Sala` — Árvore Binária (Mapa da Mansão)

```c
typedef struct Sala {
    char nome[60];
    char pista[120];
    int pistaColetada;
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;
