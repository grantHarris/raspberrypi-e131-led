// #include "WS2811Strip.h"

// WS2811Strip::WS2811Strip(YAML::Node& t_config, std::promise<void> exit_signal) : config(t_config), LEDStrip(std::move(exit_signal)) {
//     this->setup_ouput();
//     ws2811_return_t ret;
//     if ((ret = ws2811_init(&output)) != WS2811_SUCCESS){
//         BOOST_LOG_TRIVIAL(fatal) << "ws2811 init failed:" << ws2811_get_return_t_str(ret);
//         exit(1);
//     }
// }

// void WS2811Strip::setup_ouput(){
//     output.freq = TARGET_FREQ;
//     output.dmanum = DMA;
//     YAML::Node strip_channel = config["strip_channel"];
//     for(YAML::const_iterator it=strip_channel.begin(); it!=strip_channel.end(); ++it) {
//         int ch = it->first.as<int>();

//         int gpio_pin = it->second["gpionum"].as<int>();
//         int count = it->second["count"].as<int>();
//         int invert = it->second["invert"].as<int>();
//         int brightness = it->second["brightness"].as<int>();

//         output.channel[ch].gpionum = gpio_pin;
//         output.channel[ch].count = count;
//         output.channel[ch].invert = invert;
//         output.channel[ch].brightness = brightness;
//         output.channel[ch].strip_type = STRIP_TYPE;

//         BOOST_LOG_TRIVIAL(info) 
//         << "Output on Channel: " << ch 
//         << ", GPIO: " << gpio_pin
//         << ", Total LED's: " << count
//         << ", Invert: " << invert
//         << ", Brightness: " << brightness;

//     }
// }

// //void WS2811Strip::render(bool *running) {
// //    ws2811_return_t ret;
// //    
// //    while(*running == true){
// //       output_mutex.lock();
// //        if ((ret = ws2811_render(&output)) != WS2811_SUCCESS){
// //            BOOST_LOG_TRIVIAL(error) << "ws2811 render frame failed:" << ws2811_get_return_t_str(ret);
// //        }
// //        output_mutex.unlock();
// //        usleep(1000000 / 45);
// //    }
// //
// //    if(*running == false){
// //        output_mutex.lock();
// //        ws2811_fini(&output);
// //        output_mutex.unlock();
// //    }
// //
// //}

// void WS2811Strip::pixel_buffer_to_output_buffer(Pixel * pixels, int len){
//     for(int i = 0; i < len; i++){
//         this->write_to_output_buffer(0, i, pixels[i]);
//     }
// }

// void WS2811Strip::write_to_output_buffer(int strip_channel, int index, Pixel pixel){
//     output_mutex.lock();
//     output.channel[strip_channel].leds[index] = 
//     static_cast<uint32_t>(pixel.r << 16) |
//     static_cast<uint32_t>(pixel.g << 8) |
//     static_cast<uint32_t>(pixel.b);
//     output_mutex.unlock();
    
//     BOOST_LOG_TRIVIAL(trace) 
//     << "Ch: " << strip_channel 
//     << ", Index: " << index
//     << ", R: " << static_cast<int>(pixel.r)
//     << ", G: " << static_cast<int>(pixel.g)
//     << ", B: " << static_cast<int>(pixel.b);
// }
