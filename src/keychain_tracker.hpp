#ifndef GUARD_DHTPIM_KEYCHAIN_TRACKER_HPP
#define GUARD_DHTPIM_KEYCHAIN_TRACKER_HPP
#include <thread>

#include <opendht.h>

#include "io_tools.hpp"
#include "keychain.hpp"
#include "tools.hpp"

constexpr size_t wakeup_interval = 500;

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
                verbose("Starting keychain tracking thread...");

                while(this->hold())
                {
                    // Update the keychain tracker to the next period.
                    size_t last_update = get_timestamp().count();
                    this->update();

                    // Wakes up often, but wait for the next period.
                    while(this->hold()
                        && get_timestamp().count() - last_update < period)
                    {
                        std::this_thread::sleep_for(
                            std::chrono::milliseconds(wakeup_interval));
                    }
                }

                return;
            });
    }

    // Holding getter : Is this keychain tracker holding an alive thread ?
    bool hold() const
    {
        return holding;
    }

    // Terminate the holded thread and cleanup listeners.
    void join()
    {
        if(holding)
        {
            // Cleanup tracker thread.
            holding = false;
            this->holder->join();

            // Cleanup listeners.
            for(size_t i = 0; i < pool_size; ++i)
                cancel(i);
        }
    }

    // Destructor.
    ~keychain_tracker()
    {
        join();
    }

private:

    // Update the keychain tracker to the next period.
    void update(size_t ahead_of = period)
    {
        // Setup a fresh listener for the next period.
        auto target = chain.ahead(ahead_of);
        tokens[cursor] = std::make_pair(listen(node, target, map), target);

        // CancelListen the oldest listener.
        auto next_cursor = (cursor + 1) % pool_size;
        cancel(next_cursor);

        // Update cursor.
        next_cursor = cursor;
    }

    // CancelListen the target listener.
    void cancel(size_t cursor)
    {
        if(tokens[cursor].first)
            node.cancelListen(
                dht::InfoHash(tokens[cursor].second),
                tokens[cursor].first);
    }

private:
    bool holding = false;
    std::unique_ptr<std::thread> holder;

    size_t cursor = 0;
    std::array<std::pair<size_t, std::string>, pool_size> tokens;

    dht::DhtRunner& node;
    const keychain<period>& chain;
    map_type& map;
};

#endif
