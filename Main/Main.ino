#include <IRremote.hpp>

// Pino do receptor IR
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

const int TRIG_PIN = 12;
const int ECHO_PIN = 13;

unsigned long tempoUltimoSinal = 0;
const unsigned long TEMPO_LIMITE = 250;

//tempo de giro
const int TEMPO_GIRO_90  = 490;
const int TEMPO_GIRO_180 = 1040;
const int TEMPO_GIRO_360 = 2080;

// ==================================================

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(pinoReceptorIR, false);
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
  parado();
  Serial.println("Robo pronto!");
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
        case 0xE619FF00:
          Serial.println("Comando: Botao 8");
          rodarEsquerda();
          break;
        case 0xE51AFF00:
          Serial.println("Comando: Botao 9");
          rodarDireita();
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
// SENSOR ULTRASSÔNICO E ROTINAS DE MOVIMENTO
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

