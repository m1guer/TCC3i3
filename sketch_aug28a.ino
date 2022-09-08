#include <ESP8266WiFi.h>  //essa biblioteca já vem com a IDE. Portanto, não é preciso baixar nenhuma biblioteca adicional
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient
 
//defines - mapeamento de pinos do NodeMCU
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
#define SSID_REDE     "Wifi-V"  //coloque aqui o nome da rede que se deseja conectar
#define SENHA_REDE    "12151804v"  //coloque aqui a senha da rede que se deseja conectar
#define INTERVALO_ENVIO_THINGSPEAK  5000  //intervalo entre envios de dados ao ThingSpeak (em ms)
#define INTERVALO_ENVIO_MQTT        10000  //intervalo entre envios de dados via MQTT (em ms)
#define LIMITE_UMIDADE_PARA_REGAR   30     //limite percentual inferior para a planta se auto regar
#define TEMPO_PARA_REGAR            2000   //tempo (em ms) ao qual a válvula de vazão solenóide de água será acionada quando for necessário regar a planta 
#define SAIDA_COMANDO_VALVULA       D0     //saída do NodeMCU que acionará a válvula de vazão solenóide de água
   
//defines de id mqtt e tópicos para publicação e subscribe
#define TOPICO_SUBSCRIBE "MQTTFFPlanasdasdastaIoTEnvia"     //tópico MQTT de escuta
#define TOPICO_PUBLISH   "MQTTFFPlaasdasdntaIoTRecebe"    //tópico MQTT de envio de informações para Broker
                                                  //IMPORTANTE: recomendamos fortemente alterar os nomes
                                                  //            desses tópicos. Caso contrário, há grandes
                                                  //            chances de você controlar e monitorar o NodeMCU
                                                  //            de outra pessoa.
#define ID_MQTT  "PlantaIotPaasdasrte3"     //id mqtt (para identificação de sessão)
                                       //IMPORTANTE: este deve ser único no broker (ou seja, 
                                       //            se um client MQTT tentar entrar com o mesmo 
                                       //            id de outro já conectado ao broker, o broker 
                                       //            irá fechar a conexão de um deles).
   
//constantes e variáveis globais
const char* BROKER_MQTT = "test.mosquitto.org"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883;                      // Porta do Broker MQTT
char EnderecoAPIThingSpeak[] = "api.thingspeak.com";
String ChaveEscritaThingSpeak = "R4NUDKQUDDDP7KJM";        //coloque aqui sua chave de escrita do seu canal no ThingSpeak
long lastConnectionTime; 
long lastMQTTSendTime;
WiFiClient client;
WiFiClient clientMQTT;
PubSubClient MQTT(clientMQTT); // Instancia o Cliente MQTT passando o objeto clientMQTT
   
//prototypes
void EnviaInformacoesThingspeak(String StringDados);
float FazLeituraUmidade(void);
void initWiFi(void);
void initMQTT(void);
void reconectWiFi(void); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void); 
  
/*
 * Implementações
 */
   
//Função: envia informações ao ThingSpeak
//Parâmetros: String com a  informação a ser enviada
//Retorno: nenhum
void EnviaInformacoesThingspeak(String StringDados)
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
        client.print(StringDados.length());
        client.print("\n\n");
        client.print(StringDados);
         
        lastConnectionTime = millis();
        Serial.println("- Informações enviadas ao ThingSpeak!");
    }  
}
   
//Função: inicializa e conecta-se na rede WI-FI desejada
//Parâmetros: nenhum
//Retorno: nenhum
void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID_REDE);
    Serial.println("Aguarde");
       
    reconectWiFi();
}
    
//Função: inicializa parâmetros de conexão MQTT(endereço do 
//        broker, porta e seta função de callback)
//Parâmetros: nenhum
//Retorno: nenhum
void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}
    
//Função: função de callback 
//        esta função é chamada toda vez que uma informação de 
//        um dos tópicos subescritos chega)
//Parâmetros: nenhum
//Retorno: nenhum
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
          
}
    
//Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
//        em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
//Parâmetros: nenhum
//Retorno: nenhum
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
//Parâmetros: nenhum
//Retorno: nenhum
void reconectWiFi() 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
           
    WiFi.begin(SSID_REDE, SENHA_REDE); // Conecta na rede WI-FI
       
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
   
//Função: verifica o estado das conexões WiFI e ao broker MQTT. 
//        Em caso de desconexão (qualquer uma das duas), a conexão
//        é refeita.
//Parâmetros: nenhum
//Retorno: nenhum
void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
       
     reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
   
//Função: faz a leitura do nível de umidade
//Parâmetros: nenhum
//Retorno: umidade percentual (0-100)
//Observação: o ADC do NodeMCU permite até, no máximo, 1V. Dessa forma,
//            para 1V, obtem-se (empiricamente) 418 como leitura de ADC
float FazLeituraUmidade(void)
{
    int ValorADC;
    float UmidadePercentual;
   
     ValorADC = analogRead(0);   //418 -> 1.0V
     Serial.print("[Leitura ADC] ");
     Serial.println(ValorADC);
   
     //Quanto maior o numero lido do ADC, menor a umidade.
     //Sendo assim, calcula-se a porcentagem de umidade por:
     //      
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
    digitalWrite(SAIDA_COMANDO_VALVULA,LOW);  //por default, a válvula de vazão solenóide de água começa fechada    
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
       
    //Força desconexão ao ThingSpeak (se ainda estiver desconectado)
    if (client.connected())
    {
        client.stop();
        Serial.println("- Desconectado do ThingSpeak");
        Serial.println();
    }
   
    UmidadePercentualLida = FazLeituraUmidade();
    UmidadePercentualTruncada = (int)UmidadePercentualLida; //trunca umidade como número inteiro
       
    //verifica se está conectado no WiFi e se é o momento de enviar dados ao ThingSpeak
    if(!client.connected() && 
      ((millis() - lastConnectionTime) > INTERVALO_ENVIO_THINGSPEAK))
    {
        sprintf(FieldUmidade,"field1=%d",UmidadePercentualTruncada);
        EnviaInformacoesThingspeak(FieldUmidade);
    }
   
    //verifica se é o momento de enviar informações via MQTT
    if ((millis() - lastMQTTSendTime) > INTERVALO_ENVIO_MQTT)
    {
        sprintf(MsgUmidadeMQTT,"- Umidade do solo: %d porcento.",UmidadePercentualTruncada);
        MQTT.publish(TOPICO_PUBLISH, MsgUmidadeMQTT);
        lastMQTTSendTime = millis();
    }
 
    //verifica se a planta deve ser regada
    if (UmidadePercentualTruncada <= LIMITE_UMIDADE_PARA_REGAR)
    {
        //em caso positivo, a planta é regada no tempo contido no define TEMPO_PARA_REGAR
        digitalWrite(SAIDA_COMANDO_VALVULA,HIGH);  //abre a válvula de vazão solenóide de água
        delay(TEMPO_PARA_REGAR);
        digitalWrite(SAIDA_COMANDO_VALVULA,LOW);  //fecha a válvula de vazão solenóide de água
    }
      
    delay(1000);
}
