// define only one of these two. Definir solamente uno de estos dos:
//#define CASA
#define INSTI

// define or not:
//#define TEST


#ifdef CASA
  const String ssidd = "your_local_ssid";
  const String passs = "your_local_pass";
#define BROKER "192.168.0.localserver"
#endif
#ifdef INSTI
  const String ssidd = "your_production_LAN_ssid";
  const String passs = "your_production_LAN_pass";
#define BROKER "192.168.productionserver"
#endif
#ifdef TEST
const String ID = "prueba_13243-ojisjei-owieru3-2";
const String CLIENT_NAME = ID + "test";
const String CLIENT_TELEMETRY_TOPIC = ID + "/telemetry";
#define BROKER "test.mosquitto.org"
#else
const String ID = "your_unique_id";
const String CLIENT_NAME = ID + "client";
const String CLIENT_TELEMETRY_TOPIC = ID + "/telemetry";
#endif
