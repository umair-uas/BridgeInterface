#include <iostream>
#include <cstring>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <linux/if_bridge.h>
#include <sys/socket.h>
#include <unistd.h>

bool createBridge(const std::string& bridgeName) {
    int sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return false;
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, bridgeName.c_str(), IFNAMSIZ);

    if (ioctl(sockfd, SIOCBRADDBR, &ifr) == -1) {
        perror("ioctl");
        close(sockfd);
        return false;
    }

    close(sockfd);
    return true;
}

int main() {
    std::string bridgeName = "my_bridge";
    if (createBridge(bridgeName)) {
        std::cout << "Bridge '" << bridgeName << "' created successfully.\n";
    } else {
        std::cerr << "Failed to create bridge '" << bridgeName << "'.\n";
        return 1;
    }

    return 0;
}
