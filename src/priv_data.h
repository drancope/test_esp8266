// define only one of these two. Definir solamente uno de estos dos:
#define CASA
//#define INSTI

#ifdef CASA
  const String ssidd = "conlu2.4";
  const String passs = "22433299";
  const char* BROKER = "192.168.0.40";
#endif
#ifdef INSTI
  const String ssidd = "Andared";
  const String passs = "llevalatararaunvestidoblancollenodecascabeles";
  const char* BROKER = "192.168.13.169";
#endif

const String ID = "aa6ba08d-0ca2-41ec-8d4c-9e56112ff81f";
const String CLIENT_NAME = ID + "nightlight_client";
const String CLIENT_TELEMETRY_TOPIC = ID + "/telemetry";

const String TEST_ID = "prueba_13243";
const String TEST_CLIENT_NAME = TEST_ID + "test";
const String TEST_CLIENT_TELEMETRY_TOPIC = TEST_ID + "/telemetry";
