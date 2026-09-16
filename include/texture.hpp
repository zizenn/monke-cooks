#pragma once

#include <string>
#include <vector>
#include "external/raylib.h"

namespace tex {

struct TexturePacket {
      std::string name;
      Texture2D tex;
};

class TextureManager {
private:
      std::string TextureManifestPath_;
      std::vector<TexturePacket> loadedTexArray_;

public:
      TextureManager(std::string filePath) : TextureManifestPath_(filePath) {}
      ~TextureManager() {
            Unload();
      }

      void Load();

      void Unload() {
            for(const auto& item : loadedTexArray_) {
                  UnloadTexture(item.tex);
            }
            loadedTexArray_.clear();
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
