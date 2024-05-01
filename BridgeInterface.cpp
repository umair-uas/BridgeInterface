#include <iostream>
#include <cstring>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <unistd.h>

#include "BridgeInterface.h"
#include "Logger.h"
#include "BridgeIoctlConstants.h" // Include the custom header with the bridge-related IOCTL constants

BridgeInterface::BridgeInterface(const std::string& bridgeName) : bridgeName_(bridgeName), sockfd_(-1) {
    sockfd_ = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd_ == -1) {
        Logger::log(LogLevel::LOG_ERROR,"socket");
    }
}

BridgeInterface::~BridgeInterface() {
    if (sockfd_ != -1) {
        close(sockfd_);
    }
}

bool BridgeInterface::createBridge() {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, bridgeName_.c_str(), IFNAMSIZ);

  if (ioctl(sockfd_, SIOCGIFFLAGS, &ifr) != -1) {
        Logger::log(LogLevel::LOG_INFO, "[%s:%d] Bridge '%s' already exists.", __func__, __LINE__, bridgeName_.c_str());
        return true;
    }

    if (ioctl(sockfd_, SIOCBRADDBR, &ifr) == -1) {
        Logger::log(LogLevel::LOG_ERROR, "[%s:%d] Failed to create bridge: %s", __func__, __LINE__, strerror(errno));
        return false;
    }

    Logger::log(LogLevel::LOG_INFO, "[%s:%d] Bridge '%s' created successfully.", __func__, __LINE__, bridgeName_.c_str());

    return true; 
}



bool BridgeInterface::deleteBridge() {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, bridgeName_.c_str(), IFNAMSIZ);

    if (ioctl(sockfd_, SIOCBRDELBR, &ifr) == -1) {
       Logger::log(LogLevel::LOG_ERROR, "[%s:%d]  %s", __func__, __LINE__, strerror(errno));
        return false;
    }

    return true;
}

bool BridgeInterface::addInterfaceToBridge(const std::string& interfaceName) {
    struct ifreq ifr;
    strncpy(ifr.ifr_name, interfaceName.c_str(), IFNAMSIZ);

    // Get the interface index using SIOCGIFINDEX ioctl
    int ifidx = if_nametoindex(interfaceName.c_str());
    if (ifidx == 0) {
        Logger::log(LogLevel::LOG_ERROR, "[%s:%d] Interface '%s' does not exist!", __func__, __LINE__, interfaceName.c_str());
        return false;
    }

    int fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (fd == -1) {
        Logger::log(LogLevel::LOG_ERROR, "[%s:%d] Failed to open socket: %s", __func__, __LINE__, strerror(errno));
        return false;
    }

    strncpy(ifr.ifr_name, bridgeName_.c_str(), IFNAMSIZ);
    ifr.ifr_ifindex = ifidx;

   if (ioctl(fd, SIOCBRADDIF, &ifr) == -1) {
        if (errno == EBUSY) {
            Logger::log(LogLevel::LOG_INFO, "[%s:%d] Interface '%s' is already attached to another bridge", __func__, __LINE__,
                        interfaceName.c_str());
        } else {
            Logger::log(LogLevel::LOG_ERROR, "[%s:%d] Failed to add interface '%s' to bridge '%s': %s", __func__, __LINE__,
                        interfaceName.c_str(), bridgeName_.c_str(), strerror(errno));
        }
        close(fd);
        return false;
    } 

    Logger::log(LogLevel::LOG_INFO, "[%s:%d] Interface '%s' added to bridge '%s' successfully.", __func__, __LINE__,
                interfaceName.c_str(), bridgeName_.c_str());

    close(fd);
    return true;
}




bool BridgeInterface::removeInterfaceFromBridge(const std::string& interfaceName) {
    struct ifreq ifr;
    strncpy(ifr.ifr_name, bridgeName_.c_str(), IFNAMSIZ);
    strncpy(ifr.ifr_slave, interfaceName.c_str(), IFNAMSIZ);

    if (ioctl(sockfd_, SIOCBRDELIF, &ifr) == -1) {
       Logger::log(LogLevel::LOG_ERROR, "[%s:%d]  %s", __func__, __LINE__, strerror(errno));

        return false;
    }

    return true;
}

