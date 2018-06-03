# YiLiShen

Project YiLiShen (Chinese: 蚁力神), a simple helper tool for Xiaomi Yeelight WiFi light bulbs/lamps/strips.

The name was taken from a famous Chinese Ponzi scheme scandal in a decade ago. Check [Yilishen Tianxi Group](https://en.wikipedia.org/wiki/Yilishen_Tianxi_Group) for more details.


## Development environment

- Ubuntu 18.04
- macOS 10.13.5

You may need to install rapidjson separately.

On macOS you should run: 

```
brew install rapidjson
```

On Ubuntu you should run

```
sudo apt install rapidjson-dev
```

I've only got two mono color light bulbs, so I can't test those color setting commands. But it should works.

## Sample Usage

Usage:

```
Yilishen <command>  [arg1] [arg2] [arg3] ... [argN]
```

Please refer to [Yeelight WiFi Light Inter-Operation Specification](https://www.yeelight.com/download/Yeelight_Inter-Operation_Spec.pdf) for more details

Output when running `./Yilishen toggle`:

```
Found a device at 192.168.2.122, ID: 3330
Sending command toggle to this device...
Got result: {"id":9527, "result":["ok"]}



Found a device at 192.168.2.145, ID: 3331
Sending command toggle to this device...
Got result: {"id":9527, "result":["ok"]}
```

...and the light bulb will be turn on (if not on) or turn off (if it has been turned on).