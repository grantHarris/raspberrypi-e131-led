#ifndef __APA102_H__
#define __APA102_H__
#include <bcm2835.h>
#include <stdio.h>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include <future>
#include "LEDStrip.h"

using namespace boost::log;
namespace logging = boost::log;

class Apa102Strip : public LEDStrip {
public:
 	Apa102Strip();
	~Apa102Strip();
 	void set_brightness(uint8_t brightness);
 	void write_pixels_to_strip(std::vector<Pixel> &t_pixels);
 	static void write(std::vector<Pixel> &t_pixels, uint8_t t_brightness);
	static void send_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t t_brightness = 31);
 	static void send_color(Pixel t_pixel, uint8_t t_brightness = 31);
 private:
 	static void send_start_frame();
 	static void send_end_frame(uint16_t count);
 	uint8_t brightness;
 	uint16_t count;
 };

 #endif /* __APA102_H__ */
