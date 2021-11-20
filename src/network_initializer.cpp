#include "network_initializer.h"

#include <array>
#include <memory>
#include <queue>
#include <semaphore>
#include <thread>
#include <unordered_map>

#include "game/NetworkedPlayerController.h"
#include "general/algorithms/RandomNumberGenerator.h"
#include "general/math/math.h"

namespace {
float OffsetValueByDeadzone(const float value, const float deadzone) {
  if (std::abs(value) < deadzone) {
    return 0.0f;
  }

  if (value < 0.0f) {
    return value + deadzone;
  }

  return value - deadzone;
}
}  // namespace

class LocalAppNetwork : public AppNetwork {
 public:
  LocalAppNetwork() {
    std::unordered_map<Attribute, float>& mainPlayerAttributes = values_[0];
    mainPlayerAttributes[Attribute::RotationX] = 0.0f;
    mainPlayerAttributes[Attribute::RotationY] = 0.0f;
    mainPlayerAttributes[Attribute::VelocityZ] = 0.0f;
    mainPlayerAttributes[Attribute::PositionX] = 0.0f;
    mainPlayerAttributes[Attribute::PositionY] = 0.0f;
    mainPlayerAttributes[Attribute::PositionZ] = 0.0f;

    for (u32 player = 1; player < 3; ++player) {
      spawns_.push(player);
      std::unordered_map<Attribute, float>& playerAttributes = values_[player];
      playerAttributes[Attribute::RotationX] = 0.0f;
      playerAttributes[Attribute::RotationY] = 0.0f;
      playerAttributes[Attribute::VelocityZ] = 0.0f;
      playerAttributes[Attribute::PositionX] = 0.0f;
      playerAttributes[Attribute::PositionY] = 0.0f;
      playerAttributes[Attribute::PositionZ] = 0.0f;
    }
  }

  u32 ActorIndexForCurrentApp() const override {
    return 0;
  }

  float ReadFloat(const u32 actorId, const Attribute attribute) const override {
    if (!values_.contains(actorId)) {
      return 0.0f;
    }

    if (!values_.at(actorId).contains(attribute)) {
      return 0.0f;
    }

    return values_.at(actorId).at(attribute);
  }

  void Update(const UpdateContext& context) override {
    std::unordered_map<Attribute, float>& mainPlayerAttributes = values_[0];

    glm::vec2 movement = context.controls.AxisValue(Axis::Movement);
    movement.x = -movement.x;

    constexpr float Deadzone = 0.1f;
    movement.x = OffsetValueByDeadzone(movement.x, Deadzone);
    movement.y = OffsetValueByDeadzone(movement.y, Deadzone);

    mainPlayerAttributes[Attribute::RotationX] +=
        movement.y * context.deltaTime;
    mainPlayerAttributes[Attribute::RotationY] +=
        movement.x * context.deltaTime;

    float acceleration = 0.0f;
    if (context.controls.IsControlActive(Control::Accelerate)) {
      acceleration += 1.0f;
    }
    if (context.controls.IsControlActive(Control::Decelerate)) {
      acceleration -= 1.0f;
    }
    mainPlayerAttributes[Attribute::VelocityZ] +=
        acceleration * context.deltaTime;
    mainPlayerAttributes[Attribute::VelocityZ] =
        CoerceToRange(mainPlayerAttributes[Attribute::VelocityZ], 0.0f, 1.0f);

    constexpr float movementSpeed = 10.0f;

    const glm::vec3 shipMovement =
        glm::toMat4(glm::quat(
            glm::vec3(mainPlayerAttributes[Attribute::RotationX],
                      mainPlayerAttributes[Attribute::RotationY], 0.0f))) *
        glm::vec4(0.0f, 0.0f, mainPlayerAttributes[Attribute::VelocityZ], 0.0f);

    if ((std::abs(shipMovement.x) + std::abs(shipMovement.y) +
         std::abs(shipMovement.z)) > 0.0f) {
      const glm::vec3 normalizedMovement =
          glm::normalize(shipMovement) * movementSpeed * context.deltaTime;
      mainPlayerAttributes[Attribute::PositionX] += normalizedMovement.x;
      mainPlayerAttributes[Attribute::PositionY] += normalizedMovement.y;
      mainPlayerAttributes[Attribute::PositionZ] += normalizedMovement.z;
    }

    //    for (u32 player = 1; player < values_.size(); ++player) {
    //      std::unordered_map<Attribute, float>& attributes = values_[player];
    //
    //      attributes[Attribute::RotationX] =
    //          randomNumberGenerator_.RandomFloat(-FullTurn, FullTurn);
    //      attributes[Attribute::RotationY] =
    //          randomNumberGenerator_.RandomFloat(-FullTurn, FullTurn);
    //      attributes[Attribute::VelocityZ] = 1.0f;
    //    }

    for (auto& controller : networkedPlayerControllers_) {
      controller->Update(*this);
    }
  }

  void ExecuteSpawns(PlayerSpawnConsumer& spawnConsumer) override {
    if (!spawns_.empty()) {
      std::unique_ptr<NetworkedPlayerController> controller =
          std::make_unique<NetworkedPlayerController>(spawns_.front());
      PlayerController& playerController = *controller;
      networkedPlayerControllers_.push_back(std::move(controller));
      spawnConsumer.SpawnPlayer(playerController);
      spawns_.pop();
    }
  }

 private:
  RandomNumberGenerator randomNumberGenerator_;
  std::unordered_map<u32, std::unordered_map<Attribute, float>> values_;
  std::queue<u32> spawns_;
  std::vector<std::unique_ptr<NetworkedPlayerController>>
      networkedPlayerControllers_;
};

#include <winsock2.h>

#include <string>

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

static constexpr const char* const SERVER_IP = "80.7.186.32";
static constexpr u16 SERVER_PORT = 5555;

#include <iostream>

int receive(SOCKET socket, const u64 wait) {
  timeval timeout;
  fd_set fds;

  timeout.tv_sec = wait;
  FD_ZERO(&fds);
  FD_SET(socket, &fds);

  return select(0, &fds, 0, 0, &timeout);
}

class GlobalUdpNetwork : public AppNetwork {
 public:
  GlobalUdpNetwork() : serverAddress_({}), semaphore_(0), running_(true) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    serverSocket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (serverSocket_ == SOCKET_ERROR) {
      throw std::runtime_error(std::to_string(WSAGetLastError()));
    }

    serverAddress_.sin_family = AF_INET;
    serverAddress_.sin_port = htons(SERVER_PORT);
    serverAddress_.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
    serverAddressSize_ = sizeof(serverAddress_);

    {
      std::array<char, 1024> buffer;

      WriteValue(static_cast<u32>(0), buffer.data());

      sendto(serverSocket_, buffer.data(), 4, 0,
             reinterpret_cast<sockaddr*>(&serverAddress_),
             sizeof(serverAddress_));

      const i32 bytesReceived = recvfrom(
          serverSocket_, buffer.data(), buffer.size(), 0,
          reinterpret_cast<SOCKADDR*>(&serverAddress_), &serverAddressSize_);

      if (bytesReceived <= 0) {
        throw std::runtime_error("no bytes?!");
      }

      id_ = ReadBytes<u32>(buffer.data());

      std::unordered_map<Attribute, float>& mainPlayerAttributes = values_[id_];
      mainPlayerAttributes[Attribute::RotationX] = 0.0f;
      mainPlayerAttributes[Attribute::RotationY] = 0.0f;
      mainPlayerAttributes[Attribute::VelocityZ] = 0.0f;
      mainPlayerAttributes[Attribute::PositionX] = 0.0f;
      mainPlayerAttributes[Attribute::PositionY] = 0.0f;
      mainPlayerAttributes[Attribute::PositionZ] = 0.0f;
    }

    thread_ = std::thread(&GlobalUdpNetwork::CommunicateWithServer, this);
    receiveThread_ = std::thread(&GlobalUdpNetwork::Receive, this);

    //    for (u32 player = 1; player < 3; ++player) {
    //      spawns_.push(player);
    //      std::unordered_map<Attribute, float>& playerAttributes =
    //      values_[player]; playerAttributes[Attribute::RotationX] = 0.0f;
    //      playerAttributes[Attribute::RotationY] = 0.0f;
    //      playerAttributes[Attribute::VelocityZ] = 0.0f;
    //    }
  }

  ~GlobalUdpNetwork() {
    running_ = false;
    semaphore_.release();
    thread_.join();
    receiveThread_.join();
  }

  u32 ActorIndexForCurrentApp() const override {
    return id_;
  }

  float ReadFloat(const u32 actorId, const Attribute attribute) const override {
    if (!values_.contains(actorId)) {
      std::cout << "No such actor " << actorId << std::endl;
    }

    if (!values_.at(actorId).contains(attribute)) {
      std::cout << "No such attribute " << static_cast<i32>(attribute)
                << " for actor " << actorId << std::endl;
    }

    return values_.at(actorId).at(attribute);
  }

  void Update(const UpdateContext& context) override {
    glm::vec2 movement = context.controls.AxisValue(Axis::Movement);
    movement.x = -movement.x;

    constexpr float Deadzone = 0.1f;
    movement.x = OffsetValueByDeadzone(movement.x, Deadzone);
    movement.y = OffsetValueByDeadzone(movement.y, Deadzone);

    movement_ = movement;

    accelerate_ = context.controls.IsControlActive(Control::Accelerate);
    decelerate_ = context.controls.IsControlActive(Control::Decelerate);

    semaphore_.release();

    for (const auto& other : networkedPlayerControllers_) {
      other->Update(*this);
    }
  }

  void ExecuteSpawns(PlayerSpawnConsumer& spawnConsumer) override {
    if (!spawns_.empty()) {
      std::unique_ptr<NetworkedPlayerController> controller =
          std::make_unique<NetworkedPlayerController>(spawns_.front());
      PlayerController& playerController = *controller;
      networkedPlayerControllers_.push_back(std::move(controller));
      spawnConsumer.SpawnPlayer(playerController);
      spawns_.pop();
    }
  }

 private:
  void CommunicateWithServer() {
    while (true) {
      semaphore_.acquire();

      if (!running_) {
        break;
      }

      std::array<char, 1024> buffer;

      WriteValue(static_cast<u32>(1), buffer.data());
      WriteValue(movement_.x, buffer.data() + 4);
      WriteValue(movement_.y, buffer.data() + 8);
      WriteValue(accelerate_, buffer.data() + 12);
      WriteValue(decelerate_, buffer.data() + 13);

      sendto(serverSocket_, buffer.data(), 14, 0,
             reinterpret_cast<sockaddr*>(&serverAddress_),
             sizeof(serverAddress_));
    }
  }

  void Receive() {
    std::array<char, 1024> buffer;

    while (running_) {
      if (receive(serverSocket_, 1) == 0) {
        // Timeout
        continue;
      }

      const i32 bytesReceived = recvfrom(
          serverSocket_, buffer.data(), buffer.size(), 0,
          reinterpret_cast<SOCKADDR*>(&serverAddress_), &serverAddressSize_);

      if (bytesReceived <= 0) {
        throw std::runtime_error("Couldn't receive");
      }

      const u32 playerId = ReadBytes<u32>(buffer.data());
      const float rotationX = ReadBytes<float>(buffer.data() + 4);
      const float rotationY = ReadBytes<float>(buffer.data() + 8);
      const float velocity = ReadBytes<float>(buffer.data() + 12);
      const float positionX = ReadBytes<float>(buffer.data() + 16);
      const float positionY = ReadBytes<float>(buffer.data() + 20);
      const float positionZ = ReadBytes<float>(buffer.data() + 24);

      if ((playerId != id_) && !values_.contains(playerId)) {
        spawns_.push(playerId);
        std::cout << "Spawn player " << playerId << std::endl;
      }

      //      if (playerId != id_) {
      //        std::cout << "Received talk of player " << playerId <<
      //        std::endl;
      //      }

      std::unordered_map<Attribute, float>& playerAttributes =
          values_[playerId];
      playerAttributes[Attribute::RotationX] = rotationX;
      playerAttributes[Attribute::RotationY] = rotationY;
      playerAttributes[Attribute::VelocityZ] = velocity;
      playerAttributes[Attribute::PositionX] = positionX;
      playerAttributes[Attribute::PositionY] = positionY;
      playerAttributes[Attribute::PositionZ] = positionZ;
    }
  }

 private:
  std::unordered_map<u32, std::unordered_map<Attribute, float>> values_;
  std::queue<u32> spawns_;
  std::vector<std::unique_ptr<NetworkedPlayerController>>
      networkedPlayerControllers_;
  SOCKET serverSocket_;
  sockaddr_in serverAddress_;
  i32 serverAddressSize_;
  u32 id_;
  std::binary_semaphore semaphore_;
  glm::vec2 movement_;
  bool accelerate_;
  bool decelerate_;
  std::thread thread_;
  std::thread receiveThread_;
  bool running_;
};

std::unique_ptr<AppNetwork> CreateNetwork() {
//    return std::make_unique<LocalAppNetwork>();
  return std::make_unique<GlobalUdpNetwork>();
}
