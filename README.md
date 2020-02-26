# hava-kalitesesi-belirleme
Hava Kalitesi Ölçüm Aracı

//Proje tam ve fonksiyonel değil, sensörlerden veri ölçülebiliyor

Proje FreeRTOS kullanılarak gerçek zamanlı sistem oluşturmak üzere kodlandı.
Amaç ortamın hava kalitesini, sıcaklık ve nemini ölçmek ve internet ile paylaşmak.

NOT: Esp8266 kullanımı tamamlanamadı, internet bağlantısı çalışmıyor. 
FreeRTOS kullanıldığı için AT komutları zamanlama konusunda uyumsuzluk yarattı.


Kullanılan sensörler: 
DHT11 Arduino Sensör Modulü,
MQ-135 Hava Kalitesi Ölçüm Modülü,
Esp8266 Seri Wifi Modül,
LDR led diyot,
HC-SR04 Arduino Ultrasonik Mesafe Sensörü  X3
