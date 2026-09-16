#include <IRremote.hpp>

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
const int LED_ROTA3 = A0;
const int LED_ROTA2 = A1;
const int LED_ROTA1 = A2;
const int BUZZER_PIN = A3;

//tempo de giro
const int TEMPO_GIRO_45  = 245;
const int TEMPO_GIRO_90  = 490;
const int TEMPO_GIRO_180 = 1040;
const int TEMPO_GIRO_360 = 2080;

// ================= NOTAS E TEMPOS =================
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
// ==================================================

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
  
  pinMode(BUZZER_PIN, OUTPUT); 

  pinMode(LED_ROTA1, OUTPUT);
  pinMode(LED_ROTA2, OUTPUT);
  pinMode(LED_ROTA3, OUTPUT);

  Serial.println("Robo pronto!");
  parado();
  bipar(); // Bip de inicialização usando sua função segura
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
          {
            const int fraseTeste[][2] = {
              {C4, CURTA},
              {D4, CURTA},
              {E4, LONGA}
            };
            tocarFrase(fraseTeste, 3);
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

// ==============================================================
// FUNÇÕES DE SOM (AGORA 100% SEGURAS PARA O IR)
// ==============================================================

void bipar() {
  for (int i = 0; i < 100; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(1000); 
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(1000); 
    
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(1000); 
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(1000); 
  }
}

// Nova versão de tocarNota que funciona como o bipar() (sem usar a função tone())
void tocarNota(int frequencia, int duracao) {
  long periodo = 1000000L / frequencia;
  long metadePeriodo = periodo / 2;
  long ciclos = ((long)duracao * 1000L) / periodo;

  for (long i = 0; i < ciclos; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(metadePeriodo);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(metadePeriodo);
  }
}

void tocarFrase(const int notas[][2], int quantidade) {
  for (int i = 0; i < quantidade; i++) {
    tocarNota(notas[i][0], notas[i][1]);
    delay(10); // Pequena pausa entre as notas para não embolar o som
  }
}

void tocarFurElise() {
  const int frase1[][2] = {
    {E5, CURTA}, {DIS5, CURTA}, {E5, CURTA}, {DIS5, CURTA},
    {E5, CURTA}, {B4, CURTA}, {D5, CURTA}, {C5, CURTA}, {NOTA_A4, LONGA}
  };
  const int frase2[][2] = {
    {C4, CURTA}, {E4, CURTA}, {NOTA_A4, CURTA}, {B4, LONGA}
  };
  const int frase3[][2] = {
    {E4, CURTA}, {G4, CURTA}, {B4, CURTA}, {C5, LONGA}
  };
  const int frase4[][2] = {
    {E4, MEDIA}, {E5, CURTA}, {DIS5, CURTA}, {E5, CURTA},
    {DIS5, CURTA}, {E5, CURTA}, {B4, CURTA}, {D5, CURTA},
    {C5, CURTA}, {NOTA_A4, LONGA}
  };
  const int frase5[][2] = {
    {C4, CURTA}, {E4, CURTA}, {NOTA_A4, CURTA}, {B4, LONGA}
  };
  const int frase6[][2] = {
    {E4, CURTA}, {C5, CURTA}, {B4, CURTA}, {NOTA_A4, LONGA}
  };

  tocarFrase(frase1, 9); delay(PAUSA_FRASE);
  tocarFrase(frase2, 4); delay(PAUSA_FRASE);
  tocarFrase(frase3, 4); delay(PAUSA_FRASE);
  tocarFrase(frase4, 10); delay(150);
  tocarFrase(frase5, 4); delay(PAUSA_FRASE);
  tocarFrase(frase6, 4);
}

// ==============================================================
// FUNÇÕES DE MOVIMENTO
// ==============================================================

void rodarDireita() {
  direita();
  delay(TEMPO_GIRO_360);
}

void rodarEsquerda() {
  esquerda();
  delay(TEMPO_GIRO_360);
}

void frente() {
  digitalWrite(IN1_TRASEIRO, HIGH); digitalWrite(IN2_TRASEIRO, LOW);
  digitalWrite(IN3_TRASEIRO, LOW);  digitalWrite(IN4_TRASEIRO, HIGH);
  digitalWrite(IN1_DIANTEIRO, LOW); digitalWrite(IN2_DIANTEIRO, HIGH);
  digitalWrite(IN3_DIANTEIRO, HIGH); digitalWrite(IN4_DIANTEIRO, LOW);
}

void tras() {
  digitalWrite(IN1_TRASEIRO, LOW);  digitalWrite(IN2_TRASEIRO, HIGH);
  digitalWrite(IN3_TRASEIRO, HIGH); digitalWrite(IN4_TRASEIRO, LOW);
  digitalWrite(IN1_DIANTEIRO, HIGH); digitalWrite(IN2_DIANTEIRO, LOW);
  digitalWrite(IN3_DIANTEIRO, LOW); digitalWrite(IN4_DIANTEIRO, HIGH);
}

void parado() {
  digitalWrite(IN1_TRASEIRO, LOW); digitalWrite(IN2_TRASEIRO, LOW);
  digitalWrite(IN3_TRASEIRO, LOW); digitalWrite(IN4_TRASEIRO, LOW);
  digitalWrite(IN1_DIANTEIRO, LOW); digitalWrite(IN2_DIANTEIRO, LOW);
  digitalWrite(IN3_DIANTEIRO, LOW); digitalWrite(IN4_DIANTEIRO, LOW);
}

void direita() {
  digitalWrite(IN1_TRASEIRO, HIGH); digitalWrite(IN2_TRASEIRO, LOW);
  digitalWrite(IN3_TRASEIRO, HIGH); digitalWrite(IN4_TRASEIRO, LOW);
  digitalWrite(IN1_DIANTEIRO, HIGH); digitalWrite(IN2_DIANTEIRO, LOW);
  digitalWrite(IN3_DIANTEIRO, HIGH); digitalWrite(IN4_DIANTEIRO, LOW);
}

void esquerda() {
  digitalWrite(IN1_TRASEIRO, LOW); digitalWrite(IN2_TRASEIRO, HIGH);
  digitalWrite(IN3_TRASEIRO, LOW); digitalWrite(IN4_TRASEIRO, HIGH);
  digitalWrite(IN1_DIANTEIRO, LOW); digitalWrite(IN2_DIANTEIRO, HIGH);
  digitalWrite(IN3_DIANTEIRO, LOW); digitalWrite(IN4_DIANTEIRO, HIGH);
}

// ==============================================================
// FUNÇÕES ESPECIAIS / ROTINAS (Com os bips seguros)
// ==============================================================

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
  digitalWrite(LED_ROTA1, l1);
  digitalWrite(LED_ROTA2, l2);
  digitalWrite(LED_ROTA3, l3);
}

void linhaReta() {
  atualizarLeds(HIGH, LOW, LOW);

  if (distanciaSegura()) {
    frente();
    delay(1000);

    bipar(); // Substituído tone() por bipar()
    direita();
    delay(TEMPO_GIRO_180);

    delay(500);

    frente();
    delay(1000);
  
    bipar(); // Substituído tone() por bipar()
    esquerda();
    delay(TEMPO_GIRO_180);

    parado();
    roboLigado = false; 
  } else {
    parado();
    bipar();
  }
}

void rodarLosangulo() {
  atualizarLeds(LOW, HIGH, LOW);

  if (distanciaSegura()) {
    frente(); delay(500);
    parado(); delay(500);

    bipar(); // Substituído tone() por bipar()
    direita(); delay(TEMPO_GIRO_90);
    parado(); delay(500);

    frente(); delay(500);
    parado(); delay(500);

    bipar();
    direita(); delay(TEMPO_GIRO_90);
    parado(); delay(500);

    frente(); delay(500);
    parado(); delay(500);

    bipar();
    direita(); delay(TEMPO_GIRO_90);
    parado(); delay(500);

    frente(); delay(500);
    parado(); delay(500);

    bipar();
    direita(); delay(TEMPO_GIRO_90);
    frente(); delay(500);
    parado(); delay(500);
    direita(); delay(TEMPO_GIRO_90);




    
    parado();
    roboLigado = false;
  } else {
    parado();
    bipar();
  }
}

void andarInfinito() {
  atualizarLeds(LOW, LOW, HIGH);

  if (distanciaSegura()) {
    frente();
  } else {
    parado();
    bipar(); // Aviso de obstáculo

    bipar();
    direita();
    delay(TEMPO_GIRO_90);
    parado();
    delay(400);

    if (!distanciaSegura()) {
      bipar();
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
    frente(); delay(1000);
    direita(); delay(TEMPO_GIRO_90);
    frente(); delay(1000);
    esquerda(); delay(TEMPO_GIRO_90);
    frente(); delay(1000);
    direita(); delay(TEMPO_GIRO_90);
    frente(); delay(1000);
    esquerda(); delay(TEMPO_GIRO_90);
    parado();
    roboLigado = false;
  } else {
    parado();
    bipar();
  }
}

void danca() {
  atualizarLeds(HIGH, HIGH, HIGH);

  if (distanciaSegura()) {
    frente(); delay(500);
    parado(); delay(300);
    tras(); delay(500);
    parado(); delay(300);
    direita(); delay(500);
    esquerda(); delay(500);
    direita(); delay(500);
    esquerda(); delay(500);
    frente(); delay(500);
    parado();

    tocarFurElise(); // Agora vai tocar sem travar!

    roboLigado = false;
  } else {
    parado();
    bipar();
  }
}