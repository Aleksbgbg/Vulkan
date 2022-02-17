# Networking (1.0)
This is an archived branch demonstrating the first working multiplayer implementation of the game where players can join the same server and fly together.

Networking code uses platform-specific UDP sockets (winsock) and is therefore Windows-only.
No client-side prediction is implemented and the server updates all clients whenever a single client sends a message.

Regardless of the fact that the implementation is very naive and unoptimised, this first networking demonstration works quite well.
