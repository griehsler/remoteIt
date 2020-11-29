#include <Arduino.h>
#include <Ticker.h>
#include <InfluxDbClient.h>

#include <Network.h>
#include <Storage.h>
#include <Buttons.h>
#include <SwitchControl.h>
#include <Battery.h>

#include "Settings.h"
#include "LedNetworkStatusMonitor.h"
#include "PrivateSettings.h"

// #define SERIAL_OUTPUT
#define BATTERY_REPORT_INTERVAL 10 * 60 // seconds

Storage _storage;
Settings _settings;
LedNetworkStatusMonitor _statusMonitor(LED_BUILTIN);
Network _network(&_settings, &_statusMonitor);
Buttons _buttons;
Battery _battery(A0);

Ticker ticker;
bool reportBattery = false;
InfluxDBClient client;

void checkAndToggle(uint8_t triggeredPin, uint8_t targetPin, String targetHost)
{
  if (triggeredPin == targetPin)
  {
    Serial.print("switching by pin ");
    Serial.println(targetPin);
    SwitchControl control(targetHost);
    control.Toggle();
  }
}

void buttonChanged(uint8_t pin, bool pressed)
{
  Serial.println("detected button state change");
  if (!pressed) // don't react to button being released
    return;

  Serial.println("detected button press");

#if defined BUTTON1_PIN && defined BUTTON1_TARGETHOST
  checkAndToggle(pin, BUTTON1_PIN, BUTTON1_TARGETHOST);
#endif
#if defined BUTTON2_PIN && defined BUTTON2_TARGETHOST
  checkAndToggle(pin, BUTTON2_PIN, BUTTON2_TARGETHOST);
#endif
#if defined BUTTON3_PIN && defined BUTTON3_TARGETHOST
  checkAndToggle(pin, BUTTON3_PIN, BUTTON3_TARGETHOST);
#endif
#if defined BUTTON4_PIN && defined BUTTON4_TARGETHOST
  checkAndToggle(pin, BUTTON4_PIN, BUTTON4_TARGETHOST);
#endif
}

void triggerBatteryReport()
{
  reportBattery = true;
  Serial.println("setting marker for battery report");
}

void sendBatteryReport()
{
  int newRawValue = _battery.measureRaw();
  float newRelativeValue = _battery.normalize(newRawValue);

  Serial.print("sending new battery report, rawValue=");
  Serial.println(newRawValue);

  Point dataPoint("battery_status");
  dataPoint.addTag("device", _settings.hostName);
  dataPoint.addField("raw", newRawValue);
  dataPoint.addField("relative", newRelativeValue);

  client.writePoint(dataPoint);

  Serial.println("sent battery report");
}

void setup()
{
#ifdef SERIAL_OUTPUT
  Serial.begin(74880);
#endif

  _settings.hostName = HOSTNAME;
  _settings.otherAPSSID = WIFI_SSID;
  _settings.otherAPPassword = WIFI_PASSWORD;

  _settings.store(_storage);

  _network.setup();

#if defined BUTTON1_PIN && defined BUTTON1_TARGETHOST
  _buttons.registerButton(BUTTON1_PIN, ButtonMode::WaitForLow, buttonChanged);
#endif
#if defined BUTTON2_PIN && defined BUTTON2_TARGETHOST
  _buttons.registerButton(BUTTON2_PIN, ButtonMode::WaitForLow, buttonChanged);
#endif
#if defined BUTTON3_PIN && defined BUTTON3_TARGETHOST
  _buttons.registerButton(BUTTON3_PIN, ButtonMode::WaitForLow, buttonChanged);
#endif
#if defined BUTTON4_PIN && defined BUTTON4_TARGETHOST
  _buttons.registerButton(BUTTON4_PIN, ButtonMode::WaitForLow, buttonChanged);
#endif
  _buttons.setup();

  _battery.setup();
  client.setConnectionParamsV1(INFLUXDB_URL, INFLUXDB_DB_NAME, INFLUXDB_USER, INFLUXDB_PWD);
  client.setInsecure(true);
  ticker.attach(BATTERY_REPORT_INTERVAL, triggerBatteryReport);
}

void loop()
{
  _network.loop();
  _buttons.loop();

  if (reportBattery)
  {
    reportBattery = false;
    sendBatteryReport();
  }
}
