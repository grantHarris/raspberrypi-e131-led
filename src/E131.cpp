#include "E131.h"

E131::E131(YAML::Node& t_config) : config(t_config){
    pixels.resize(t_config["led_count"].as<int>());

    do {
        sockfd = e131_socket();
        if(sockfd < 0){
            BOOST_LOG_TRIVIAL(error) << "E1.31 socket creation failed.";
            usleep(1000000);//get rid of this weird loop crap
        }
    }

    while (sockfd < 0);

    BOOST_LOG_TRIVIAL(info) << "E1.31 socket created.";

    while (e131_bind(sockfd, E131_DEFAULT_PORT) < 0){
        BOOST_LOG_TRIVIAL(error) << "E1.31 failed binding to port: " << E131_DEFAULT_PORT;
        usleep(1000000); //get rid of this weird loop crap       
    }

    BOOST_LOG_TRIVIAL(info) << "E1.31 client bound to port: " << E131_DEFAULT_PORT;

    for(YAML::const_iterator it=config["mapping"].begin(); it != config["mapping"].end(); ++it) {
        int universe = it->first.as<int>();
        this->join_universe(universe);
        sequence_numbers[universe] = 0;
        this->register_universe_for_stats(universe);
    }

}

/**
 * @brief Execute shell commands to properly route the universes
 * 
 * @param t_universe universe number
 */
void E131::join_universe(int t_universe){
    BOOST_LOG_TRIVIAL(info) << "Joining universe: " << t_universe;

    std::stringstream ss;
    ss << "sudo ip maddr add 239.255.0." << t_universe << " dev wlan0";
    
    BOOST_LOG_TRIVIAL(debug) << "Executing shell command: " << ss.str();
    Util::exec(ss.str().c_str());

    if (e131_multicast_join(sockfd, t_universe) < 0)
        err(EXIT_FAILURE, "E1.31 Multicast join failed for universe ");   
}

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param running [description]
 */
void E131::receive_data(){
    BOOST_LOG_TRIVIAL(info) << "E.131 receive data starting";
    while(stop_requested() == false) {
        if (e131_recv(sockfd, &packet) < 0)
            err(EXIT_FAILURE, "E1.31 receive failed");

        if ((error = e131_pkt_validate(&packet)) != E131_ERR_NONE) {
            BOOST_LOG_TRIVIAL(error) << "E1.31 packet validation error" << e131_strerror(error);
            continue;
        }
        
        int universe = ntohs(packet.frame.universe);

        if (e131_pkt_discard(&packet, sequence_numbers[universe])) {
            BOOST_LOG_TRIVIAL(warning) << "E1.31 packet received out of sequence. Last: " 
            << static_cast<int>(sequence_numbers[universe] ) << ", Seq: " << static_cast<int>(packet.frame.seq_number);
            this->log_universe_packet(universe, State::OUT_OF_SEQUENCE);
            sequence_numbers[universe] = packet.frame.seq_number;
            continue;
        }

        this->log_universe_packet(universe, State::GOOD);
        BOOST_LOG_TRIVIAL(debug) << "Packet for universe: " << universe;
        this->map_to_buffer(packet);

        sequence_numbers[universe] = packet.frame.seq_number;
    }
}

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param packet [description]
 */
void E131::map_to_buffer(e131_packet_t &packet){
    int start_address, end_address, output_address_start;
    YAML::Node universe_config = config["mapping"][ntohs(packet.frame.universe)];

    for(YAML::const_iterator it = universe_config.begin(); it != universe_config.end(); ++it) {
        const YAML::Node& entry = *it;
        
        // Input, from universe packet
        YAML::Node input_params = entry["input"];
        // Ther start index where the mapping should begin
        start_address = std::max(1, std::min(input_params["start_address"].as<int>(), 511));
        //The last address we are mapping
        end_address = std::max(1, std::min(std::max(0, std::min(input_params["total_rgb_channels"].as<int>() - 1, 511)) - start_address, 511));

        //Output to strip
        YAML::Node output_params = entry["output"];
        output_address_start = output_params["start_address"].as<int>();
        
        for(int i = start_address - 1; i < end_address; i++){
            int index = i * 3 + 1;
            Pixel pixel;
            pixel.r = packet.dmp.prop_val[index];
            pixel.g = packet.dmp.prop_val[index + 1];
            pixel.b = packet.dmp.prop_val[index + 2];
            //put a lock here
            pixels[output_address_start + i] = pixel;
            //end lock
        }
    }
    wait_for_frame.notify_all();
}

/**
 * @brief Register a s particular universe for stats collection
 * @param int Universe id
 */
void E131::register_universe_for_stats(unsigned int t_universe){
    UniverseStats universe {0, 0};
    universe_stats[t_universe] = universe;
}

/**
 * @brief Log packet as good or out of sequence (will be discarded)
 *
 * @param int universe number
 * @param state state of packet
 */
void E131::log_universe_packet(unsigned int t_universe, State state){
    log_mutex.lock();
    switch(state){
        case State::GOOD:
            universe_stats[t_universe].updates++;
        break;
        case State::OUT_OF_SEQUENCE:
            universe_stats[t_universe].out_of_sequence++;
        break;
    }
    log_mutex.unlock();
}

/**
 * @brief Log stats every second to the console
 */
void E131::stats_thread(){
    while (stop_requested() == false){
        log_mutex.lock();
        for(std::map<unsigned int, UniverseStats>::iterator iter = universe_stats.begin(); iter != universe_stats.end(); ++iter)
        {
            unsigned int universe_number =  iter->first;
            UniverseStats stats = iter->second;
            BOOST_LOG_TRIVIAL(info) << "Universe: " << universe_number << ", Good: " << stats.updates << "FPS, Dropped: " << stats.out_of_sequence << "FPS";
            UniverseStats universe {0, 0};
            iter->second = universe;
        }

        log_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
