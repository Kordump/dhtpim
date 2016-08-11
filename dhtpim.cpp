#include <thread>
#include <chrono>
#include <iostream>
#include <unordered_map>

#include <opendht.h>

#include "cli_tools.hpp"
#include "keychain.hpp"
#include "tools.hpp"
#include "msg.hpp"

template<size_t pool_size, size_t period> struct keychain_tracker;

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

    ~keychain_tracker()
    {
        if(this->holding)
        {
            this->holding = false;
            this->holder->join();
        }
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

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        std::cout
            << "Usage : "                                       "\n"
            << " - " << argv[0] << " <host> <port>"             "\n"
            << "   - <host>: Host of the bootstrap node."       "\n"
            << "   - <port>: Port of the bootstrap node."       "\n"
            << " - " << argv[0] << " bootstrap.ring.cx 4222"    "\n"
            << std::endl;
        return 1;
    }

    // Use GNU readline history.
    using_history();

    // Create an unordered_map, for messages deduplication.
    map_type map;

    // Use a opendht node.
    dht::DhtRunner node;

    // Create a brand new identity.
    auto node_identity = dht::crypto::generateIdentity();

    // Run a node with a free port (0), use a separate thread.
    node.run(0, node_identity, true);

    // Join the network through an already known node.
    node.bootstrap(argv[1], argv[2]);

    // User settings.
    std::string username = input("Username : ");
    keychain<> chain(input("Channel : "), input("Password : "));
    std::cout << "\n";

    // Pack&Put data on the DHT.
    node.putSigned(chain, msg(username, "Enter the chatroom."));

    // Watch for incoming new messages.
    keychain_tracker<5> tracker(node, chain, map);

    // Read stdin and put messages on the DHT.
    for(;;)
    {
        std::string output = input("");
        std::cout << "\x1b[A\x1b[2K\r";

        if(!output.empty())
        {
            // Insert the message in the DHT.
            auto content = msg(username, output);
            node.putSigned(chain, content);

            // Register-it locally and instant display.
            auto stamp = get_timestamp();
            map.insert(std::make_pair(msg(content), stamp));
            disp(content);
        }
    }

    // Wait threads
    node.join();
    return 0;
}
