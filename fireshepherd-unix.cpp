#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>


extern int h_errno;


    /*Copyright (C) 2010 Gunnar Atli Sigurğsson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

  //Original code: www.zedwood.com


using namespace std;

#define BUFFERSIZE 1024
void die_with_error(char *errorMessage);
void die_with_wserror(char *errorMessage);
int requester();
int tracert();
char *ipaddress = "69.63.189.16"; //random facebook server.
int timeout = 400; //in ms


int main(int argc, char *argv[])
{
  cout << "Gunnar Atli Sigurdsson, 2010" << endl;
  cout << "Start with -h parameter for help" << endl;
  cout << "Note that a dot represents a packet being sent" << endl << endl;

  //DNS lookup
  hostent* lookup = gethostbyname("www.facebook.com");
  if (lookup!=NULL) {
    int i = 0;
    struct in_addr addr;
    while (lookup->h_addr_list[i] != 0) {
      addr.s_addr = *(u_long *) lookup->h_addr_list[i++];
      ipaddress = (char *)inet_ntoa(addr);
    }
  }

  //Commnand line parsing
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i],"-h")==0) {
      cout   << "Command line switches: \n\t-i *IP* \tIp Destination for packets, local router is recommended, if possible."
            << "\n\t\t\tThe local router can be found with the tracert command."
          << "\n\t-t *timeout* \tTimeout in ms for packet sending. About 200 is ok for local router, 400 otherwise."
          << "\n\t-h \t\tShows this message" << endl;
      exit(0);
    } else if (strcmp(argv[i],"-i")==0) {
      ipaddress = argv[i + 1];
      cout << "Ip address: " << ipaddress << endl;
    } else if (strcmp(argv[i],"-t")==0) {
      timeout = atoi(argv[i + 1]);
      cout << "Timeout: " << timeout << " ms" << endl;
    }
  }

  struct timespec sleeptime;
  sleeptime.tv_sec = 0;
  sleeptime.tv_nsec = timeout * 1000 * 1000;
  //Inf loop
  while(true)
  {
    requester();
    nanosleep(&sleeptime, NULL);
  }
}

int requester()
{
    string request;
    string response;
    int resp_leng;

    char buffer[BUFFERSIZE];
    struct sockaddr_in serveraddr;
    int sock;

    int port = 80;

    request+="GET /packetSniffingKillsKittens HTTP/1.1\r\n";
  request+="Host: www.facebook.com\r\n";
  request+="User-Agent: Mozilla\r\n";
  request+="Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
  request+="Accept-Language: is,en;q=0.7,en-us;q=0.3\r\n";
  request+="Accept-Encoding: gzip,deflate\r\n";
  request+="Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n";
  request+="Keep-Alive: 115\r\n";
  request+="Connection: keep-alive\r\n";
  request+="Referer: http://www.facebook.com/\r\n";
  request+="Cookie: lsd=spsse; c_user=666660000; sct=01010101; sid=0; xs=3randomhashyes666666666; asdf=??????????????!!!!!!!!!!!!!!!!!!!!%•eëÒˆY¼¥­Áøşh¡F4£A€ º¦SÍÂİåt¹Òv5şhèŸË&%%¥Ô$FsnÄ‹xÏÏvVfi6ƒÊìÈ_7Î½çÜQlXËFÿë~~½¹ùÉÛ,÷7¬ùüyóÇ>hº_ošŸ¿ÄGÜ5¼yy{ÃZÆ|øË,şÊjo¬´­W¢y¹¹y5ù|Êmk¤P“Ãt¦[%´Ô“û)7­°f²›ÎDk¹0vò€_ykW“Ûİ=ş‹ËV©&«ş×åtfç­ğÔS{/Z9Yàé½n­lãï¬ÅÇÛåô/u#8“´Ã—¯±šÿìÇúyøëO^ˆn73®¥“ĞŠ·LÄÏ1MKºGGÖ: Íìd3MCÌ§iñ_õ{[Ïs§‡0gÂë´ »°n~)ºù…áF7ÂKÙzG_O~9}ùöÆ1XÓ™4ÀwSA»Ó<Ø®ûu…ß“™×SÕ2ãŸ,¦®åÒ11ÙçN‡İ|—×ÿI·Íâœa˜ŠÃŞgtçÓ´Áeşm?å¢0Éb:K“RÛv:KÓ¯º£øìåÍïoğ¡nştÃ-Ó€@có­tÍ¦o±Íúæó³L+>… 5-  ÃÒX&bğ³lˆ[ $¯DZJ\r\n\r\n";
    request+="\r\n";

    //open socket
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        die_with_wserror("socket() failed");
 
    //connect
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family      = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(ipaddress);
    serveraddr.sin_port        = htons((unsigned short) port);
    if (connect(sock, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
        die_with_wserror("connect() failed");

    //send request
    if (send(sock, request.c_str(), request.length(), 0) != request.length())
        die_with_wserror("send() sent a different number of bytes than expected");

    //disconnect
    cout << "." << flush;
    close(sock);

    return 0;
}

void die_with_error(char *errorMessage)
{
    cerr << errorMessage << endl;
    exit(1);
}

void die_with_wserror(char *errorMessage)
{
    //cerr << errorMessage << ": " << WSAGetLastError() << endl; //no one cares.
    cerr << errorMessage << endl;
    exit(1);
}
