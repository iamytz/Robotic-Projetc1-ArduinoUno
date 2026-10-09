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


  void linhaReta() {
    if (distanciaSegura()) {
      frente();
      delay(1000);
  
      direita();
      delay(TEMPO_GIRO_180);
  
      delay(500);
  
      frente();
      delay(1000);
    
      esquerda();
      delay(TEMPO_GIRO_180);
  
      parado();
    } else {
      parado();
    }
  }
  
  void rodarLosangulo() {
    if (distanciaSegura()) {
      frente(); delay(500);
      parado(); delay(500);
  
      direita(); delay(TEMPO_GIRO_90);
      parado(); delay(500);
  
      frente(); delay(500);
      parado(); delay(500);
  
      direita(); delay(TEMPO_GIRO_90);
      parado(); delay(500);
  
      frente(); delay(500);
      parado(); delay(500);
      direita(); delay(TEMPO_GIRO_90);
      parado(); delay(500);
  
      frente(); delay(500);
      parado(); delay(500);
  
      direita(); delay(TEMPO_GIRO_90);
      frente(); delay(500);
      parado(); delay(500);
      direita(); delay(TEMPO_GIRO_90);
  
      parado();
    } else {
      parado();
    }
  }
  
  void andarInfinito() {
    if (distanciaSegura()) {
      frente();
    } else {
      parado();
  
      direita();
      delay(TEMPO_GIRO_90);
      parado();
      delay(400);
  
      if (!distanciaSegura()) {
        esquerda();
        delay(TEMPO_GIRO_180);
        parado();
        delay(400);
      }
    }
  }
  
  void zigueZague() {
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
    } else {
      parado();
    }
  }
  
  void danca() {
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
  
    } else {
      parado();
    }
  }