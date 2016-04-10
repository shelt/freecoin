# Processes

Currently, this repository contains a library several different binaries, each with a different responsibility.

## freecoind
Maintains a connection to the p2p network.

### MAIN/LOGIC THREAD
* Spawns the server thread.
* Dequeues messages from the `receive` queue.
  * If this process were described as a cycle, it would be something like this:
  * Dequeued messages are passed to the appropriate (ctype-specific) `handler` function. The handler function will process the data, and if the data requires a response, the response will be aggregated by a `builder` function, which returns a raw message. This message is then enqueued into the appropriate peer(s') `send` queue.
* Enqueues messages into peers' respective `send` queues.

### SERVER THREAD
* Handles incoming connections.
* Spawns new socket threads for new connections.

### SOCKET THREAD
* Each peer has one of these.
* Handles incoming data and pushes it to the `recieved` queue

#### TODO: Other processes; these processes connect to freecoind via IPC.
