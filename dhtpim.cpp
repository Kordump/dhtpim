#include "keychain_tracker.hpp"
#include "io_tools.hpp"
#include "keychain.hpp"
#include "tools.hpp"
#include "msg.hpp"
#include "opt.hpp"

int main(int argc, char** argv)
{
    // Parse args.
    opt cmd(argc, argv);

    // Init io-related stuff.
    io_init();

    // Create an unordered_map, for messages deduplication.
    map_type map;

    // Use a opendht node.
    dht::DhtRunner node;

    // Create a brand new identity.
    auto node_identity = dht::crypto::generateIdentity();

    // Run a node with a free port (0), use a separate thread.
    node.run(opt::args().port_arg, node_identity, true);

    // Join the network through an already known node.
    node.bootstrap(
        opt::args().bootstrap_host_arg,
        std::to_string(opt::args().bootstrap_port_arg).c_str());

    // User settings.
    std::string username = opt::args().username_arg;
    std::string chainame = opt::args().keychain_arg;
    std::string password = "";

    // Prompt password if none given in interactive mode
    if(opt::args().password_given)
        password = opt::args().password_arg;
    else
        if(opt::args().interactive_flag)
            password = input("Password : ");
        else
            verbose("Warning: No password provided\n"
                "(Use interactive mode to prompt password input)");

    // Setup keychain
    keychain<> chain(chainame, password);

    // Pack&Put data on the DHT.
    node.putSigned(chain, msg(username, "Enter the chatroom."));

    // Watch for incoming new messages.
    keychain_tracker<5> tracker(node, chain, map);

    // Read stdin and put messages on the DHT.
    for(;;)
    {
        std::string output = input();

        if(output.empty())
            continue;
        if(output.substr(0, 6) == "/quit")
            break;
        if(output[0] == '/')
            continue;

        // Insert the message in the DHT.
        auto content = msg(username, output);
        node.putSigned(chain, content);

        // Register-it locally and instant display.
        auto stamp = get_timestamp();
        map.insert(std::make_pair(msg(content), stamp));
        disp(content);
    }

    // Wait threads
    verbose("Stopping...");
    tracker.join();
    node.join();

    // Bye !
    verbose("Bye !");
    return 0;
}
