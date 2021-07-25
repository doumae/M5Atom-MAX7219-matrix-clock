/*

  M5Atom MAX7219 MatrixLED clock
  https://parame.mwj.jp/

  G22    CLK
  G19    CS
  G23    DIN
  5V     VCC
  GND    GND

  Library
  http://wayoda.github.io/LedControl/

*/

#include <WiFi.h>
#include "time.h"
#include "LedControl.h"

#include "net.h"
#include "v4font.h"

const char* ssid = WIFI_SSID;
const char* pass = WIFI_PASS;

const char* ntpServer =  NTP_SERV;
const long  gmtOffset_sec = NTP_TZ;
const int   daylightOffset_sec = 0;

// LedControl (DIN, CLK, LOAS/CS, module)
LedControl lc = LedControl(23, 22, 19, 4);

void setup() {
  Serial.begin(115200);

  for (int d = 0 ; d < 4; d++) {
    lc.shutdown(d, false);
    lc.setIntensity(d, 4);
    lc.clearDisplay(d);
  }

  lc.setLed(0, 0, 0, true);

  // WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}

void loop() {

  struct tm tm;

  if (getLocalTime(&tm)) {

    Serial.printf("%02d:%02d:%02d\n",
                  tm.tm_hour, tm.tm_min, tm.tm_sec);

    digit(0 , tm.tm_hour / 10);
    digit(5 , tm.tm_hour % 10);

    digit(11, tm.tm_min / 10);
    digit(16, tm.tm_min % 10);

    digit(22, tm.tm_sec / 10);
    digit(27, tm.tm_sec % 10);

  }

  delay (100);

}

int digit (int x, int d) {

  if (d < 0 || 9 < d) {
    return 0;
  }

  int addr;
  int col;
  int f;
  f = d * FONT_W;

  for (int i = 0; i < FONT_W; i++) {
    addr = (x + i) / 8;
    col = 7 - ((x + i) % 8);
    lc.setColumn(addr, col, vfont[f + i]);
  }

}
