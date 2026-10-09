#include <IRremote.hpp>

// pino receptor ir
const int pinoReceptorIR = A5;

// CONFIG LEDS
const int LED_VERMELHO = A4;
const int LED_AMARELO = A3;
const int LED_VERDE = A2;

// BIPPER
const int BEPPER = A1;
const int TRIG_PIN = 4;
const int ECHO_PIN = 5;

unsigned long tempoUltimoSinal = 0;
const unsigned long TEMPO_LIMITE = 250;
bool led_ligado = false;
bool verificar_sensor = false; // Mudado para iniciar DESLIGADO até receber o comando

// ==================================================
void setup() {
  Serial.begin(9600);
  IrReceiver.begin(pinoReceptorIR, false);
  Serial.println("Aguardando comandos...");

  // Configuração dos Leds
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  // Configuração do Bipper
  pinMode(BEPPER, OUTPUT);

  //Configuração do Sensor Ultra Sonico
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Robo pronto!");
}

void loop() {
  // 1. ROTINA DO SENSOR (Executa continuamente se o modo estiver ativo)
  if (verificar_sensor) {
    int distanciaCm = calcularDistancia();
    
    // Evita leituras falsas de 0cm (fora de alcance)
    if (distanciaCm > 0) { 
      if (distanciaCm <= 10) {
        digitalWrite(LED_VERDE, LOW);
        beepar(80);
        piscarLed(LED_VERMELHO, 50);
      } 
      else if (distanciaCm <= 20) {
        digitalWrite(LED_VERDE, LOW);
        beepar(120);
        piscarLed(LED_AMARELO, 100);
      } 
      else {
        digitalWrite(LED_VERDE, HIGH);
        digitalWrite(LED_AMARELO, LOW);
        digitalWrite(LED_VERMELHO, LOW);
      }
    }
  }

  // 2. VERIFICAÇÃO DO SINAL DO CONTROLE IR
  if (IrReceiver.decode()) {
    unsigned long codigoRecebido = IrReceiver.decodedIRData.decodedRawData;
    tempoUltimoSinal = millis();

    if (codigoRecebido != 0) {
      switch (codigoRecebido) {
        // ================= SETAS DIRECIONAIS =================
        case 0xFF00FF00: 
          // Alterna o estado do sensor (Liga / Desliga)
          verificar_sensor = !verificar_sensor; 
          if (verificar_sensor) {
            Serial.println("Comando: Monitoramento Sensor LIGADO");
          } else {
            Serial.println("Comando: Monitoramento Sensor DESLIGADO");
            desligarLed();
          }
          break;

        case 0xFD02FF00:
          if (led_ligado) {
            desligarLed();
            led_ligado = false;
            Serial.println("Comando: Led Desligado!");
          } else {
            ligarLed();
            led_ligado = true;
            Serial.println("Comando: Led Ligado!");
          }
          break;

        case 0xFE01FF00: Serial.println("Comando: CIMA (Frente)"); break;
        case 0xF609FF00: Serial.println("Comando: BAIXO (Trás)"); break;
        case 0xFB04FF00: Serial.println("Comancy: ESQUERDA"); break;
        case 0xF906FF00: Serial.println("Comando: DIREITA"); break;

        // ================= TECLADO NUMÉRICO =================
        case 0xF20DFF00:
          Serial.println("Comando: Botao 0");
          beepar(100);
          break;

        case 0xEF10FF00:
          Serial.println("Comando: Botao 1");
          desligarLed();
          break;

        case 0xEE11FF00: Serial.println("Comando: Botao 2"); break;
        case 0xED12FF00: Serial.println("Comando: Botao 3"); break;
        case 0xEB14FF00: Serial.println("Comando: Botao 4"); break;
        case 0xE619FF00: Serial.println("Comando: Botao 8"); break;
        case 0xE51AFF00: Serial.println("Comando: Botao 9"); break;

        default:
          Serial.print("Botao não mapeado: ");
          Serial.println(codigoRecebido, HEX);
          break;
      }
    }
    IrReceiver.resume(); // Prepara para receber o próximo valor
  }
}

// ==============================================================
// SENSOR ULTRASSÔNICO E ROTINAS DE MOVIMENTO
// ==============================================================
int calcularDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(4);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); // O padrão para o HC-SR04 são 10 microssegundos
  digitalWrite(TRIG_PIN, LOW);
  
  long duracao = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout de 30ms
  
  if (duracao == 0) return 0;
  
  int distanciaCm = (duracao * 0.034) / 2;
  return distanciaCm;
}

void piscarLed(int LED, int time) {
  digitalWrite(LED, HIGH);
  delay(time);
  digitalWrite(LED, LOW);
}

void ligarLed() {
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  digitalWrite(LED_VERDE, HIGH);
}

void desligarLed() {
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERDE, LOW);
}

void beepar(int time) {
  digitalWrite(BEPPER, HIGH);
  delay(time);
  digitalWrite(BEPPER, LOW);
}
