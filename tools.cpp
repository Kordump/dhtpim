#include "tools.hpp"

std::chrono::milliseconds get_timestamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
}

size_t listen(dht::DhtRunner& node, std::string chain, map_type& map)
{
    std::future<size_t> token = node.listen(chain,
        [&map](const std::vector<std::shared_ptr<dht::Value>>& values)
        {
            // For every value found...
            for (const auto& value : values)
            {
                // Unpack then register and display it, if it's a new value.
                std::string content = value->unpack<std::string>();
                if(!map.count(content))
                {
                    map.insert(std::make_pair(content, get_timestamp()));
                    disp(content);
                }
            }

            // Continue lookup until no values are left.
            return true;
        });

    if(token.wait_for(std::chrono::seconds(1)) != std::future_status::ready)
    {
        verbose("Warning: Could not create a listener since 1000ms.");
        verbose("         Trying again for 30s...");

        if(token.wait_for(std::chrono::seconds(30))
            != std::future_status::ready)
            verbose("Error: Failure.");
        else
            verbose("         Done.");
    }

    auto v = token.get();
    verbose("Starting listening to "
        + chain
        + " with token "
        + std::to_string(v) + ".");
    return v;
}
