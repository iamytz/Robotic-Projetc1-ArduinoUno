# 🤖 Robô Controlado por Infravermelho (Arduino Uno)

Este repositório contém o código-fonte (em C++) para um robô móvel baseado em Arduino Uno. O projeto é controlado remotamente via Infravermelho (IR) e conta com tração nas quatro rodas, desvio de obstáculos via sensor ultrassônico, indicadores visuais (LEDs) e feedback sonoro (Buzzer).

O código decodifica sinais hexadecimais recebidos de um controle remoto padrão e os traduz em movimentos precisos, rotinas autônomas e até execução de músicas.

---

## 🛠️ Hardware e Pinos

O mapeamento dos componentes no Arduino Uno foi configurado da seguinte forma:

* **Receptor IR:** `A4`
* **Sensor Ultrassônico (HC-SR04):** `TRIG` no `12` | `ECHO` no `13`
* **Motor Driver (Dianteiros):** Pinos `8`, `9`, `10`, `11`
* **Motor Driver (Traseiros):** Pinos `4`, `5`, `6`, `7`
* **Buzzer:** `8` e `A3`
* **LEDs Indicadores:** `A0`, `A1`, `A2`
* **Botão de Emergência:** `3`

---

## 🎮 Comandos do Controle Remoto

O robô possui três camadas principais de funcionalidades, todas acessadas pelo controle IR.

### 1. Movimentação Livre (Setas)
Responde em tempo real aos comandos direcionais:
- **Cima/Baixo:** Motores impulsionam o robô para frente ou para trás.
- **Esquerda/Direita:** Rotação no próprio eixo (tank turn).
> 🛡️ **Segurança (Timeout):** O sistema exige comunicação constante. Se o controle parar de enviar o sinal (após 250ms), os motores param automaticamente.

### 2. Rotinas Autônomas (Teclado Numérico)
Sequências de movimentos pré-programadas, acompanhadas de indicadores visuais nos LEDs. 
*(Nota: Todas essas rotinas dependem do sistema anti-colisão detalhado abaixo).*

- `0` **Losango:** Desenha um trajeto em formato de losango no chão, girando 90º a cada passo.
- `1` **Linha Reta:** Avança, faz um giro de 180º e retorna ao ponto de origem.
- `2` **Modo Exploração (Infinito):** Anda para a frente continuamente. Se encontrar um obstáculo, tenta desviar girando à direita.
- `3` **Zigue-zague:** Trajetória em "S", alternando giros curtos para os lados.
- `4` **Dança:** Uma coreografia de passos rápidos e giros, finalizada com uma música.
- `8` e `9` **Pião:** Gira rapidamente no próprio eixo por ~2 segundos.

### 3. Sons e Melodias (Buzzer)
O código inclui frequências mapeadas para notas musicais (Dó4 a Si5).
- `5` **Nota:** Toca a nota Dó.
- `6` **Frase:** Toca uma pequena escala musical.
- `7` **Música Completa:** Executa o tema clássico *"Für Elise"* (Beethoven).

---

## 🛑 Sistema Anti-Colisão
Antes de executar qualquer movimento das rotinas autônomas (botões 0 a 4), o robô executa a função `distanciaSegura()`.

Se o sensor ultrassônico detectar um objeto a **menos de 30 cm** de distância, o robô:
1. Cancela a movimentação.
2. Para os motores imediatamente.
3. Emite um alerta sonoro grave (300Hz) no buzzer avisando que a rota está bloqueada.

---

## 🚀 Como usar

1. Conecte todos os jumpers e componentes conforme o mapeamento de pinos listado acima.
2. Abra este código na **Arduino IDE**.
3. Certifique-se de ter a biblioteca `IRremote` instalada *(Sketch > Include Library > Manage Libraries > busque por "IRremote")*.
4. Compile e faça o upload para o seu Arduino Uno.
5. Ao ligar, o robô emitirá um "bip" curto indicando que o `setup()` foi concluído e está aguardando os sinais do controle!