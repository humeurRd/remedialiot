#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "HTTPClient.h"

#define LED_BUILTIN 4

/** Tiempo de puesta en linea del controlador */
const char compileDate[] = __DATE__ " " __TIME__;

/** Nombre unico del dispositivo */
char apName[] = "ESP32-xxxxxxxxxxxx";

/** Clase de cliente WIFI */
WiFiClient client;
/** Clase de cleinte HTTP */
HTTPClient http;

/** Crendenciales de la red wifi local*/
const char *ssid = ""; //Colorcar nombre del wifi
const char *password = ""; //Colocar contraseña

/** Variable que almacena el estado del led */
volatile bool status = false;

/** Crear nombre unco a traves de la MAC */
void createName()
{
  uint8_t baseMac[6];
  /** Obetener la mac a partir del wifi */
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  /** Escribir nombre unico */
  sprintf(apName, "ESP32-%02X%02X%02X%02X%02X%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
}

/** Solicitar estado al servidor */
void requestJSON()
{
  /** URL de nuestro host donde estan los datos */
  String url = "http://pinware.tech/iotResp/json/ESP32-3C61053095C8.json";

  /** Inicio de la peticion */
  if (http.begin(client, url))
  {
    Serial.print("[HTTP] GET...\n");
    int httpCode = http.GET();
    if (httpCode > 0)
    {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        /** Obtener respuesta en un string */
        String payload = http.getString();
        Serial.println(payload);
        /** Deserializar la respuesta en variables */
        String json = payload;
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, json);
        if (error)
        {
          return;
        }

        /** Variables con los datos obtenidos del json*/
        String opc = doc["estado"];
        String aux = doc["fecha"];
        fecha = aux;

        /** Cambiar la variable del estado segun lo obtenido en el JSON*/
        if (opc == "on")
        {
          status = true;
        }
        else
        {
          status = false;
        }
      }
    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  else
  {
    Serial.printf("[HTTP} Unable to connect \n");
  }
  delay(100);
}

void setup()
{
  /** Iniciar conexion con el puerto serial*/
  Serial.begin(115200);
  // crear nombre
  createName();
  // declarar como salida el pin del led
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.print("Build: ");
  Serial.println(compileDate);

  //incio de las config de wifi
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ESP32-CAM IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  //imprimir el nombre del dispositivo
  Serial.println("Device name: " + String(apName));
  //llamada al metodo que solicita json
  requestJSON();
  if (status)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("\n");
    Serial.println("Estado del led: ON  Fecha: " + fecha);
  }
  if (!status)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("\n");
    Serial.println("Estado del led: OFF  Fecha: " + fecha);
  }
  delay(1000);
}
