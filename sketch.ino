#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 2 // Pino utilizado para controle do led verde
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led azul

const int buttonPin = 5;  // pino utilizado para o botão
int buttonState = 0;  // vvariável de status do botão

const int ldrPin = 4;  // pino do sensor ldr
int threshold = 600;

const char* ssid = "Wokwi-GUEST";
const char* password = "";
  
void setup_wifi() {
  delay(10);
  // iniciando a conexão com o wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // tentando conectar
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // exibição das informações de conexão no monitor serial
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo, OUTPUT);
  pinMode(led_verde, OUTPUT);
  pinMode(led_vermelho, OUTPUT);

  // Inicialização das entradas
  pinMode(buttonPin, INPUT); // inicializa o pino do botão

  // inicializa os LEDs apagados
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  setup_wifi();

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  // realiza a leitura do sensor de luminosidade
  int ldrstatus = analogRead(ldrPin);

  // verifica se 
  if(ldrstatus <= threshold){
    Serial.print("Modo Noturno");
    Serial.println(ldrstatus);
    digitalWrite(led_amarelo, HIGH);
    delay(1000);

  }else{
    Serial.print("Modo de operação normal");
    Serial.println(ldrstatus);

    // acende o led verde por 3 segundos
    digitalWrite(led_amarelo, LOW);
    digitalWrite(led_verde, HIGH);
    digitalWrite(led_vermelho, LOW);
    delay(3000);

    // acende o led amarelo por 2 segundos
    digitalWrite(led_amarelo, HIGH);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_vermelho, LOW);
    delay(2000);

    // acende o led vermelho por 5 segundos
    digitalWrite(led_amarelo, LOW);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_vermelho, HIGH);
    delay(5000);

  }

  // verifica o estado do botão
  buttonState = digitalRead(buttonPin);

  // Verifica estado do botão
  if (buttonState == LOW & digitalRead(led_vermelho) == HIGH) {
    Serial.println("Botão pressionado!");
    delay(1000);
    digitalWrite(led_verde, HIGH);
    digitalWrite(led_vermelho, LOW);
    digitalWrite(led_amarelo, LOW);
} else {
  Serial.println("Botão não pressionado!");
}

}