#include <mbed.h>
#include <mbedtls/base64.h>
/* exposes ETH object */
#include <easy-connect.h>

/* global variables from easy-connect. */
#include "WIZnetInterface.h"
#include "DHCPClient.h"
extern WIZnetInterface eth;
extern DHCPClient dhcp;

#include <TCPSocket.h>

int main() {

	// put your setup code here, to run once:
	uint8_t dest[20];
	const char* src = "Test";

	size_t outputLen = 0;
	mbedtls_base64_encode(dest, sizeof(dest), &outputLen, (const uint8_t*) src,
			strlen(src));

	printf("Base64 encoding of \"%s\": %s\n", src, (const char*) dest);

	//connect to ethernet network via DHCP, print messages.
	//iface = eth.
	NetworkInterface* iface = easy_connect(true);
	//work with uniform iface object.. (or global eth object for extended functionality)

	//might be needed for others
	//iface->set_as_default();

	//do stuff.
	TCPSocket sock;
	sock.open(&eth);
	sock.connect("httpbin.org", 80);
	const char* request_str = "GET /status/200 HTTP/1.0\r\n\r\n";
	sock.send(request_str, strlen(request_str));
	char buf[256] = {0};
	nsapi_size_or_error_t err = sock.recv(buf, sizeof(buf));
	if(err > 0) {
		printf("Received %d bytes: %s\n", (int) err, buf);
	} else {
		printf("Receive error: %d\n", (int) err);
	}
	sock.close();

	while (1) {
		// put your main code here, to run repeatedly:
	}
}
