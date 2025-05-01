#include <Arduino.h>
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui.h"
#define FORMAT_SPIFFS_IF_FAILED true
// Set the following to true for serial debugging
#define DEBUG false


volatile uint volume = 100;
bool vario = true;
uint luminosity = 100;
int telemetryprotocol = 1;
int saveconfig = 0;
int needrestart = 0;
uint32_t tellostcounter = 0;
bool alertvisible = false;
unsigned long lastupdate;
unsigned long lasttxbattupdate;
char buffer[30];

#include "FrSkySportSensor.h"
#include "FrSkySportSensorXjt.h"
#include "FrSkySportSensorGps.h"
#include "FrSkySportSensorVario.h"
#include "FrSkySportSingleWireSerial.h"
#include "FrSkySportDecoder.h"
#include "SoftwareSerial.h"
FrSkySportSensorXjt xjt;  // Create Xjt sensor with default ID
FrSkySportSensorGps gpssensor(FrSkySportSensor::ID_IGNORE);
FrSkySportSensorVario variosensor(FrSkySportSensor::ID_IGNORE);
FrSkySportDecoder decoder;  // Create decoder object without polling

#define TXBATTPARTRATIO 13.3
#define BATTERY_FILTER_BETA 2
#include "frsky.h"
#include "SoftwareSerial.h"
EspSoftwareSerial::UART swSer;
FrSky frsky(&swSer);

#define SWSERIAL_PIN 27
#define SWSERIAL_SPORTPIN FrSkySportSingleWireSerial::SOFT_SERIAL_PIN_27
#define TXBATT_PIN 35
#define TELEMETRYLOSTTIMEOUT 10000ull
#define MINREPEATTIME 10000ull

uint8_t redrawScreen = 0;
int16_t rssiRx = 0;
int16_t rssiTx = 0;
int16_t voltageBattery = 0;
uint8_t analog1 = 0;
uint8_t analog2 = 0;
float txbattvalue = 0;
float battrxalarm = 5;
float batttxalarm = 7.2;
float altitude;
float altitudeoffset;
float vspeed;

uint rssilowvalue = 42;
uint rssicritvalue = 39;
uint32_t lastrxbattalarm = 0;
uint32_t lasttxbattalarm = 0;
uint32_t lastrssilowalarm = 0;
uint32_t lastrssicritalarm = 0;
uint8_t analogs[2] = { 0, 0 };
String userDataString = "None Received";

void dataHandler(uint8_t a1, uint8_t a2, uint8_t q1, uint8_t q2) {
  redrawScreen = 1;
  // Low-Pass Filter for voltages
  analog1 = (((analog1 << BATTERY_FILTER_BETA) - analog1) + a1) >> BATTERY_FILTER_BETA;
  analog2 = (((analog2 << BATTERY_FILTER_BETA) - analog2) + a2) >> BATTERY_FILTER_BETA;
  rssiRx = q1;
  rssiTx = q2;
  analogs[0] = analog1;
  analogs[1] = analog2;
}

void alarmThresholdHandler(FrSky::AlarmThreshold alarm) {
}

void userDataHandler(const uint8_t *buf, uint8_t len) {
  redrawScreen = 1;
  String s;
  for (int i = 0; i < len; i++) {
    s += buf[i];
  }
  userDataString = s;
}


#include <TFT_eSPI.h>

#include <XPT2046_Touchscreen.h>
// A library for interfacing with the touch screen
//
// Can be installed from the library manager (Search for "XPT2046")
//https://github.com/PaulStoffregen/XPT2046_Touchscreen
// ----------------------------
// Touch Screen pins
// ----------------------------

// The CYD touch uses some non default
// SPI pins

#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33
SPIClass touchscreenSpi = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
uint16_t touchScreenMinimumX = 200, touchScreenMaximumX = 3700, touchScreenMinimumY = 240, touchScreenMaximumY = 3800;

extern objects_t objects;
/*Set to your screen resolution*/
#define TFT_HOR_RES 240
#define TFT_VER_RES 320

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

#include <driver/dac.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <math.h>

// Definizione degli eventi che il task audio può gestire
typedef enum {
  AUDIO_EVENT_NONE,
  AUDIO_EVENT_PLAY_STARTUP,
  AUDIO_EVENT_PLAY_TELEMETRY_LOST,
  AUDIO_EVENT_PLAY_TELEMETRY_RECOVERED,
  AUDIO_EVENT_PLAY_TXBATTLOW,
  AUDIO_EVENT_PLAY_RXBATTLOW,
  AUDIO_EVENT_PLAY_RSSILOW,
  AUDIO_EVENT_PLAY_RSSICRIT,
  // Aggiungi altri eventi se necessario
} audio_event_t;

// Global Handle for audio queue events
QueueHandle_t audio_event_queue;

// Audio files
#include "startup.h"
#include "telemko.h"
#include "telemok.h"
#include "rssilow.h"
#include "rssicrit.h"
#include "txbattlow.h"
#include "rxbattlow.h"

// Definitions for frequency and interval ranges (to be calibrated!)
#define VEL_THRESHOLD 0.1    // Threshold to consider actual ascent/descent
#define VEL_MAX_SALITA 5.0   // m/s, maximum mapped velocity for ascent
#define VEL_MAX_DISCESA 5.0  // m/s, maximum mapped velocity for descent (absolute value)

#define FREQ_MIN_SALITA 600       // Hz, lowest beep frequency for ascent
#define FREQ_MAX_SALITA 3000      // Hz, highest beep frequency for ascent
#define INTERVAL_MIN_SALITA 100   // ms, shortest interval between beeps for ascent
#define INTERVAL_MAX_SALITA 1000  // ms, longest interval between beeps for ascent
#define DURATA_BEEP_MS 80         // ms, duration of each single beep for ascent

#define FREQ_MIN_DISCESA 100  // Hz, lowest frequency for continuous descent sound (for strong descent)
#define FREQ_MAX_DISCESA 400  // Hz, highest frequency for continuous descent sound (for weak descent)

#define DAC_PIN_AUDIO 26  // Use GPIO25 for DAC1 (or 26 for DAC2)

float map_range(float x, float in_min, float in_max, float out_min, float out_max) {
  // Avoid division by zero in case in_min == in_max
  if (in_min == in_max) {
    return out_min;  // Or handle the error as you prefer
  }
  // Perform linear mapping
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Function to play an array of samples (for "Telemetry lost") - Adapted for Arduino
void play_audio_sample(const unsigned char *sample_data, unsigned int sample_len, unsigned int sample_rate) {
  if (sample_rate = 0) {
    return;
  }
  unsigned int sample_interval_us = 1000000 / sample_rate;
  const unsigned int YIELD_DURATION_US = 10000;
  unsigned int samples_per_yield;
  if (sample_interval_us > 0) {
    samples_per_yield = YIELD_DURATION_US / sample_interval_us;
  }
  if (samples_per_yield == 0) {
    samples_per_yield = 1;
  }
  int samples_processed_since_yield = 0;
  for (unsigned int i = 0; i < sample_len; i++) {
    dacWrite(DAC_PIN_AUDIO, (uint8_t)(128 + (((float)sample_data[i] - 128.0) * volume) / 100.0));  // Ho aggiunto .0 per assicurare float division
    delayMicroseconds(sample_interval_us);
    samples_processed_since_yield++;
    if (samples_processed_since_yield >= samples_per_yield) {
      vTaskDelay(pdMS_TO_TICKS(1));
      samples_processed_since_yield = 0;
    }
  }
}

void play_sine_waveform(int frequency, int duration_us) {
  const int samples_per_cycle = 64;  // Number of steps for one cycle (higher = more precise, slower)
  unsigned int total_samples_to_play = (unsigned int)((long)duration_us * frequency * samples_per_cycle / 1000000L);
  unsigned int sample_delay_us = 1000000 / (long)frequency / samples_per_cycle;

  for (unsigned int i = 0; i < total_samples_to_play; i++) {
    // Calculate the sinusoidal value (mapped from -1 to 1 to 0 to 255 for 8-bit unsigned DAC)
    uint8_t dac_value = (uint8_t)(128 + (127 * sin(2 * PI * i / samples_per_cycle) * volume / 100.0));
    dacWrite(DAC_PIN_AUDIO, dac_value);
    delayMicroseconds(sample_delay_us);
  }
  // Do not set the output to zero here
}

void audio_variometro_task(void *pvParameters) {
  float velocita_verticale = vspeed;
  audio_event_t received_event;
  unsigned long last_beep_time = 0;
  int last_beep_interval_ms = INTERVAL_MAX_SALITA;
  const int BUFFER_SIZE = 128;  // Sample buffer size
  static uint8_t sine_buffer[BUFFER_SIZE];
  static int buffer_freq = -1;  // -1 indicates the buffer needs to be calculated the first time or when frequency changes
  while (true) {
    velocita_verticale = vspeed;
    BaseType_t event_received = xQueueReceive(audio_event_queue, &received_event, pdMS_TO_TICKS(10));
    if (event_received == pdTRUE) {
      dacWrite(DAC_PIN_AUDIO, 128);
      delay(10);  // Short pause
      switch (received_event) {
        case AUDIO_EVENT_PLAY_STARTUP:
          play_audio_sample(startup, startup_len, startup_sr);
          break;
        case AUDIO_EVENT_PLAY_TELEMETRY_LOST:
          play_audio_sample(telemko, telemko_len, telemko_sr);
          break;
        case AUDIO_EVENT_PLAY_TELEMETRY_RECOVERED:
          play_audio_sample(telemok, telemok_len, telemok_sr);
          break;
        case AUDIO_EVENT_PLAY_TXBATTLOW:
          play_audio_sample(txbattlow, txbattlow_len, txbattlow_sr);
          break;
        case AUDIO_EVENT_PLAY_RXBATTLOW:
          play_audio_sample(rxbattlow, rxbattlow_len, rxbattlow_sr);
          break;
        case AUDIO_EVENT_PLAY_RSSILOW:
          play_audio_sample(rssilow, rssilow_len, rssilow_sr);
          break;
        case AUDIO_EVENT_PLAY_RSSICRIT:
          play_audio_sample(rssicrit, rssicrit_len, rssicrit_sr);
          break;
        default:
          break;
      }
      buffer_freq = -1;
      dacWrite(DAC_PIN_AUDIO, 128);
    } else if (vario) {
      if (velocita_verticale < -VEL_THRESHOLD) {  // Significant descent
        int frequenza_continua = map_range(fabs(velocita_verticale), VEL_THRESHOLD, VEL_MAX_DISCESA, FREQ_MAX_DISCESA, FREQ_MIN_DISCESA);
        if (frequenza_continua <= 0) frequenza_continua = 1;
        if (frequenza_continua != buffer_freq) {
          for (int i = 0; i < BUFFER_SIZE; i++) {
            sine_buffer[i] = (uint8_t)(128 + (127 * sin(2 * PI * i / BUFFER_SIZE) * volume / 100.0));
          }
          buffer_freq = frequenza_continua;
        }
        unsigned int delay_per_sample_us = 1000000 / ((long)frequenza_continua * BUFFER_SIZE);
        for (int i = 0; i < BUFFER_SIZE; i++) {
          dacWrite(DAC_PIN_AUDIO, sine_buffer[i]);
          delayMicroseconds(delay_per_sample_us);
        }
      } else if (velocita_verticale > VEL_THRESHOLD) {  // Significant ascent
        dacWrite(DAC_PIN_AUDIO, 128);                   // Silence between beeps
        buffer_freq = -1;                               // Reset descent buffer
        int frequenza_beep = map_range(velocita_verticale, VEL_THRESHOLD, VEL_MAX_SALITA, FREQ_MIN_SALITA, FREQ_MAX_SALITA);
        int intervallo_ms = map_range(velocita_verticale, VEL_THRESHOLD, VEL_MAX_SALITA, INTERVAL_MAX_SALITA, INTERVAL_MIN_SALITA);
        if (intervallo_ms < DURATA_BEEP_MS + 20) {
          intervallo_ms = DURATA_BEEP_MS + 20;
        }
        if (frequenza_beep <= 0) {
          frequenza_beep = 1;
        }
        unsigned long current_time = millis();
        if (current_time - last_beep_time >= last_beep_interval_ms) {
          play_sine_waveform(frequenza_beep, DURATA_BEEP_MS * 1000);
          last_beep_time = current_time;
          last_beep_interval_ms = intervallo_ms;
        }
        vTaskDelay(pdMS_TO_TICKS(20));
      } else {                                        // Level flight or almost
        dacWrite(DAC_PIN_AUDIO, 128);                 // Silence
        buffer_freq = -1;                             // Reset descent buffer state
        last_beep_time = 0;                           // Reset ascent beep timer
        last_beep_interval_ms = INTERVAL_MAX_SALITA;  // Reset ascent beep interval
        vTaskDelay(pdMS_TO_TICKS(100));
      }
    } else {
      dacWrite(DAC_PIN_AUDIO, 128);                 // Silence
      buffer_freq = -1;                             // Reset descent buffer state
      last_beep_time = 0;                           // Reset ascent beep timer
      last_beep_interval_ms = INTERVAL_MAX_SALITA;  // Reset ascent beep interval
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf) {
  LV_UNUSED(level);
#if DEBUG
  Serial.println(buf);
  Serial.flush();
#endif
}
#endif

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  /*Call it to tell LVGL you are ready*/
  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
  if (touchscreen.touched()) {
    TS_Point p = touchscreen.getPoint();
    //Some very basic auto calibration so it doesn't go out of range
    if (p.x < touchScreenMinimumX) touchScreenMinimumX = p.x;
    if (p.x > touchScreenMaximumX) touchScreenMaximumX = p.x;
    if (p.y < touchScreenMinimumY) touchScreenMinimumY = p.y;
    if (p.y > touchScreenMaximumY) touchScreenMaximumY = p.y;
    //Map this to the pixel position
    data->point.x = map(p.x, touchScreenMinimumX, touchScreenMaximumX, 1, TFT_HOR_RES); /* Touchscreen X calibration */
    data->point.y = map(p.y, touchScreenMinimumY, touchScreenMaximumY, 1, TFT_VER_RES); /* Touchscreen Y calibration */
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

lv_indev_t *indev;      //Touchscreen input device
uint8_t *draw_buf;      //draw_buf is allocated on heap otherwise the static area is too big on ESP32 at compile
uint32_t lastTick = 0;  //Used to track the tick timer

uint32_t currentTime, displayTime;
uint16_t decodeResult;

void setup() {
  // Initialization function
  // Some basic info on the Serial console
  String LVGL_Arduino = "LVGL demo ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
#if DEBUG
  Serial.begin(115200);
  Serial.println(LVGL_Arduino);
#endif
  // Load configuration from SPIFFS
  JsonDocument doc;
  if (SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
#if DEBUG
    Serial.println("Opening config.. ");
#endif
    if (SPIFFS.exists("/system.conf")) {
      File file = SPIFFS.open("/system.conf", FILE_READ);
      if (file) {
        DeserializationError error = deserializeJson(doc, file);
        if (!error) {
          // Read config values, use default if not found
          telemetryprotocol = doc["protocol"] | 1;
          luminosity = doc["luminosity"] | 100;
          volume = doc["volume"] | 100;
          vario = ((int)doc["vario"] == 1 ? true : false);
          battrxalarm = doc["battrxalarm"] | 5;
          batttxalarm = doc["batttxalarm"] | 7.2;
          rssilowvalue = doc["rssilowvalue"] | 42;
          rssicritvalue = doc["rssicritvalue"] | 39;
#if DEBUG
          // --- INIZIO DEBUG: Stampa i valori letti ---
          Serial.println("\nConfiguration values: ");
          Serial.print("telemetryprotocol: ");
          Serial.println(telemetryprotocol);
          Serial.print("luminosity: ");
          Serial.println(luminosity);
          Serial.print("volume: ");
          Serial.println(volume);
          Serial.print("vario: ");
          Serial.println(vario ? "true" : "false");  // Stampa true/false per il booleano
          Serial.print("battrxalarm: ");
          Serial.println(battrxalarm);
          Serial.print("batttxalarm: ");
          Serial.println(batttxalarm);
          Serial.print("rssilowvalue: ");
          Serial.println(rssilowvalue);
          Serial.print("rssicritvalue: ");
          Serial.println(rssicritvalue);
          // --- FINE DEBUG ---
#endif
        } else {
#if DEBUG
          Serial.println("Json deserialization error ");
#endif
          saveconf();  // Save default config if deserialization fails
        }
      } else {
#if DEBUG
        Serial.println("Opening config failed ");
#endif
        saveconf();  // Save default config if file open fails
      }
    } else {
#if DEBUG
      Serial.println("Config file not existing ");
#endif
      saveconf();  // Save default config if file doesn't exist
    }
  }

  // Initialise the touchscreen
  touchscreenSpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
  touchscreen.begin(touchscreenSpi);                                         /* Touchscreen init */
  touchscreen.setRotation(2);                                                /* Inverted landscape orientation to match screen */

  // Initialise LVGL GUI
  lv_init();

  // Setup LVGL display and input devices
  draw_buf = new uint8_t[DRAW_BUF_SIZE];
  lv_display_t *disp;
  disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE);
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);
  // Initialize the XPT2046 input device driver
  indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touchpad_read);

  // Integrate EEZ Studio GUI (initialize UI elements based on definition)
  ui_init();
  analogWrite(21, luminosity);  // Set screen brightness
  pinMode(4, OUTPUT);           // Configure pin 4 as output
  digitalWrite(4, 1);           // Set pin 4 high

  //setup hw pins
  pinMode(TXBATT_PIN, INPUT);
  pinMode(SWSERIAL_PIN, INPUT);  // Configure software serial pin as input

  // Configure telemetry based on selected protocol
  if (telemetryprotocol == 1) {
    // Configure the decoder serial port and sensors for protocol 1
    decoder.begin(SWSERIAL_SPORTPIN, &xjt, &gpssensor, &variosensor);
    lv_dropdown_set_selected(objects.proto, 1);  // Set protocol dropdown in UI
  } else {
    // Configure software serial and FrSky library for other protocol
    swSer.begin(9600, EspSoftwareSerial::SWSERIAL_8N1, SWSERIAL_PIN, -1, true);
    frsky.setDataHandler(&dataHandler);                      // Set data handler callback
    frsky.setAlarmThresholdHandler(&alarmThresholdHandler);  // Set alarm handler callback
    frsky.setUserDataHandler(&userDataHandler);              // Set user data handler callback
    lv_dropdown_set_selected(objects.proto, 0);              // Set protocol dropdown in UI
    // Hide UI elements not available in this protocol
    lv_obj_add_flag(objects.swr, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.swrbigtext, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(objects.a1, "A2");              // Rename A1 label to A2 (specific to this protocol?)
    lv_obj_add_flag(objects.a2, LV_OBJ_FLAG_HIDDEN);  // Hide A2 related elements
    lv_obj_add_flag(objects.a2bigtext, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.a2units, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.alt, LV_OBJ_FLAG_HIDDEN);  // Hide Altitude related elements
    lv_obj_add_flag(objects.altbigtext, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.altunits, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.vspd, LV_OBJ_FLAG_HIDDEN);  // Hide VSpeed related elements
    lv_obj_add_flag(objects.vspdbigtext, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.vspdunits, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.varioswitch, LV_OBJ_FLAG_HIDDEN);  // Hide Vario switch
  }

  // Set initial values for UI sliders and labels based on loaded config
  lv_slider_set_value(objects.luxvalue, luminosity, LV_ANIM_OFF);                 // Set luminosity slider
  lv_slider_set_value(objects.volvalue, volume, LV_ANIM_OFF);                     // Set volume slider
  lv_slider_set_value(objects.alarmvalue, (int)(battrxalarm * 10), LV_ANIM_OFF);  // Set RxBatt alarm slider
  sprintf(buffer, "%0.1f", battrxalarm);
  lv_label_set_text(objects.alarmvaluelabel, buffer);                               // Update RxBatt alarm label
  lv_slider_set_value(objects.alarmvaluetx, (int)(batttxalarm * 10), LV_ANIM_OFF);  // Set TxBatt alarm slider
  sprintf(buffer, "%0.1f", batttxalarm);
  lv_label_set_text(objects.alarmvaluetxlabel, buffer);                  // Update TxBatt alarm label
  lv_slider_set_value(objects.alarmrssilow, rssilowvalue, LV_ANIM_OFF);  // Set RSSI Low alarm slider
  sprintf(buffer, "%d", rssilowvalue);
  lv_label_set_text(objects.alarmrssilowlabel, buffer);                    // Update RSSI Low alarm label
  lv_slider_set_value(objects.alarmrssicrit, rssicritvalue, LV_ANIM_OFF);  // Set RSSI Critical alarm slider
  sprintf(buffer, "%d", rssicritvalue);
  lv_label_set_text(objects.alarmrssicritlabel, buffer);  // Update RSSI Critical alarm label

  // Set the state of the variometer switch based on config
  if (vario) {
    lv_obj_set_state(objects.varioswitch, LV_STATE_CHECKED, LV_ANIM_ON);
  } else {
    lv_obj_remove_state(objects.varioswitch, LV_STATE_CHECKED);
  }

  vspeed = 0.0;  // Initialize vertical speed to zero
  analogReadResolution(12);
  txbattvalue = ((analogRead(TXBATT_PIN) * TXBATTPARTRATIO) / 4095);
  // Create audio event queue and task
  audio_event_queue = xQueueCreate(10, sizeof(audio_event_t));  // Create a queue for 10 audio events
  if (audio_event_queue != NULL) {                              // Check if queue creation was successful
    xTaskCreatePinnedToCore(
      audio_variometro_task,  // Task function
      "AudioVarioTask",       // Task name
      8192,                   // Stack size (bytes)
      NULL,                   // Task parameters
      5,                      // Priority
      NULL,                   // Task handle (not used here)
      0);                     // Core to run on (Core 0)
  }

  // Send startup audio event
  audio_event_t event = AUDIO_EVENT_PLAY_STARTUP;
  xQueueSend(audio_event_queue, &event, 0);
}

void saveconf() {
  JsonDocument doc;
  File file = SPIFFS.open("/system.conf", FILE_WRITE);
  if (file) {
    doc.clear();
    doc["protocol"] = telemetryprotocol;
    doc["luminosity"] = luminosity;
    doc["volume"] = volume;
    doc["vario"] = (vario ? 1 : 0);
    doc["battrxalarm"] = battrxalarm;
    doc["batttxalarm"] = batttxalarm;
    doc["rssilowvalue"] = rssilowvalue;
    doc["rssicritvalue"] = rssicritvalue;
    serializeJson(doc, file);
    file.close();
#if DEBUG
    Serial.println("\nConfiguration saved");
  } else {
    Serial.println("\nAlert: Configuration notsaved");
#endif
  }
}


float value;  // Variable to temporarily store telemetry values
unsigned long now;
void loop() {
  // Main program loop
  now = millis();
  if ((now - lasttxbattupdate) > 1000ull) {
    lasttxbattupdate = now;
    txbattvalue = (txbattvalue * 4 + ((analogRead(TXBATT_PIN) * TXBATTPARTRATIO) / 4095)) / 5;
    sprintf(buffer, "%0.1f", txbattvalue);
    lv_label_set_text(objects.txbattbigtext, buffer);  // Update RxBatt text
    if (txbattvalue < batttxalarm && ((now - lasttxbattalarm) > MINREPEATTIME)) {
      lasttxbattalarm = now;
      audio_event_t event = AUDIO_EVENT_PLAY_TXBATTLOW;
      xQueueSend(audio_event_queue, &event, 0);
    }
  }
  if (telemetryprotocol == 1) {
    // Process telemetry for protocol 1 (S.Port)
    decodeResult = decoder.decode();  // Decode received telemetry data
    switch (decodeResult) {
      // Handle different telemetry data IDs
      case 0xF101:  // RSSI data
        value = xjt.getRssi();
        sprintf(buffer, "%0.0f", value);
        lv_bar_set_value(objects.rssibar, round(value), LV_ANIM_OFF);  // Update RSSI bar
        lv_label_set_text(objects.rssibigtext, buffer);                // Update RSSI large text
        lv_label_set_text(objects.rssivalue, buffer);                  // Update RSSI value text
        if (value != 0) {
          lastupdate = now;  // Record last update time if value is valid
          // Check and trigger RSSI critical/low alarms
          if (value < rssicritvalue) {
            if ((now - lastrssicritalarm) > MINREPEATTIME) {
              lastrssicritalarm = now;
              audio_event_t event = AUDIO_EVENT_PLAY_RSSICRIT;
              xQueueSend(audio_event_queue, &event, 0);
            }
          } else if (value < rssilowvalue) {
            if ((now - lastrssilowalarm) > MINREPEATTIME) {
              lastrssilowalarm = now;
              audio_event_t event = AUDIO_EVENT_PLAY_RSSILOW;
              xQueueSend(audio_event_queue, &event, 0);
            }
          }
        }
        break;
      case 0xF102:  // ADC1 data
        sprintf(buffer, "%0.1f", xjt.getAdc1());
        lv_label_set_text(objects.a1bigtext, buffer);  // Update ADC1 text
        break;
      case 0xF103:  // ADC2 data
        sprintf(buffer, "%0.1f", xjt.getAdc2());
        lv_label_set_text(objects.a2bigtext, buffer);  // Update ADC2 text
        break;
      case 0xF104:  // Receiver Battery Voltage
        value = xjt.getRxBatt();
        sprintf(buffer, "%0.1f", value);
        lv_bar_set_value(objects.rxbatt, value, LV_ANIM_OFF);  // Update RxBatt bar
        lv_label_set_text(objects.rxbattbigtext, buffer);      // Update RxBatt text
        // Check and trigger low RxBatt alarm
        if (value < battrxalarm && ((now - lastrxbattalarm) > MINREPEATTIME)) {
          lastrxbattalarm = now;
          audio_event_t event = AUDIO_EVENT_PLAY_RXBATTLOW;
          xQueueSend(audio_event_queue, &event, 0);
        }
        break;
      case 0xF105:  // SWR data
        value = xjt.getSwr();
        sprintf(buffer, "%0.0f", value);
        lv_label_set_text(objects.swrbigtext, buffer);  // Update SWR text
        break;
      case 0x0009:  // GPS Altitude (older FrSky ID)
        sprintf(buffer, "%d", xjt.getGpsAltitude());
        lv_label_set_text(objects.altvalue, buffer);  // Update GPS Alt text
        break;
      case 0x0016:  // Date
        sprintf(buffer, "%0d/%0d/20%0d", xjt.getDay(), xjt.getMonth(), xjt.getYear());
        lv_label_set_text(objects.datavalue, buffer);  // Update Date text
        break;
      case 0x0018:  // Time
        sprintf(buffer, "%0d:%0d:%0d", xjt.getHour(), xjt.getMinute(), xjt.getSecond());
        lv_label_set_text(objects.oravalue, buffer);  // Update Time text
        break;
      case 0x0019:  // GPS Speed (older FrSky ID)
        sprintf(buffer, "%d", xjt.getSpeed());
        lv_label_set_text(objects.speedvalue, buffer);  // Update GPS Speed text
        break;
      case 0x001C:  // Course Over Ground (COG) (older FrSky ID)
        sprintf(buffer, "%d", xjt.getCog());
        lv_label_set_text(objects.headvalue, buffer);  // Update Heading text
        break;
      case 0x0021:  // Barometric Altitude (newer FrSky ID)
        value = xjt.getAltitude();
        sprintf(buffer, "%0.0f", value / 100.0);
        lv_label_set_text(objects.altbigtext, buffer);  // Update large Altitude text
        break;
      case 0x0022:  // Longitude (newer FrSky ID)
        value = xjt.getLon();
        sprintf(buffer, "%0.7f", value);
        lv_label_set_text(objects.lonvalue, buffer);  // Update Longitude text
        break;
      case 0x0023:  // Latitude (newer FrSky ID)
        value = xjt.getLat();
        sprintf(buffer, "%0.7f", value);
        lv_label_set_text(objects.latvalue, buffer);  // Update Latitude text
        break;
      case 0x0030:  // Vertical Speed Indicator (VSI) (newer FrSky ID)
        value = xjt.getVsi();
        sprintf(buffer, "%0.0f", value);
        vspeed = value;                                  // Update global vertical speed variable
        lv_label_set_text(objects.vspdbigtext, buffer);  // Update large VSpeed text
        break;
      case 0x0100:  // Barometric Altitude from separate variosensor (alternative)
        altitude = variosensor.getAltitude();
        sprintf(buffer, "%0.0f", (altitude - altitudeoffset));
        lv_label_set_text(objects.altbigtext, buffer);  // Update large Altitude text
        break;
      case 0x0110:  // Vertical Speed from separate variosensor (alternative)
        value = variosensor.getVsi();
        sprintf(buffer, "%0.0f", value);
        vspeed = value;                                  // Update global vertical speed variable
        lv_label_set_text(objects.vspdbigtext, buffer);  // Update large VSpeed text
        break;
      case 0x0800:  // GPS Lat/Lon from separate gpssensor (alternative)
        value = gpssensor.getLat();
        sprintf(buffer, "%0.7f", value);
        lv_label_set_text(objects.latvalue, buffer);  // Update Latitude text
        value = gpssensor.getLon();
        sprintf(buffer, "%0.7f", value);
        lv_label_set_text(objects.lonvalue, buffer);  // Update Longitude text
        break;
      case 0x0820:  // GPS Altitude from separate gpssensor (alternative)
        sprintf(buffer, "%0.0f", gpssensor.getAltitude());
        lv_label_set_text(objects.altvalue, buffer);  // Update GPS Alt text
        break;
      case 0X0830:  // GPS Speed from separate gpssensor (alternative)
        sprintf(buffer, "%0.1f", gpssensor.getSpeed());
        lv_label_set_text(objects.speedvalue, buffer);  // Update GPS Speed text
        break;
      case 0x0840:  // COG from separate gpssensor (alternative)
        sprintf(buffer, "%0.1f", gpssensor.getCog());
        lv_label_set_text(objects.headvalue, buffer);  // Update Heading text
        break;
      case 0x5100:  // Number of Satellites
        value = gpssensor.getNumsat();
        if (value > 100) {  // Check for 3D fix indicator (common in some protocols)
          sprintf(buffer, "%0.0f (3d fix)", (value - 100));
        } else {
          sprintf(buffer, "%0.0f (no fix)", value);
        }
        lv_label_set_text(objects.satvalue, buffer);  // Update Satellites text
        break;
      case SENSOR_NO_DATA_ID:  // No data received from this sensor type
        break;                 // Do nothing
      default:                 // Unhandled telemetry ID
        break;                 // Do nothing
    }
  } else {
    // Process telemetry for another protocol (older FrSky)
    frsky.poll();        // Poll the FrSky data
    if (redrawScreen) {  // Check if screen update is needed (data received)
      value = rssiRx;    // Get RSSI value
      if (value != 0) {
        // Check and trigger RSSI critical/low alarms
        if (value < rssicritvalue) {
          if ((now - lastrssicritalarm) > MINREPEATTIME) {
            lastrssicritalarm = now;
            audio_event_t event = AUDIO_EVENT_PLAY_RSSICRIT;
            xQueueSend(audio_event_queue, &event, 0);
          }
        } else if (value < rssilowvalue) {
          if ((now - lastrssilowalarm) > MINREPEATTIME) {
            lastrssilowalarm = now;
            audio_event_t event = AUDIO_EVENT_PLAY_RSSILOW;
            xQueueSend(audio_event_queue, &event, 0);
          }
        }
        lastupdate = now;  // Record last update time if value is valid
      }
      sprintf(buffer, "%0.0f", value);
      lv_bar_set_value(objects.rssibar, round(value), LV_ANIM_OFF);  // Update RSSI bar
      lv_label_set_text(objects.rssibigtext, buffer);                // Update RSSI large text
      lv_label_set_text(objects.rssivalue, buffer);                  // Update RSSI value text

      value = ((float)analog1) / 21.83;  // Convert raw analog value to voltage (calibration dependent)
      sprintf(buffer, "%0.2f", value);
      lv_bar_set_value(objects.rxbatt, value, LV_ANIM_OFF);  // Update RxBatt bar
      lv_label_set_text(objects.rxbattbigtext, buffer);      // Update RxBatt text

      value = ((float)analog2) / 21.83;  // Convert raw analog value 2 to voltage
      sprintf(buffer, "%0.2f", value);
      lv_label_set_text(objects.a2bigtext, buffer);  // Update ADC2 text
      redrawScreen = 0;                              // Reset redraw flag
    }
  }

  // Check for telemetry lost timeout
  if ((now - lastupdate) > TELEMETRYLOSTTIMEOUT) {
    vspeed = 0.0;  // Set vertical speed to zero when telemetry is lost
    if (alertvisible == false) {
      lv_obj_clear_flag(objects.alertlabel, LV_OBJ_FLAG_HIDDEN);  // Show telemetry lost alert
      alertvisible = true;
      audio_event_t event = AUDIO_EVENT_PLAY_TELEMETRY_LOST;  // Trigger telemetry lost audio
      xQueueSend(audio_event_queue, &event, 0);
    }
  } else {
    // Telemetry is OK
    if (alertvisible == true) {
      lv_obj_add_flag(objects.alertlabel, LV_OBJ_FLAG_HIDDEN);  // Hide telemetry lost alert
      alertvisible = false;
      audio_event_t event = AUDIO_EVENT_PLAY_TELEMETRY_RECOVERED;  // Trigger telemetry lost audio
      xQueueSend(audio_event_queue, &event, 0);
      // Note: Telemetry recovered audio event is likely triggered elsewhere upon first valid data
    }
  }

  lv_tick_inc(now - lastTick);  // Update the LVGL tick timer
  lastTick = now;
  lv_timer_handler();  // Update the UI (LVGL)

  if (saveconfig == 1) {
    saveconfig = 0;
    saveconf();  // Save configuration if requested
  }
  if (needrestart == 1) {
    ESP.restart();  // Restart the ESP if requested
  }
}
