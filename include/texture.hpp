# pragma once

#include <queue>
#include <string>
#include <vector>
#include "external/raylib.h"

namespace tex {

class TextureManager {
private:
      std::string TextureManifestPath_;
      std::vector<Texture2D> loadedTexArray_;

      // get a queue of texture file paths to load from json file
      std::queue<std::string> DecodeFilePathsFromJSON_();

public:
      TextureManager(std::string filePath) : TextureManifestPath_(filePath) {}
      ~TextureManager() {
            Unload();
      }

      void Load();

      void Unload() {
            for(const auto& tex : loadedTexArray_) {
                  UnloadTexture(tex);
            }
      }
};



// i tried making it async, didnt work ill finish it later
// struct ImagePacket {
//       Image image;
//       std::string path;
// };
//
// class TextureManager {
// private:
//       // -- variables --
//       std::vector<std::string> texPaths_;
//       int arrayLen_;
//
//       // async stuff
//       std::queue<ImagePacket> loadedTex_;
//       std::queue<ImagePacket> toLoadTex_;
//       std::mutex texMutex_;
//       std::jthread workerThread_;
//       std::condition_variable_any cv_;
//
//       // functions
//       void CreateArrayFromSize(int size);
//
//       void stop() {
//           if (workerThread_.joinable()) {
//               workerThread_.request_stop();
//               cv_.notify_all();
//           }
//       }
//
//
// public:
//       void AddTexToPaths(std::string path);
//       [[nodiscard]] int LoadTextures();
//
// };

}
