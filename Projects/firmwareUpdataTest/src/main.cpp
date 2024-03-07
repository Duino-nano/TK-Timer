#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>

const char* ssid = "aterm-6922cd-a";
const char* password = "5794229c5110a";
const char* update_url =  "https://objects.githubusercontent.com/github-production-release-asset-2e65be/757218028/878db84f-12f1-451d-88c4-80a790ffedd7?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAVCODYLSA53PQK4ZA%2F20240214%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20240214T092404Z&X-Amz-Expires=300&X-Amz-Signature=383da0dc2b897be014926cf28e9d46fafb8109a177283fe153c6511b5ca492a4&X-Amz-SignedHeaders=host&actor_id=0&key_id=0&repo_id=757218028&response-content-disposition=attachment%3B%20filename%3Dfirmware.bin&response-content-type=application%2Foctet-stream";
void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected.");

  WiFiClientSecure client;
  client.setInsecure(); // 証明書の検証をスキップします。セキュリティ上の理由から、本番環境では使用しないでください。

 t_httpUpdate_return ret = httpUpdate.update(client, update_url);

  switch(ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP Update failed. Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("No Update Available.");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("Update OK."); // 更新成功時、デバイスは自動的に再起動します
      break;
  }
}

void loop() {
  // ここでは何もしません
}