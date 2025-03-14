# Source queries

It's a C++ client for querying the Valve's GoldSource engine games.

Tested on team fortress.

Using dll on windows.
The library is **Windows** only and uses the **winsock2** APIs for the UDP connection to the server.

## About

In project 2 classes and 3 function.
You can create custom setting

Class Request have 3 method and return char* data;
Class Parser can convert:
  A2S_INFO, A2S_RULES, A2S_PlAYER to strcuct 
  infoServer, playerServer, rulesServer 

Also 3 method for output data to console

## Usage

### use fuction
```cpp
quer::getAll(port, ip);
```


### Use the client to query the server:

* Query server info
```cpp
quer::infoServer infServ{ 0 };

quer::RequestValve reqval(port, ipserver);
quer::ParserMessage parser;

infServ = parser.parseInfoServer(reqval.A2S_INFO());
  
quer::ShowInfo(infServ);
```

also another 2 type



