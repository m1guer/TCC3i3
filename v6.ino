#include <ESP8266WiFi.h>  
#include <PubSubClient.h>

#define D0    16
#define D1    5
#define D2    4
#define D3    0
#define D4    2
#define D5    14
#define D6    12
#define D7    13
#define D8    15
#define D9    3
#define D10   1
  
//defines
#define SSID_REDE     "Wifi-V" 
#define SENHA_REDE    "12151804v" 
#define INTERVALO_ENVIO_THINGSPEAK  30000 
#define INTERVALO_ENVIO_MQTT        10000  
#define LIMITE_UMIDADE_PARA_REGAR   30    
#define TEMPO_PARA_REGAR            2000   
#define SAIDA_COMANDO_VALVULA       D0     
   
#define TOPICO_SUBSCRIBE "AquaInsights_Send"    
#define TOPICO_PUBLISH   "AquaInsights_Recive"   
                                                 
#define ID_MQTT  "planta_regg"     
void initWiFi(void);   

const char* BROKER_MQTT = "192.168.0.102"; 
int BROKER_PORT = 1883;                      
char EnderecoAPIThingSpeak[] = "api.thingspeak.com";
String ChaveEscritaThingSpeak = "5372SFD56DLEKMDJ";       
long lastConnectionTime; 
long lastMQTTSendTime;
WiFiClient client;
WiFiClient clientMQTT;
PubSubClient MQTT(clientMQTT);
   
//prototypes
void EnviaInformacoesThingspeak(String StringDados);
float FazLeituraUmidade(void);

void initMQTT(void);
void reconectWiFi(void); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void); 
  

void EnviaInformacoesThingspeak(String string_dados)
{
    if (client.connect(EnderecoAPIThingSpeak, 80))
    {
        /* faz a requisição HTTP ao ThingSpeak */
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+ChaveEscritaThingSpeak+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(string_dados.length());
        client.print("\n\n");
        client.print(string_dados);
         
        lastConnectionTime = millis();
        Serial.println("- Informações enviadas ao ThingSpeak!");
    }
}
   

void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID_REDE);
    Serial.println("Aguarde");
       
    reconectWiFi();
}
    

void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);  
    MQTT.setCallback(mqtt_callback);            
}
    

void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
          
}
    

void reconnectMQTT() 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
    
//Função: reconecta-se ao WiFi

void reconectWiFi() 
{

    if (WiFi.status() == WL_CONNECTED)
        return;
           
    WiFi.begin(SSID_REDE, SENHA_REDE);
       
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
     
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID_REDE);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}
   

void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT();
       
     reconectWiFi();
}
   

//Parâmetros: nenhum
//Retorno: umidade percentual (0-100)
float FazLeituraUmidade(void)
{
    int ValorADC;
    float UmidadePercentual;
   
     ValorADC = analogRead(0);  
     Serial.print("[Leitura ADC] ");
     Serial.println(ValorADC);
   
   
     //   Valor lido                 Umidade percentual
     //      _    0                           _ 100
     //      |                                |   
     //      |                                |   
     //      -   ValorADC                     - UmidadePercentual 
     //      |                                |   
     //      |                                |   
     //     _|_  418                         _|_ 0
     //
     //   (UmidadePercentual-0) / (100-0)  =  (ValorADC - 418) / (-418)
     //      Logo:
     //      UmidadePercentual = 100 * ((418-ValorADC) / 418)  
        
     UmidadePercentual = 100 * ((418-(float)ValorADC) / 418);
     Serial.print("[Umidade Percentual] ");
     Serial.print(UmidadePercentual);
     Serial.println("%");
   
     return UmidadePercentual;
}
void setup()
{  
    Serial.begin(9600);
    lastConnectionTime = 0; 
    lastMQTTSendTime = 0;
    initWiFi();
    initMQTT();
    pinMode(SAIDA_COMANDO_VALVULA,OUTPUT);
    digitalWrite(SAIDA_COMANDO_VALVULA,LOW); 
    Serial.println("Planta IoT com ESP8266 NodeMCU");
}
   
//loop principal
void loop()
{
    float UmidadePercentualLida;
    int UmidadePercentualTruncada;
    char FieldUmidade[11];
    char MsgUmidadeMQTT[50];


    
    VerificaConexoesWiFIEMQTT(); 
       

    if (client.connected())
    {
        client.stop();
        Serial.println("- Desconectado do ThingSpeak");
        Serial.println();
    }
   
    UmidadePercentualLida = FazLeituraUmidade();
    UmidadePercentualTruncada = (int)UmidadePercentualLida;
  

    if(!client.connected() && 
      ((millis() - lastConnectionTime) > INTERVALO_ENVIO_THINGSPEAK))
    {
        sprintf(FieldUmidade,"field1=%d",UmidadePercentualTruncada);
        EnviaInformacoesThingspeak(FieldUmidade);
    }
   

    if ((millis() - lastMQTTSendTime) > INTERVALO_ENVIO_MQTT)
    {
        sprintf(MsgUmidadeMQTT,"%d",UmidadePercentualTruncada);
        MQTT.publish(TOPICO_PUBLISH, MsgUmidadeMQTT);
        lastMQTTSendTime = millis();
    }
 
    if (UmidadePercentualTruncada <= LIMITE_UMIDADE_PARA_REGAR)
    {
        digitalWrite(SAIDA_COMANDO_VALVULA,HIGH);  
        delay(TEMPO_PARA_REGAR);
        digitalWrite(SAIDA_COMANDO_VALVULA,LOW);
    }
      
    delay(1000);
}
