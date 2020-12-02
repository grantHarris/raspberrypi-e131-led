# raspberrypi-e131-led

RPi e1.31 bridge to WS2811 or APA102 strips

## Basic Usage
```bash
sudo ./e131bridge
```

### Config location, logging, and stats
```bash
sudo ./e131bridge --config=../config/config-sample.yml --verbosity=info --stats=true
```

## Mapping
Because a e131 universe can only support 512 bytes (channels) of data we must use multiple universes to assmeble any signficant amount of LED's. This software supports mapping from multiple e131 universes to a single strip. The mapping configuration is defined in as yml file.

### Only Used for WS2811 Strip
gpionum - Gpio pin number used as an output for WS2811 strip

```yaml
---
gpionum: 21
led_count: 680
brightness: 255
strip_type: WS2811_STRIP_GRB
mapping:
  1:
  - input:
      start_address: 1
      total_rgb_channels: 170
    output:
      start_address: 0
  2:
  - input:
      start_address: 1
      total_rgb_channels: 170
    output:
      start_address: 169
  3:
  - input:
      start_address: 1
      total_rgb_channels: 170
    output:
      start_address: 336
  4:
  - input:
      start_address: 1
      total_rgb_channels: 170
    output:
      start_address: 509

```

### Flags
* --verbosity="[log level]"
Toggle level of logging verbosity. 

* --stats=true
Toggle stats display on or off

* --config="path/to/config.yaml"
Path to config file

* --config="path/to/logfile"
Path to logging file



## Build Debug 
```bash
mkdir Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

## Build Release 
```bash
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Libraries
* https://github.com/jgarff/rpi_ws281x.git
* https://github.com/jbeder/yaml-cpp.git
* Boost 1.66 https://www.boost.org/
* http://www.airspayce.com/mikem/bcm2835/
* https://github.com/hhromic/libe131

## If you find rthis helpful
If you do something cool with this software please share it
