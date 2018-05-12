// #ifndef __WS2811STRIP_H__
// #define __WS2811STRIP_H__

// #define DMA             10
// #define TARGET_FREQ     WS2811_TARGET_FREQ
// #define STRIP_TYPE      WS2811_STRIP_GRB

// #include <stdint.h>
// #include <unistd.h>
// #include <mutex>
// #include <err.h>
// #include <boost/log/trivial.hpp>
// #include <boost/log/core.hpp>
// #include <boost/log/expressions.hpp>
// #include "LEDStrip.h"
// #include "../lib/ws2811.h"
// #include "../lib/clk.h"
// #include "../lib/gpio.h"
// #include "../lib/dma.h"
// #include "../lib/pwm.h"
// #include "../lib/version.h"
// #include "yaml-cpp/yaml.h"
// #include <future>

// using namespace boost::log;
// namespace logging = boost::log;

// class WS2811Strip : public LEDStrip {
//     public:
//     	WS2811Strip(YAML::Node& t_config, std::promise<void> exit_signal);
//     	void write_to_output_buffer(int strip_channel, int index, Pixel pixel);
//         void pixel_buffer_to_output_buffer(Pixel * pixels, int len);
//     private:
//         ws2811_t output;
//         YAML::Node config;
//         std::mutex output_mutex;
//         void setup_ouput();
// };

// #endif /* __WS2811STRIP_H__ */
