#include "APA102Strip.h"


Apa102Strip::Apa102Strip(){
    //Initiate the SPI Data Frame
    if (!bcm2835_init()){
      BOOST_LOG_TRIVIAL(error) << "bcm2835_init failed";
      throw std::string("bcm2835_init failed");
    }

    if (!bcm2835_spi_begin()){
      BOOST_LOG_TRIVIAL(error) << "bcm2835_spi_begin failed";
      throw std::string("bcm2835_init failed");
    }

    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    /*
        
    BCM2835_SPI_MODE0       CPOL = 0, CPHA = 0
    BCM2835_SPI_MODE1       CPOL = 0, CPHA = 1
    BCM2835_SPI_MODE2       CPOL = 1, CPHA = 0
    BCM2835_SPI_MODE3       CPOL = 1, CPHA = 1  
    */
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128); // The default
    /*

        BCM2835_SPI_CLOCK_DIVIDER_65536     65536 = 262.144us = 3.814697260kHz
        BCM2835_SPI_CLOCK_DIVIDER_32768     32768 = 131.072us = 7.629394531kHz
        BCM2835_SPI_CLOCK_DIVIDER_16384     16384 = 65.536us = 15.25878906kHz
        BCM2835_SPI_CLOCK_DIVIDER_8192      8192 = 32.768us = 30/51757813kHz
        BCM2835_SPI_CLOCK_DIVIDER_4096      4096 = 16.384us = 61.03515625kHz
        BCM2835_SPI_CLOCK_DIVIDER_2048      2048 = 8.192us = 122.0703125kHz
        BCM2835_SPI_CLOCK_DIVIDER_1024      1024 = 4.096us = 244.140625kHz
        BCM2835_SPI_CLOCK_DIVIDER_512       512 = 2.048us = 488.28125kHz
        BCM2835_SPI_CLOCK_DIVIDER_256       256 = 1.024us = 976.5625kHz
        BCM2835_SPI_CLOCK_DIVIDER_128       128 = 512ns = = 1.953125MHz
        BCM2835_SPI_CLOCK_DIVIDER_64        64 = 256ns = 3.90625MHz
        BCM2835_SPI_CLOCK_DIVIDER_32        32 = 128ns = 7.8125MHz
        BCM2835_SPI_CLOCK_DIVIDER_16        16 = 64ns = 15.625MHz
        BCM2835_SPI_CLOCK_DIVIDER_8         8 = 32ns = 31.25MHz
        BCM2835_SPI_CLOCK_DIVIDER_4         4 = 16ns = 62.5MHz
        BCM2835_SPI_CLOCK_DIVIDER_2         2 = 8ns = 125MHz, fastest you can get   
    
    */
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
}

/**
 * @brief Set 5 bit global brightness value for strip
 * 
 * @param t_brightness 5 bit brightness value
 */
void Apa102Strip::set_brightness(uint8_t t_brightness = 31){
  brightness = t_brightness;
}

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param t_brightness [description]
 */
void Apa102Strip::write_pixels_to_strip(std::vector<Pixel>& t_pixels){
	Apa102Strip::write(t_pixels, brightness);
}

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param t_brightness [description]
 */
void Apa102Strip::write(std::vector<Pixel>& t_pixels, uint8_t t_brightness){
  BOOST_LOG_TRIVIAL(debug) << "send apa102 frame";
  Apa102Strip::send_start_frame();
  for(uint16_t i = 0; i < t_pixels.size(); i++){
       Apa102Strip::send_color(t_pixels[i], t_brightness);
  }
  Apa102Strip::send_end_frame(t_pixels.size());
}

/**
 * @brief Send four 0x00 bytes for APA102 start frame
 * 
 */
void Apa102Strip::send_start_frame(){
  char ledDataBlock[4] = {0};
  bcm2835_spi_writenb(ledDataBlock, 4);
}

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param count [description]
 */
void Apa102Strip::send_end_frame(uint16_t count){
  bcm2835_spi_transfer(0xFF);
  for (uint16_t i = 0; i < 5 + count / 16; i++){
    bcm2835_spi_transfer(0);
  }
}

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param red
 * @param green
 * @param blue
 * @param t_brightness [description]
 */
void Apa102Strip::send_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness){
  bcm2835_spi_transfer(0b11100000 | brightness);
  bcm2835_spi_transfer(blue);
  bcm2835_spi_transfer(green);
  bcm2835_spi_transfer(red);
}

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param t_pixel
 * @param t_brightness [description]
 */
void Apa102Strip::send_color(Pixel t_pixel, uint8_t t_brightness){
  Apa102Strip::send_color(t_pixel.r, t_pixel.g, t_pixel.b, t_brightness);
}

Apa102Strip::~Apa102Strip(){
    bcm2835_spi_end();
    bcm2835_close();
}
