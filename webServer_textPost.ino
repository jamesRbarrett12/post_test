/*
 * An edit of the code provided by Hohn Harrison:
 * http://stackoverflow.com/questions/14944773/receiving-a-http-post-request-on-arduino
 * A simple Arduino Ethernet webserver html form, which when
 * entered, creates a POST request.
 * The arduino processes the form data and prints the text string to
 * the serial port.
 *
 * Created by James Barrett
 * Date: 1st Mar 2017
 */

#include <SPI.h>
#include <Ethernet.h>

// You can change the MAC and IP addresses to suit your network:

byte mac[] = { 0X52, 0X64, 0X75, 0X69, 0X6E, 0X6F };
IPAddress ip( 192, 168, 15, 134 );

EthernetServer server(80); // Port 80 is HTTP port
char new_state[1024];

void setup()
{
  Serial.begin(9600);
  // Start the Ethernet server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Serving on http://");
  Serial.println(Ethernet.localIP());
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // Serial.println("Client connected");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      int i = 0;
      int head = 1;
      int body = 0;
      while(client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          if ( i <= 2 ) {
            // an http request ends with a blank line
            sendPage(client);
            if ( head == 1 ) {
              body = 1;
              head = 0;
            }
          }
          i = -1;
        }
        if ( body == 1 ) {
          new_state[i] = c;
        }
        i++;
        new_state[i] = '\0';
      }
      i = 0;
    }
      if ( strlen(new_state) > 0 ){
      Serial.print("Form data: ");
      Serial.print ("[");
      Serial.print(new_state);
      Serial.println ("]");
      // get string
      Serial.print("Trimmed data: ");
      Serial.println(new_state+10);
      String str = new_state+10;
      str.replace('+', ' ');
      Serial.print("Resutling text: ");
      Serial.println(str);
      //str = "";
      Serial.println();
    }
  }
}

void sendPage(EthernetClient client)
{
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1> Hello James... </h1>");
  client.println("<form action=\"\" method=\"post\">");
  // text input
  client.println("<input type='text' name='firstname' value='enter here'/>");
  client.println("</form>\n</body>\n</html>");
  client.stop();
  Serial.println("Disconnected");
}
