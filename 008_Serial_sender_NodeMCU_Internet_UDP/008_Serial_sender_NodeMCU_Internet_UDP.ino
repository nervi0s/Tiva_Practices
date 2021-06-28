#include <ESP8266WiFi.h>
#include<SoftwareSerial.h>
#include <WiFiUdp.h>

//#### CONFIGURACIONES GLOBALES DE LA PLACA NODEMCU ####//

// Variables de configuración de los pines
#define RX_PIN 3                                                          // Número de PIN de la placa NodeMCU usado para recibir datos de la comunicación serie (RX)
#define TX_PIN 1                                                          // Número de PIN de la placa NodeMCU usado para transmitir datos de la comunicación serie (TX)

// Variables auxiliares
#define MAX_BYTES 500                                                     // Define el número máximo de bytes que se enviarán por la comunicación serie

// Variables de comunicación en serie
SoftwareSerial serialCommunication(RX_PIN, TX_PIN);                       // Se crea una instancia de la clase SoftwareSerial usando los pines asignados como argumento. 'SoftwareSerial(rxPin, txPin, [inverse_logic])'

// Variables de Red
byte mac[] = { 0xA0, 0x20, 0xA6, 0x05, 0x89, 0x7D };                      // MAC de la placa NodeMCU (Viene de fábrica)
IPAddress local_ip (192, 168, 0, 111);                                    // Se le asigna a la placa NodeMCU una dirección IP estática
IPAddress gateway (192, 168, 0, 1);                                       // Puerta de enlace predeterminada para la placa
IPAddress subnet (255, 255, 255, 0);                                      // Máscara de subred donde está la placa
unsigned int local_port = 5500;                                           // Puerto local de la placa NodeMCU que estará escuchando
IPAddress remote_ip;                                                      // Variable donde se almacenará la ip remota con la que nos comunicaremos
unsigned int remote_port;                                                 // Puerto remoto con el que nos estamos comunicando
WiFiUDP udp_object;                                                       // Instancia de la clase WiFiUDP que nos permitirá enviar y recibir paquetes a través del protocolo UDP

// Variables de acceso a la red inalámbrica
const char* ssid =  "Vodafone6CA8";                                       // Nombre de la red a la que nos queremos conectar (El tipo dela red no debe ser 5G)
const char* pass =  "ZWMYKJZWQGN5EJnervi+";                               // Password de la red

// Buffer para recibir datos
char packetRecived[UDP_TX_PACKET_MAX_SIZE];                               // Array usado como buffer para almacenar paquetes entrantes
// Array para enviar datos por el puerto serie
char bytesToSerial[MAX_BYTES];                                            // Array usado como alamacenamineto de los datos que vamos a enviar a la Tiva mediante la comunicación serie

//#### CONFIGURACIONES INICIALES DE LA PLACA NODEMCU ####//

void setup() {
  serialCommunication.begin(9600);                                        // Abre el puerto serie y establece la velocidad de comunicación de la instancia en baudios, tanto la Tiva como esta placa NodeMCU deben tener la misma velocidad
  serialCommunication.setTimeout(5);                                      // Establece, en milisegundos, el tiempo máximo para esperar por datos de la comunicación serie. Por defecto son 1000 ms
  WiFi.config(local_ip, gateway, subnet);                                 // Configura a la placa con la IP, puerta de enlace y máscara de subred que le hemos dado como parámetros;
  WiFi.begin(ssid, pass);                                                 // Inicializa la librería WiFi y configura ajustes de red de la placa NodeMCU

  // Intentamos la conexión a la red inalámbrica
  while (WiFi.status() != WL_CONNECTED) {                                 // Esperamos que tengamos establecida la conexión WiFi
    serialCommunication.write("sin_conexion");                            // Mientras no haya conexión la placa manda por la conexión serie 'sin_conexion' como una serie de bytes en raw
    delay(1000);
  }

  udp_object.begin(local_port);                                           // Inicializa la instancia de WiFiUDP y configura ajustes de red de la placa NodeMCU
  serialCommunication.write("conexion_establecida");                      // Manda 1 vez por la conexión serie 'conexion_establecida' como una serie de bytes en raw
}


//#### LOOP INFINITO DE LA PLACA NODEMCU ####//
void loop() {
  ListenIncomingUDPPackets ();                                            // Escucha por si hay algún paquete UDP entrante y en caso afirmativo lo procesa llamando a 'SendActionToTiva()'

  CheckForSerialDataAvilable();                                           // Comprueba si hay datos que han llegado y están en el buffer  de recepción serie y en caso de haberlos los manda a procesar a otras funciones
}

//#### FUNCIONES AUXILIARES ####//

void ListenIncomingUDPPackets() {
  int incoming_package_size = udp_object.parsePacket();                   // Comprueba la presencia de un paquete UDP y nos devuelve su tamaño

  if (incoming_package_size) {
    //Si hay un paquete esperando se obtienen los datos del remitente (Controller Application)
    remote_ip = udp_object.remoteIP();
    remote_port = udp_object.remotePort();

    udp_object.read(packetRecived, UDP_TX_PACKET_MAX_SIZE);               // Se lee la información del paquete y se almacena la información de este en en array de chars 'packetRecived'
    String message = String(packetRecived);                               // Se transforma ese array de chars en un String

    SendActionToTiva(message);                                            // Manda el mensaje recibido por UDP a la Tiva usando la comunicación serie
    memset(packetRecived, 0, UDP_TX_PACKET_MAX_SIZE);                     // Limpia el contenido del array que recibe los paquetes UDP
  }
}

void CheckForSerialDataAvilable () {
  if (serialCommunication.available()) {                                  // Comprueba que existan bytes disponibles para leer desde el puerto serie, son datos que ya llegaron y se almacenaron en el buffer de recepción
    String data = serialCommunication.readString();                       // Leemos los datos que hay como un String.
    if (data.indexOf("_") == -1) {                                        // Si el mensaje no contiene '_' en su contenido, se trata de una solicitud de envío de pong 'pong_request' a la petición de ping de la aplicación a la placa NodeMCU y de esta a la Tiva
      sendPongToAplication();
    } else {
      sendPingUpdateToAplication(data);                                   // Si el mensaje contiene '_' se trata de una petición de actualización del estado de los pines en la aplicación controller
    }
  }
}

void SendActionToTiva(String command) {
  command.toCharArray(bytesToSerial, MAX_BYTES);                          // Se almacena el comando como una array de chars para poderlo mandar por el puerto serie
  serialCommunication.write(bytesToSerial);                               // Envía la información pasada como argumento a través de la comunicación serie // Manda al puerto serie datos, en este caso un String como una serie de bytes.
  memset(bytesToSerial, 0, MAX_BYTES);                                    // Limpia el contenido del array que almacena los datos para enviar por la comunicación serie
}

void sendPongToAplication() {                                             // Manda 'pong' a la aplicación para controlar que exista comunicación UDP con la placa
  udp_object.beginPacket(remote_ip, remote_port);
  udp_object.write("pong");
  udp_object.endPacket();
}

void sendPingUpdateToAplication(String pinUpdate) {                       // Debe mandar un mensaje (tipo: 'numPin_Num', EJM: '81_1') a la aplicación con el estado de un pin
  char charBuf[5];
  pinUpdate.toCharArray(charBuf, 5);
  udp_object.beginPacket(remote_ip, remote_port);
  udp_object.write(charBuf);
  udp_object.endPacket();
}
