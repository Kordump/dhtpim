#ifndef GUARD_DHTPIM_KEYCHAIN_TRACKER_HPP
#define GUARD_DHTPIM_KEYCHAIN_TRACKER_HPP
#include <thread>

#include <opendht.h>

#include "io_tools.hpp"
#include "keychain.hpp"
#include "tools.hpp"

// Follow and listen a moving keychain.
template<size_t pool_size, size_t period = 60000>
struct keychain_tracker
{
    keychain_tracker(
        dht::DhtRunner& node,
        const keychain<period>& chain,
        map_type& map)
    : node{node}, chain{chain}, map{map}
    {
        // Reset listener pool.
        for(size_t i = 0; i < pool_size; ++i)
            tokens[i].first = 0;

        // Start listenning the current period.
        update(false);

        // Start a thread which will track each period.
        holding = true;
        this->holder = std::make_unique<std::thread>(
            [this]()
            {
                disp("Starting keychain tracking thread...");
                while(this->hold())
                {
                    this->update();
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(period));
                }

                return;
            });
    }

    bool hold() const
    {
        return holding;
    }

    void join()
    {
        if(this->holding)
        {
            this->holding = false;
            this->holder->join();
        }
    }

    ~keychain_tracker()
    {
        join();
    }

private:
    void update(size_t ahead_of = period)
    {
        auto target = chain.ahead(ahead_of);
        tokens[cursor] = std::make_pair(listen(node, target, map), target);

        auto next_cursor = (cursor + 1) % pool_size;
        if(tokens[next_cursor].first)
            node.cancelListen(
                dht::InfoHash(tokens[next_cursor].second),
                tokens[next_cursor].first);

        next_cursor = cursor;
    }

    bool holding = false;
    std::unique_ptr<std::thread> holder;

    size_t cursor = 0;
    std::array<std::pair<size_t, std::string>, pool_size> tokens;

    dht::DhtRunner& node;
    const keychain<period>& chain;
    map_type& map;
};

#endif
