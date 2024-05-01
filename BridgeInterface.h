#ifndef BRIDGEINTERFACE_H
#define BRIDGEINTERFACE_H

#include <string>
enum class LogLevel; // Forward declaration of the LogLevel enum

class BridgeInterface {
public:
    BridgeInterface(const std::string& bridgeName);
    ~BridgeInterface();

    bool createBridge();
    bool deleteBridge();
    bool addInterfaceToBridge(const std::string& interfaceName);
    bool removeInterfaceFromBridge(const std::string& interfaceName);

private:
    std::string bridgeName_;
    int sockfd_;
};

#endif // BRIDGEINTERFACE_H

