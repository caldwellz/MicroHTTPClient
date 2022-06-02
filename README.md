# MicroHTTPClient
A small synchronous self-contained HTTP client intended for embedded systems, such as the ESP32 family.

It does NOT use the C standard library or any other library, and as such it does not do dynamic memory allocation, handle network connections, or parse the response body itself; it simply expects to be passed a context structure containing socket-like callbacks to communicate through. See the tests/ folder for examples of using SDL_net and JSON / CBOR parsers to fill in the gaps.

It can be built as a library using Meson, or everything in src/ and include/ can also just be dropped directly into your project without further configuration. In either case, however, make sure to check the application-specific section of MHC_config.h and adjust the values to your use-case (particularly the static buffer sizes) as needed. There are checks in place to prevent overruns, but since requests are fully synchronous (not streaming) at the moment, they may fail if the buffers are not large enough. Streaming modes might be added in the future.
