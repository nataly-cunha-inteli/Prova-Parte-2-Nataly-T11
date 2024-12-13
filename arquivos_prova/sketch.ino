// (OK) 1) Todo o código deve estar padronizado para inglês ou português, incluindo comentários (1,0 ponto)
// (OK) 2) Todas as entradas e saídas devem estar declaradas corretamente na inicialização (1,0 ponto);
// (OK) 3) Todos os leds devem estar apagados na inicialização (até 1,0 ponto);
// (OK) 4) Quando estiver escuro (segundo a leitura analógica do sensor LDR), o protótipo deve ativar o modo noturno e piscar o led amarelo a cada segundo (até 1,0 ponto);
// (OK) 5) Quando estiver claro (segundo a leitura analógica do sensor LDR), o protótipo deve ativar o modo convencional e fazer a temporização alternando entre verde (3 segundos), amarelo (2 segundos) e vermelho (5 segundos) (até 2,0 pontos);

// Importação das bibliotecas necessárias para conexão Wi-fi
#include <WiFi.h>
#include <HTTPClient.h>

// Com o #define, declaramos os pinos como variáveis
#define led_verde 2 // Pino utilizado para controle do led verde
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led amarelo
#define pinBotao 18 // Pino utilizado para o controle do botão
#define pinLDR 13 // Pino utilizado para a verificação do sensor LDR

int threshold=600;

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_amarelo,OUTPUT);

  // Inicialização das entradas
  pinMode(pinBotao, INPUT_PULLUP); // Configura o pinBotao como INPUT_PULLUP para evitar resistores externos

  // Nas portas digitais, escreve o estado de cada LED como low
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_amarelo, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  // Verifica estado do botão
  int estadoBotao = digitalRead(pinBotao);
  if (estadoBotao == LOW) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  // O seguinte bloco de código contendo as funcionalidades de requisição e conexão com a internet não teve algumas variáveis alteradas, na intenção de não se quebrar o código padrão.
  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("Código de resposta HTTP: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Erro de código: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi desconectado");
  }
}

// Função que será executada constantemente enquanto o código estiver em funcionamento
void loop() {

  // Guarda na variável estado LDR a leitura analógica do pin do sensor de luminosidade (LDR)
  int estadoLDR=analogRead(pinLDR);

  // Se o estado do LDR for menor ou igual ao limiar (thresold), significa que está escuro
  if(estadoLDR>=threshold){
    Serial.print("Tá escuro! Ligue o LED"); // É printado um aviso
    Serial.println(estadoLDR); // É printado o valor do LDR, da luminosidade
    
    // Delay para LEDS inicializarem desligados
    delay(2500);
    // Liga o LED amarelo
    digitalWrite(led_amarelo, HIGH); // Liga o LED
    delay(1000); // Espera 1 seg
    digitalWrite(led_amarelo, LOW); // Desliga o LED
    delay(1000); // Espera 1 seg
  }else if (estadoLDR<threshold){
    Serial.print("Já está claro! Desligue esse LED"); // Se não tiver escuro, é printado um aviso de que está claro
    Serial.println(estadoLDR); // É printado o valor do LDR, da luminosidade

 // Delay para LEDS inicializarem desligados
   delay(2500);

    // Liga o LED verde
    digitalWrite(led_verde, HIGH); // Liga o LED
    delay(3000); // Espera 1 seg
    digitalWrite(led_verde, LOW); // Desliga o LED

    // Liga o LED amarelo
    digitalWrite(led_amarelo, HIGH); // Liga o LED
    delay(2000); // Espera 1 seg
    digitalWrite(led_amarelo, LOW); // Desliga o LED

     // Liga o LED amarelo
    digitalWrite(led_vermelho, HIGH); // Liga o LED
    delay(5000); // Espera 1 seg
    digitalWrite(led_vermelho, LOW); // Desliga o LED
  }else {
    Serial.println("inválido");
  }
}