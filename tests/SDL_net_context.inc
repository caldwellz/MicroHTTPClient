#include "MHC_context.h"
#include <SDL2/SDL_net.h>

socket* SDLNet_connect (const char* host, u16_t port) {
  IPaddress addr;
  if (SDLNet_ResolveHost(&addr, host, port) != 0)
    return NULL;

  TCPsocket sock = SDLNet_TCP_Open(&addr);
  return (socket*) sock;
}

void SDLNet_disconnect (socket* sock) {
  SDLNet_TCP_Close((TCPsocket) sock);
}

u16_t SDLNet_send (socket* sock, const byte_t* data, length_t len) {
  return SDLNet_TCP_Send((TCPsocket) sock, (void *) data, len);
}

u16_t SDLNet_recv (socket* sock, const byte_t* data, length_t maxLen) {
  int actualLen = SDLNet_TCP_Recv((TCPsocket) sock, (void *) data, maxLen);
  if (actualLen < 0)
    return 0;

  return (u16_t) actualLen;
}

static MHC_context ctx = {
  NULL,
  SDLNet_connect,
  SDLNet_disconnect,
  SDLNet_send,
  SDLNet_recv
};
