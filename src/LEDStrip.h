#ifndef __LEDSTRIP_H__
#define __LEDSTRIP_H__
#include <queue>
#include <vector>
#include <utility>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include <condition_variable>
#include <thread>
#include <mutex>
#include <future>

#include "yaml-cpp/yaml.h"

#include "Stoppable.h"

using namespace boost::log;
namespace logging = boost::log;

struct Pixel {
     uint32_t r;
     uint32_t g;
     uint32_t b;
 };

 class LEDStrip : public Stoppable {
    protected:
        std::queue<std::vector<Pixel>> frame_queue;
    public:
        mutable std::mutex frame_mutex;
        std::condition_variable wait_for_frame;

        /**
         * @brief Abstract class to be implemented for writing to the strip
         * @param t_pixels Vector with pixel data to write to strip
         */
        virtual void write_pixels_to_strip(std::vector<Pixel>& t_pixels) = 0;
        
        /**
         * @brief Display a new frame on the strip
         * 
         * @param t_pixels Vector of pixels
         */
        void push_frame(std::vector<Pixel> const& t_pixels){
            std::lock_guard<std::mutex> lock(frame_mutex); //replace with c++11
            frame_queue.push(t_pixels);
            wait_for_frame.notify_one();
        }

        /**
         * @brief [brief description]
         * @details [long description]
         * 
         * @param lock [description]
         */
        void pop_and_display_frame(){
            std::unique_lock<std::mutex> lock(frame_mutex); //replace with c++11
            BOOST_LOG_TRIVIAL(info) << "Wait and pop";
            while(stop_requested() == false)
            {
                while(frame_queue.empty())
                {
                    if(stop_requested() == false){
                        wait_for_frame.wait(lock);
                    }else{
                        break;
                    }
                }
                this->write_pixels_to_strip(frame_queue.front());
                frame_queue.pop();
            }
        }
 };

 #endif /* __LEDSTRIP_H__ */

