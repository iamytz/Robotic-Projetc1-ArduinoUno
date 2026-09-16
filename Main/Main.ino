#include <IRremote.hpp>
#define BUZZER 8

// Define o pino onde o receptor IR está conectado
const int pinoReceptorIR = A4;

// Driver 1 - Traseiro
const int IN1_TRASEIRO = 4;
const int IN2_TRASEIRO = 5;
const int IN3_TRASEIRO = 6;
const int IN4_TRASEIRO = 7;

// Driver 2 - Dianteiro
const int IN1_DIANTEIRO = 8;
const int IN2_DIANTEIRO = 9;
const int IN3_DIANTEIRO = 10;
const int IN4_DIANTEIRO = 11;

//sensor ultra
const int TRIG_PIN = 12;
const int ECHO_PIN = 13;

//botão emergencia
const int BOTAO_PIN = 3;  

bool roboLigado = false;

bool estadoBotaoAnterior = HIGH;      
unsigned long ultimoDebounceTime = 0; 
const unsigned long delayDebounce = 200; 


unsigned long tempoUltimoSinal = 0;
const unsigned long TEMPO_LIMITE = 250; 

// LEDs e Buzzer
const int LED3 = A0;
const int LED2 = A1;
const int LED1 = A2;
const int BUZZER_PIN = A3;

//tempo de giro

const int TEMPO_GIRO_45  = 245;
const int TEMPO_GIRO_90  = 490;
const int TEMPO_GIRO_180 = 1040;
const int TEMPO_GIRO_360 = 2080;

// notas para a música
const int C4 = 262;
const int D4 = 294;
const int DIS4 = 311;
const int E4 = 330;
const int F4 = 349;
const int G4 = 392;
const int NOTA_A4 = 440;
const int B4 = 494;

const int C5 = 523;
const int D5 = 587;
const int DIS5 = 622;
const int E5 = 659;
const int F5 = 698;
const int G5 = 784;
const int NOTA_A5 = 880;
const int B5 = 988;

const int CURTA = 180;
const int MEDIA = 280;
const int LONGA = 500;

const int PAUSA_FRASE = 80;


void setup() {
  Serial.begin(9600);
  IrReceiver.begin(pinoReceptorIR, ENABLE_LED_FEEDBACK);
  Serial.println("Aguardando comandos...");

  // Configuração dos Drivers
  pinMode(IN1_TRASEIRO, OUTPUT);
  pinMode(IN2_TRASEIRO, OUTPUT);
  pinMode(IN3_TRASEIRO, OUTPUT);
  pinMode(IN4_TRASEIRO, OUTPUT);

  pinMode(IN1_DIANTEIRO, OUTPUT);
  pinMode(IN2_DIANTEIRO, OUTPUT);
  pinMode(IN3_DIANTEIRO, OUTPUT);
  pinMode(IN4_DIANTEIRO, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BOTAO_PIN, INPUT_PULLUP);
  
  // ---> NOVO: Declarar o pino do Buzzer como saída <---
  pinMode(BUZZER_PIN, OUTPUT); 

  Serial.println("Robo pronto!");
  parado();
  tone(BUZZER_PIN, 400, 100); // Bip inicial indicando que ligou
}

void loop() {
  // 1. Verifica se recebeu algum sinal do controle
  if (IrReceiver.decode()) {
    unsigned long codigoRecebido = IrReceiver.decodedIRData.decodedRawData;


    tempoUltimoSinal = millis();

    if (codigoRecebido != 0) {
      
      switch (codigoRecebido) {
        
        // ================= SETAS DIRECIONAIS =================
        case 0xFE01FF00:
          Serial.println("Comando: CIMA (Frente)");
          frente();
          break;
          
        case 0xF609FF00:
          Serial.println("Comando: BAIXO (Trás)");
          tras();
          break;
          
        case 0xFB04FF00:
          Serial.println("Comando: ESQUERDA");
          esquerda();
          break;
          
        case 0xF906FF00:
          Serial.println("Comando: DIREITA");
          direita();
          break;

        // ================= TECLADO NUMÉRICO =================
        case 0xF20DFF00:
          Serial.println("Comando: Botao 0");
          rodarLosangulo();
          break;
        case 0xEF10FF00:
          Serial.println("Comando: Botao 1");
          linhaReta();
          break;
        case 0xEE11FF00:
          Serial.println("Comando: Botao 2");
          andarInfinito();
          break;
        case 0xED12FF00:
          Serial.println("Comando: Botao 3");
          zigueZague();
          break;
        case 0xEB14FF00:
          Serial.println("Comando: Botao 4");
          danca();
          break;
        case 0xEA15FF00:
          Serial.println("Comando: Botao 5");
          tocarNota(C4, LONGA);
          break;
        case 0xE916FF00:
          Serial.println("Comando: Botao 6");
          // Para tocar uma frase, você precisa de uma matriz de notas.
          // Como você não definiu uma música específica para o botão 6, 
          // aqui está um exemplo criando uma pequena frase na hora:
          {
            const int fraseTeste[][2] = {
              {C4, CURTA},
              {D4, CURTA},
              {E4, LONGA}
            };
            tocarFrase(fraseTeste, 3); // 3 é a quantidade de notas na frase
          }
          break;
        case 0xE718FF00:
          Serial.println("Comando: Botao 7");
          tocarFurElise();          
          break;
        case 0xE619FF00:
          Serial.println("Comando: Botao 8");
          rodarEsquerda();
          break;
        case 0xE51AFF00:
          Serial.println("Comando: Botao 9");
          rodarDireita();
          break;

        case 0xFA05FF00:
          Serial.println("Comando: Volume ");
          bipar();
          break;

        default:
          Serial.print("Botao não mapeado: ");
          Serial.println(codigoRecebido, HEX);
          break;
      }
    }
    
    IrReceiver.resume(); 
  }


  if (millis() - tempoUltimoSinal > TEMPO_LIMITE) {
    parado();
  }
}

void bipar() {
  // Cria a frequência de 1000Hz manualmente repetindo o pulso 100 vezes (aprox. 100ms)
  for (int i = 0; i < 100; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(1000); // Metade da onda (500us)
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(1000); // Outra metade (500us)
    
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(1000); // Metade da onda (500us)
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(1000); // Outra metade (500us)

  }
}

void rodarDireita() {
  direita();
  delay(2080);
}

void rodarEsquerda() {
  esquerda();
  delay(2080);
}

void rodarLosanguloGa() {
  frente();
  delay(500);

  parado();
  delay(500);

  direita();
  delay(TEMPO_GIRO_90);

  parado();
  delay(500);

  frente();
  delay(500);

  parado();
  delay(500);

  direita();
  delay(TEMPO_GIRO_90);

  parado();
  delay(500);

  frente();
  delay(500);

  parado();
  delay(500);

  direita();
  delay(TEMPO_GIRO_90);

  parado();
  delay(500);

  frente();
  delay(500);

  parado();
  delay(500);

  direita();
  delay(TEMPO_GIRO_90);




}

// --- Funções de Movimento dos Motores ---
void frente() {
  digitalWrite(IN1_TRASEIRO, HIGH);
  digitalWrite(IN2_TRASEIRO, LOW);
  digitalWrite(IN3_TRASEIRO, LOW);
  digitalWrite(IN4_TRASEIRO, HIGH);

  digitalWrite(IN1_DIANTEIRO, LOW);
  digitalWrite(IN2_DIANTEIRO, HIGH);
  digitalWrite(IN3_DIANTEIRO, HIGH);
  digitalWrite(IN4_DIANTEIRO, LOW);
}

void tras() {
  digitalWrite(IN1_TRASEIRO, LOW);
  digitalWrite(IN2_TRASEIRO, HIGH);
  digitalWrite(IN3_TRASEIRO, HIGH);
  digitalWrite(IN4_TRASEIRO, LOW);

  digitalWrite(IN1_DIANTEIRO, HIGH);
  digitalWrite(IN2_DIANTEIRO, LOW);
  digitalWrite(IN3_DIANTEIRO, LOW);
  digitalWrite(IN4_DIANTEIRO, HIGH);
}

void parado() {
  digitalWrite(IN1_TRASEIRO, LOW);
  digitalWrite(IN2_TRASEIRO, LOW);
  digitalWrite(IN3_TRASEIRO, LOW);
  digitalWrite(IN4_TRASEIRO, LOW);

  digitalWrite(IN1_DIANTEIRO, LOW);
  digitalWrite(IN2_DIANTEIRO, LOW);
  digitalWrite(IN3_DIANTEIRO, LOW);
  digitalWrite(IN4_DIANTEIRO, LOW);
}

void direita() {
  digitalWrite(IN1_TRASEIRO, HIGH);
  digitalWrite(IN2_TRASEIRO, LOW);
  digitalWrite(IN3_TRASEIRO, HIGH);
  digitalWrite(IN4_TRASEIRO, LOW);

  digitalWrite(IN1_DIANTEIRO, HIGH);
  digitalWrite(IN2_DIANTEIRO, LOW);
  digitalWrite(IN3_DIANTEIRO, HIGH);
  digitalWrite(IN4_DIANTEIRO, LOW);
}

void esquerda() {
  digitalWrite(IN1_TRASEIRO, LOW);
  digitalWrite(IN2_TRASEIRO, HIGH);
  digitalWrite(IN3_TRASEIRO, LOW);
  digitalWrite(IN4_TRASEIRO, HIGH);

  digitalWrite(IN1_DIANTEIRO, LOW);
  digitalWrite(IN2_DIANTEIRO, HIGH);
  digitalWrite(IN3_DIANTEIRO, LOW);
  digitalWrite(IN4_DIANTEIRO, HIGH);
}


//FUNÇÕES DO JEAN

void linhaReta() {
  atualizarLeds(HIGH, LOW, LOW);

  if (distanciaSegura()) {
    frente();
    delay(1000);

    tone(BUZZER_PIN, 350, TEMPO_GIRO_180);
    direita();
    delay(TEMPO_GIRO_180);

    frente();
    delay(1000);

    tone(BUZZER_PIN, 350, TEMPO_GIRO_180);
    esquerda();
    delay(TEMPO_GIRO_180);

    parado();
    roboLigado = false; // Para o robô ao terminar a rota
  } else {
    parado();
    tone(BUZZER_PIN, 300, 150);
  }
}

void rodarLosangulo() {
  atualizarLeds(LOW, HIGH, LOW);

  if (distanciaSegura()) {
    frente();
    delay(500);

    parado();
    delay(500);

    tone(BUZZER_PIN, 350, TEMPO_GIRO_90);
    direita();
    delay(TEMPO_GIRO_90);

    parado();
    delay(500);

    frente();
    delay(500);

    parado();
    delay(500);

    tone(BUZZER_PIN, 350, TEMPO_GIRO_90);
    direita();
    delay(TEMPO_GIRO_90);

    parado();
    delay(500);

    frente();
    delay(500);

    parado();
    delay(500);

    tone(BUZZER_PIN, 350, TEMPO_GIRO_90);
    direita();
    delay(TEMPO_GIRO_90);

    parado();
    delay(500);

    frente();
    delay(500);

    parado();
    delay(500);

    tone(BUZZER_PIN, 350, TEMPO_GIRO_90);
    direita();
    delay(TEMPO_GIRO_90);

    parado();
    roboLigado = false;
  } else {
    parado();
    tone(BUZZER_PIN, 300, 150);
  }
}

void andarInfinito() {
  atualizarLeds(LOW, LOW, HIGH);

  if (distanciaSegura()) {
    frente();
  } else {
    parado();
    tone(BUZZER_PIN, 200, 300);

    tone(BUZZER_PIN, 350, TEMPO_GIRO_90);
    direita();
    delay(TEMPO_GIRO_90);

    parado();
    delay(400);

    if (!distanciaSegura()) {
      tone(BUZZER_PIN, 350, TEMPO_GIRO_180);
      esquerda();
      delay(TEMPO_GIRO_180);

      parado();
      delay(400);
    }
  }
}

void zigueZague() {
  atualizarLeds(LOW, HIGH, HIGH);

  if (distanciaSegura()) {
    frente();
    delay(1000);

    direita();
    delay(TEMPO_GIRO_90);

    frente();
    delay(1000);

    esquerda();
    delay(TEMPO_GIRO_90);

    frente();
    delay(1000);

    direita();
    delay(TEMPO_GIRO_90);

    frente();
    delay(1000);

    esquerda();
    delay(TEMPO_GIRO_90);

    parado();
    roboLigado = false;
  } else {
    parado();
    tone(BUZZER_PIN, 300, 150);
  }
}

void danca() {
  atualizarLeds(HIGH, HIGH, HIGH);

  if (distanciaSegura()) {
    frente();
    delay(500);

    parado();
    delay(300);

    tras();
    delay(500);

    parado();
    delay(300);

    direita();
    delay(500);

    esquerda();
    delay(500);

    direita();
    delay(500);

    esquerda();
    delay(500);

    frente();
    delay(500);

    parado();

    tocarFurElise();

    roboLigado = false;
  } else {
    parado();
    tone(BUZZER_PIN, 300, 150);
  }
}

void tocarNota(int frequencia, int duracao) {
  tone(BUZZER, frequencia);
  delay(duracao);
  noTone(BUZZER);
}

void tocarFrase(const int notas[][2], int quantidade) {
  for (int i = 0; i < quantidade; i++) {
    tocarNota(notas[i][0], notas[i][1]);
  }
}

void tocarFurElise() {

  const int frase1[][2] = {
    {E5, CURTA},
    {DIS5, CURTA},
    {E5, CURTA},
    {DIS5, CURTA},
    {E5, CURTA},
    {B4, CURTA},
    {D5, CURTA},
    {C5, CURTA},
    {NOTA_A4, LONGA}
  };

  const int frase2[][2] = {
    {C4, CURTA},
    {E4, CURTA},
    {NOTA_A4, CURTA},
    {B4, LONGA}
  };

  const int frase3[][2] = {
    {E4, CURTA},
    {G4, CURTA},
    {B4, CURTA},
    {C5, LONGA}
  };

  const int frase4[][2] = {
    {E4, MEDIA},
    {E5, CURTA},
    {DIS5, CURTA},
    {E5, CURTA},
    {DIS5, CURTA},
    {E5, CURTA},
    {B4, CURTA},
    {D5, CURTA},
    {C5, CURTA},
    {NOTA_A4, LONGA}
  };

  const int frase5[][2] = {
    {C4, CURTA},
    {E4, CURTA},
    {NOTA_A4, CURTA},
    {B4, LONGA}
  };

  const int frase6[][2] = {
    {E4, CURTA},
    {C5, CURTA},
    {B4, CURTA},
    {NOTA_A4, LONGA}
  };

  tocarFrase(frase1, 9);
  delay(PAUSA_FRASE);

  tocarFrase(frase2, 4);
  delay(PAUSA_FRASE);

  tocarFrase(frase3, 4);
  delay(PAUSA_FRASE);

  tocarFrase(frase4, 10);
  delay(150);

  tocarFrase(frase5, 4);
  delay(PAUSA_FRASE);

  tocarFrase(frase6, 4);
}

// --- AUXILIARES ---

bool distanciaSegura() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duracao == 0) return true;

  return (duracao * 0.034 / 2) > 30;
}

void atualizarLeds(bool l1, bool l2, bool l3) {
  digitalWrite(LED1, l1);
  digitalWrite(LED2, l2);
  digitalWrite(LED3, l3);
}



