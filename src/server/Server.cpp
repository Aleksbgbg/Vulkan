#include "Server.h"

#include <winsock2.h>
#undef min
#undef max

#include <array>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "inputs.h"
#include "util/include/glm.h"
#include "util/types.h"

#define PROCEED_ON_VALID_RESULT(result)             \
  if ((result) != 0) {                              \
    throw std::runtime_error(gai_strerror(result)); \
  }

#define TERMINATE_ON_INVALID_RESULT(result, invalid)             \
  if ((result) == (invalid)) {                                   \
    throw std::runtime_error(std::to_string(WSAGetLastError())); \
  }

static constexpr u16 Port = 5555;

int receive(SOCKET socket, const u64 wait) {
  timeval timeout;
  fd_set fds;

  timeout.tv_sec = wait;
  FD_ZERO(&fds);
  FD_SET(socket, &fds);

  return select(0, &fds, 0, 0, &timeout);
}

template <typename T>
T ReadBytes(const char* const stream) {
  return *reinterpret_cast<const T*>(stream);
}

template <typename T>
void WriteValue(const T value, char* const stream) {
  const u8* bytes = reinterpret_cast<const u8*>(&value);

  for (u32 byte = 0; byte < sizeof(T); ++byte) {
    stream[byte] = bytes[byte];
  }
}

struct PlayerInfo {
  float rotationX;
  float rotationY;
  float velocity;
};

class PlayerCommunicator {
 public:
  PlayerCommunicator(u32 id, SOCKET socket, SOCKADDR_IN address, i32 size)
      : id_(id),
        listenSocket_(socket),
        senderAddress_(address),
        senderAddressSize_(size),
        previousTime_(std::chrono::high_resolution_clock::time_point::min()),
        velocity_(0.0f),
        rotationX_(0.0f),
        rotationY_(0.0f),
        positionX_(0.0f),
        positionY_(0.0f),
        positionZ_(0.0f) {}

  bool operator==(const PlayerCommunicator& other) const {
    return id_ == other.id_;
  }

  void Introduce() {
    std::array<char, 4> buffer;

    WriteValue(id_, buffer.data());

    sendto(listenSocket_, buffer.data(), buffer.size(), 0,
           reinterpret_cast<const SOCKADDR*>(&senderAddress_),
           senderAddressSize_);
  }

  void Update(std::array<char, 1024>& buffer) {
    if (previousTime_ ==
        std::chrono::high_resolution_clock::time_point::min()) {
      previousTime_ = std::chrono::high_resolution_clock::now();
    }

    const auto timeNow = std::chrono::high_resolution_clock::now();
    const float deltaTime =
        std::chrono::duration<float, std::chrono::seconds::period>(
            timeNow - previousTime_)
            .count();
    previousTime_ = timeNow;

    const float movementX = ReadBytes<float>(buffer.data() + 4);
    const float movementY = ReadBytes<float>(buffer.data() + 8);
    const bool accelerating = ReadBytes<bool>(buffer.data() + 12);
    const bool decelerating = ReadBytes<bool>(buffer.data() + 13);

    float acceleration = 0.0f;
    if (accelerating) {
      acceleration += 1.0f;
    }
    if (decelerating) {
      acceleration -= 1.0f;
    }

    std::cout << "player " << id_ << " communicated after " << deltaTime
              << "s moving {" << movementX << ", " << movementY
              << "} (accelerating: " << acceleration << ")" << std::endl;

    velocity_ += acceleration * deltaTime;
    velocity_ = std::max(0.0f, std::min(1.0f, velocity_));

    rotationX_ += movementY * deltaTime;
    rotationY_ += movementX * deltaTime;

    constexpr float movementSpeed = 10.0f;

    const glm::vec3 shipMovement =
        glm::toMat4(glm::quat(
            glm::vec3(rotationX_,
                      rotationY_, 0.0f))) *
        glm::vec4(0.0f, 0.0f, velocity_, 0.0f);

    if ((std::abs(shipMovement.x) + std::abs(shipMovement.y) +
         std::abs(shipMovement.z)) > 0.0f) {
      const glm::vec3 normalizedMovement =
          glm::normalize(shipMovement) * movementSpeed * deltaTime;
      positionX_ += normalizedMovement.x;
      positionY_ += normalizedMovement.y;
      positionZ_ += normalizedMovement.z;
    }
  }

  void SendUpdate(const PlayerCommunicator& source) const {
    std::array<char, 28> buffer;

    WriteValue(source.id_, buffer.data());
    WriteValue(source.rotationX_, buffer.data() + 4);
    WriteValue(source.rotationY_, buffer.data() + 8);
    WriteValue(source.velocity_, buffer.data() + 12);
    WriteValue(source.positionX_, buffer.data() + 16);
    WriteValue(source.positionY_, buffer.data() + 20);
    WriteValue(source.positionZ_, buffer.data() + 24);

    sendto(listenSocket_, buffer.data(), buffer.size(), 0,
           reinterpret_cast<const SOCKADDR*>(&senderAddress_),
           senderAddressSize_);
  }

 private:
  u32 id_;
  SOCKET listenSocket_;
  SOCKADDR_IN senderAddress_;
  i32 senderAddressSize_;
  std::chrono::time_point<std::chrono::high_resolution_clock> previousTime_;
  float velocity_;
  float rotationX_;
  float rotationY_;
  float positionX_;
  float positionY_;
  float positionZ_;
};

struct NetworkId {
  u64 ip;
  u16 port;

  bool operator==(const NetworkId& other) const {
    return (ip == other.ip) && (port == other.port);
  }
};

namespace std {

template <>
class std::hash<NetworkId> {
 public:
  size_t operator()(const NetworkId& value) const {
    return std::hash<u64>()(value.ip) ^ std::hash<u16>()(value.port);
  }
};

}  // namespace std

void Server::Run() {
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);

  //  addrinfo hints{};
  //  addrinfo* result;
  //
  //  hints.ai_family = AF_INET;
  //  hints.ai_socktype = SOCK_DGRAM;
  //  hints.ai_protocol = IPPROTO_UDP;
  //  hints.ai_flags = AI_PASSIVE;
  //
  //  PROCEED_ON_VALID_RESULT(getaddrinfo(NULL, Port, &hints, &result));

  //  SOCKET listenSocket =
  //      socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  //  TERMINATE_ON_INVALID_RESULT(listenSocket, INVALID_SOCKET);
  //
  //  TERMINATE_ON_INVALID_RESULT(
  //      bind(listenSocket, result->ai_addr, (int)result->ai_addrlen),
  //      SOCKET_ERROR);

  SOCKET listenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  TERMINATE_ON_INVALID_RESULT(listenSocket, INVALID_SOCKET);

  SOCKADDR_IN receiverAddress{};
  receiverAddress.sin_family = AF_INET;
  receiverAddress.sin_port = htons(Port);
  receiverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

  TERMINATE_ON_INVALID_RESULT(
      bind(listenSocket, reinterpret_cast<SOCKADDR*>(&receiverAddress),
           sizeof(receiverAddress)),
      SOCKET_ERROR);

  std::array<char, 1024> buffer;

  SOCKADDR_IN senderAddress;
  i32 senderAddressSize = sizeof(senderAddress);

  std::unordered_map<u32, std::unordered_map<u32, float>> floatAttributes;

  std::chrono::time_point<std::chrono::high_resolution_clock> previousTime =
      std::chrono::high_resolution_clock::time_point::min();

  std::unordered_map<NetworkId, PlayerCommunicator> players;

  while (true) {
    if (previousTime == std::chrono::high_resolution_clock::time_point::min()) {
      previousTime = std::chrono::high_resolution_clock::now();
    }

    const i32 bytesReceived = recvfrom(
        listenSocket, buffer.data(), buffer.size(), 0,
        reinterpret_cast<SOCKADDR*>(&senderAddress), &senderAddressSize);

    if (bytesReceived > 0) {
      //        Input input =
      //            static_cast<Input>(ReadBytes<i32>(4,
      //            receiveBuffer_.data()));
      //        Type type =
      //            static_cast<Type>(ReadBytes<i32>(4, receiveBuffer_.data()
      //            + 4));

      const u64 sourceAddress = senderAddress.sin_addr.S_un.S_addr;
      const u16 sourcePort = senderAddress.sin_port;
      const NetworkId networkId = {sourceAddress, sourcePort};

      const u32 messageType = ReadBytes<u32>(buffer.data());

      // std::cout << "Got message " << messageType << std::endl;

      if (messageType == 0) {  // connect
        if (!players.contains(networkId)) {
          players.insert({networkId, PlayerCommunicator(
                                         players.size(), listenSocket,
                                         senderAddress, senderAddressSize)});
          PlayerCommunicator& p = players.at(networkId);

          p.Introduce();

          for (const auto& pair : players) {
            if (pair.second == p) {
              continue;
            }

            p.SendUpdate(pair.second);
            pair.second.SendUpdate(p);
          }
        }
      } else if (messageType == 1) {  // Update position
        if (!players.contains(networkId)) {
          continue;
        }

        PlayerCommunicator& p = players.at(networkId);
        p.Update(buffer);

        for (const auto& pair : players) {
          pair.second.SendUpdate(p);
        }
      }
    } else {
      std::cout << std::string("Player ") + " closed connection " +
                       std::to_string(WSAGetLastError())
                << std::endl;
      break;
    }
  }
}
