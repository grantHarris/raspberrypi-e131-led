#ifndef __E131_H__
#define __E131_H__
#include "yaml-cpp/yaml.h"
#include <utility>
#include <e131.h>
#include <err.h>
#include <mutex>
#include <vector>
#include <functional>
#include <future>
#include <thread>
#include <chrono>
#include <condition_variable>

#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include "LEDStrip.h"
#include "Util.h"
#include "Stoppable.h"

using namespace boost::log;
namespace logging = boost::log;

struct UniverseStats {
    unsigned int out_of_sequence;
    unsigned int updates;
};

enum State {
    GOOD,
    OUT_OF_SEQUENCE
};

class E131 : public Stoppable{
public:
    E131(YAML::Node& t_config);
    void receive_data();
    void stats_thread();
    std::condition_variable wait_for_frame;
    std::vector<Pixel> pixels;
    mutable std::mutex frame_mutex;
private:
    mutable std::mutex log_mutex;

    std::map<unsigned int, UniverseStats> universe_stats;
    std::map<unsigned int, unsigned int> sequence_numbers;
    YAML::Node config;
    
    int sockfd;
    e131_packet_t packet;
    e131_error_t error;
    uint8_t last_seq = 0x00;

    void map_to_buffer(e131_packet_t &packet);
    void register_universe_for_stats(unsigned int t_universe);
    void join_universe(int t_universe);
    void log_universe_packet(unsigned int t_universe, State state);
};

#endif /* __E131_H__ */
