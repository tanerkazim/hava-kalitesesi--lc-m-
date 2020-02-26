#include <Arduino_FreeRTOS.h>
#include <frt.h>
#include <dht11.h>
#define ag_ismi "null" //senin ağın
#define ag_sifresi "null" //ağ şifresi

String yollanacak_komut = "";
String metin = "";
const int32_t inSag1 = 2;
const int32_t inSag2 = 3;
const int32_t inSol1 = 4;
const int32_t inSol2 = 5;
int32_t solTrigPin = 6;
int32_t solEchoPin = 7;
int32_t sagTrigPin = 8;
int32_t sagEchoPin = 9;
int32_t ortaTrigPin = 12;
int32_t ortaEchoPin = 13;
int DHT11_pin=10;
const int32_t pwmSol = 38;
const int32_t pwmSag = 39;
long sureOrta;
long mesafeOrta;
long sureSol;
long mesafeSol;
long sureSag;
long mesafeSag;
int chk;
float sicaklikD;
float nemD;
float havaD;
float isikD;

dht11 DHT11_sensor;

void setup() {

  pinMode(inSag1, OUTPUT);
  pinMode(inSag2, OUTPUT);
  pinMode(inSol1, OUTPUT);
  pinMode(inSol2, OUTPUT);
  pinMode(pwmSag, OUTPUT);
  pinMode(pwmSol, OUTPUT);
  pinMode(ortaTrigPin, OUTPUT);
  pinMode(ortaEchoPin, INPUT);
  pinMode(solTrigPin, OUTPUT);
  pinMode(solEchoPin, INPUT);
  pinMode(sagTrigPin, OUTPUT);
  pinMode(sagEchoPin, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(115200);
xTaskCreate(mesafeOn,
              (const portCHAR*)"TASK 1",
              100,
              NULL,
              1,
              NULL);
xTaskCreate(mesafeR,
              (const portCHAR*)"TASK 2",
              100,
              NULL,
              1,
              NULL);
xTaskCreate(mesafeL,
              (const portCHAR*)"TASK 3",
              100,
              NULL,
              1,
              NULL);
xTaskCreate(Haberlesme,
              (const portCHAR*)"TASK 4",
              100,
              NULL,
              1,
              NULL);
xTaskCreate(Sicaklik,
              (const portCHAR*)"TASK 5",
              100,
              NULL,
              1,
              NULL);
  xTaskCreate(Hava,
              (const portCHAR*)"TASK 6",
              100,
              NULL,
              1,
              NULL);
xTaskCreate(Isik,
              (const portCHAR*)"TASK 7",
              100,
              NULL,
              1,
              NULL);
  xTaskCreate(Yurru,
              (const portCHAR*)"TASK 8",
              100,
              NULL,
              1,
              NULL);



  Serial.println("AT"); //ESP modülümüz ile bağlantı kurulup kurulmadığını kontrol ediyoruz.
  
  delay(3000); //ESP ile iletişim için 3 saniye bekliyoruz.
 
  if(Serial.find("OK")){         //esp modülü ile bağlantıyı kurabilmişsek modül "AT" komutuna "OK" komutu ile geri dönüş yapıyor.
     Serial.println("AT+CWMODE=1"); //esp modülümüzün WiFi modunu STA şekline getiriyoruz. Bu mod ile modülümüz başka ağlara bağlanabilecek.
     delay(2000);
     String baglantiKomutu=String("AT+CWJAP=\"")+ag_ismi+"\",\""+ag_sifresi+"\"";
    Serial.println(baglantiKomutu);
     delay(5000);
 }
 
   Serial.print("AT+CIPMUX=1\r\n"); //modülümüzü TCP/UDP bağlantısına sokuyoruz.
   delay(200);
   Serial.print("AT+CIPSERVER=1,80\r\n"); //Sunucumuzu 80 portunda açıyoruz
   delay(500);
}

void loop() {

}

void mesafeOn(void *pvParameters) {
  for (;;) {
    //ORTA MESAFE SENSÖRÜ
    digitalWrite(ortaTrigPin, LOW);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    digitalWrite(ortaTrigPin, HIGH);
    vTaskDelay(40 / portTICK_PERIOD_MS);
    digitalWrite(ortaTrigPin, LOW);
    sureOrta = pulseIn(ortaEchoPin, HIGH);
    mesafeOrta = sureOrta / 29.1 / 2;
    /*Serial.print("Orta :");
    Serial.print(mesafeOrta);
    Serial.print("\n");
    vTaskDelay(10 / portTICK_PERIOD_MS);
    */
  }
  
}
void mesafeR(void *pvParameters) {
  for (;;) {
    // SAĞDA KULLANILAN SENSÖR
    digitalWrite(sagTrigPin, LOW);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    digitalWrite(sagTrigPin, HIGH);
    vTaskDelay(40 / portTICK_PERIOD_MS);
    digitalWrite(sagTrigPin, LOW);
    sureSag = pulseIn(sagEchoPin, HIGH);
    mesafeSag = sureSag / 29.1 / 2;
    /*Serial.print("Sağ :");
    Serial.println(mesafeSag);
    Serial.print("\n");
    vTaskDelay(10 / portTICK_PERIOD_MS);
    */
  }
  
}

void mesafeL(void *pvParameters) {
  for (;;) {
    // SOLDA KULLANILAN SENSÖR
    digitalWrite(solTrigPin, LOW);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    digitalWrite(solTrigPin, HIGH);
    vTaskDelay(40 / portTICK_PERIOD_MS);
    digitalWrite(solTrigPin, LOW);
    sureSol = pulseIn(solEchoPin, HIGH);
    mesafeSol = sureSol / 29.1 / 2;
    /*Serial.print("Sol :");
    Serial.println(mesafeSol);
    Serial.print("\n");
    vTaskDelay(10 / portTICK_PERIOD_MS);
    */
  }
  
}

void Yurru(void *pvParameters) {
  for (;;) {
    
    if (mesafeOrta > 12) {
      digitalWrite(inSag1, HIGH);
      digitalWrite(inSol1, HIGH);
      digitalWrite(inSag2, LOW);
      digitalWrite(inSol2, LOW);
      analogWrite(pwmSol, 150);
      analogWrite(pwmSag, 150);
      vTaskDelay(200 / portTICK_PERIOD_MS);
      digitalWrite(inSag1, LOW);
      digitalWrite(inSol1, LOW);
      digitalWrite(inSag2, LOW);
      digitalWrite(inSol2, LOW);
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    else if (mesafeOrta <= 12) {
      digitalWrite(inSag1, LOW);
      digitalWrite(inSol1, LOW);
      digitalWrite(inSag2, LOW);
      digitalWrite(inSol2, LOW);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      if (mesafeSag > mesafeSol) {
        digitalWrite(inSag1, LOW);
        digitalWrite(inSag2, HIGH);
        digitalWrite(inSol1, HIGH);
        digitalWrite(inSol2, LOW);
        analogWrite(pwmSol, 150);
        analogWrite(pwmSag, 150);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        digitalWrite(inSag1, LOW);
        digitalWrite(inSol1, LOW);
        digitalWrite(inSag2, LOW);
        digitalWrite(inSol2, LOW);
        vTaskDelay(500 / portTICK_PERIOD_MS);
      }
      else if (mesafeSag < mesafeSol) {
        digitalWrite(inSag1, HIGH);
        digitalWrite(inSag2, LOW);
        digitalWrite(inSol1, LOW);
        digitalWrite(inSol2, HIGH);
        analogWrite(pwmSol, 150);
        analogWrite(pwmSag, 150);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        digitalWrite(inSag1, LOW);
        digitalWrite(inSol1, LOW);
        digitalWrite(inSag2, LOW);
        digitalWrite(inSol2, LOW);
        vTaskDelay(500 / portTICK_PERIOD_MS);
      }
      else {
        digitalWrite(inSag1, LOW);
        digitalWrite(inSol1, LOW);
        digitalWrite(inSag2, HIGH);
        digitalWrite(inSol2, HIGH);
        analogWrite(pwmSol, 150);
        analogWrite(pwmSag, 150);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        digitalWrite(inSag1, LOW);
        digitalWrite(inSol1, LOW);
        digitalWrite(inSag2, LOW);
        digitalWrite(inSol2, LOW);
        vTaskDelay(500 / portTICK_PERIOD_MS);
      }
    }

  }
}


void Sicaklik(void *pvParameters) {
  for (;;) {
    chk = DHT11_sensor.read(DHT11_pin);
    nemD = (float)DHT11_sensor.humidity;
    vTaskDelay(40 / portTICK_PERIOD_MS);
    sicaklikD = (float)DHT11_sensor.temperature;
    vTaskDelay(40 / portTICK_PERIOD_MS);
    Serial.println(nemD);
    Serial.println(sicaklikD);
    
  }
}

void Hava(void *pvParameters) {
  for (;;) {
    havaD = analogRead(A0);
    vTaskDelay(40 / portTICK_PERIOD_MS);
    Serial.println(havaD);
  }
}
void Isik(void *pvParameters) {
  for (;;) {
    isikD = analogRead(A1);
    vTaskDelay(40 / portTICK_PERIOD_MS);
    Serial.println(isikD);
    
  }
}

void Haberlesme(void *pvParameters) {
  for (;;) {
    if(Serial.available()>0){  //Eğer gelen veri varsa gir
    if(Serial.find("+IPD,")){  //Gelen verinin içerisinde "+IPD" komutu geçiyorsa gir
      yollanacak_komut = "";
      metin += "<h2>ŞU ANKİ - Sıcaklık: ";
      metin += (String)sicaklikD;
      metin +="  Nem Oranı: ";
      metin +=(String)nemD;
      metin +="  Hava Kalitesi: ";
      metin +=(String)havaD;
      metin +="  Işık Şiddeti: ";
      metin +=(String)isikD;
      metin += " </h2> <br>";
      yollanacak_komut += "AT+CIPSEND=";  //yollayacağımız komutun boyutunu söylediğimiz kısım.
      yollanacak_komut +="0";
      yollanacak_komut +=",";
      yollanacak_komut += metin.length(); //yollayacağımız yazının uzunluğunu belirliyor.
      yollanacak_komut += "\r\n";
      Serial.print(yollanacak_komut);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      Serial.println(metin);
      vTaskDelay(2500 / portTICK_PERIOD_MS);
      Serial.println("AT+CIPCLOSE=0"); //Server'ı kapatıyoruz.
      vTaskDelay(500 / portTICK_PERIOD_MS);
      Serial.print("AT+CIPMUX=1\r\n"); //modülümüzü TCP/UDP bağlantısına sokuyoruz.
      vTaskDelay(200 / portTICK_PERIOD_MS);
      Serial.print("AT+CIPSERVER=1,80\r\n"); //Sunucumuzu 80 portunda açıyoruz
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  }
  }
}
