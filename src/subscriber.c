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
}
