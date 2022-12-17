#include <stdio.h>
#include <stdlib.h>

//chosen broker
#include <mosquitto.h>

//test to check

int main() {
    int rc, id = 12;

    mosquitto_lib_init();

    struct mosquitto *mosq;

    mosq = mosquitto_new("subscribe-test", true, &id);

   rc = mosquitto_connect(mosq, "test.mosquitto.org", 1883, 10);

   if (rc) {
    printf("Unable to connect to broker woth %d\n", rc);
    return -1;
   }

   mosquitto_loop_start(mosq);
   printf("Press Enter to quit\n");
   getchar();
   mosquitto_loop_end(mosq, true);

   mosquitto_disconnect(mosq);
   mosquitto_destroy(mosq);

   mosquitto_lib_cleanup();

   return EXIT_SUCCESS;
}
