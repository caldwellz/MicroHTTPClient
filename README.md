# MicroHTTPClient
A small HTTP client/parser intended for embedded systems, such as the ESP32 series. It has both synchronous (blocking) and async (update/callback) APIs. It is independent of any networking library, and as such it does not handle any network connections itself; it simply expects to be passed a structure of socket-like callbacks to communicate through. It can be built as a library using Meson, or everything in src/ can also just be dropped directly into your project.
