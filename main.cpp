#include "BridgeInterface.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <cstring>

int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <bridge_name> <interface_name>" << std::endl;
        return 1;
    }
    Logger mainlogger;
    mainlogger.setLogLevel(LogLevel::LOG_INFO);
 // std::string bridgeName = "br0";
    std::string bridgeName = argv[1];
 // std::string interfaceName = "veth0";
    std::string interfaceName = argv[2];

    BridgeInterface bridge(bridgeName);

    // Call createBridge to create the bridge if it doesn't exist
    if (!bridge.createBridge()) {
        std::cerr << "Error creating bridge: " << bridgeName << std::endl;
        return 1;
    }

    if (!bridge.addInterfaceToBridge(interfaceName)) {
        Logger::log(LogLevel::LOG_ERROR, "[%s:%d] Failed to add interface: %s", __func__, __LINE__, strerror(errno));
        return 1;
    }

    
    return 0;
}