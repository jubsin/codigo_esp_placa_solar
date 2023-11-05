#include <WiFi.h>
#include <PubSubClient.h>

#define LED_1   2
#define LED_2   4

const char* ssid = "nome wifi";
const char* password = "senha wifi";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic = "esp32/topic";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  // Conectar-se à rede Wi-Fi
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");

  // Configurar o cliente MQTT
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);

  // Inscrever-se no tópico MQTT
  client.subscribe(mqtt_topic);
}

void loop() {
  if (!client.connected()) {
    // Reconectar-se ao broker MQTT
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado ao broker MQTT");
      // Reinscrever-se no tópico MQTT após a conexão bem-sucedida
      client.subscribe(mqtt_topic);
    } else {
      Serial.println("Falha na conexão ao broker MQTT. Tentando novamente em 5 segundos...");
      delay(5000);
      return;
    }
  }

  // Aguardar mensagens do tópico MQTT
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Mensagem recebida do tópico: " + String(topic));
  String mensagem = "";
  for (int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }

  // Processar a mensagem recebida do servidor
  Serial.println(mensagem);
  if (mensagem == "Dusty"){
    digitalWrite(LED_1, HIGH); // Acende o LED azul
    delay(5000);
    digitalWrite(LED_1, LOW);  // Apaga o LED azul
  }
  else{
    digitalWrite(LED_2, HIGH); // Acende o LED vermelho
    delay(5000);
    digitalWrite(LED_2, LOW);  // Apaga o LED vermelho
  }

}
