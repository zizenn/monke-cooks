#pragma once

#include "raylib.h"
class Time {
private:
      static inline float timeScale_ = 1.0f;
      static inline float deltaTime_ = 0.0f;
      static inline double totalTime_ =
            0.0; // Fixed: Matches GetTime() double type
      static inline int frameCount_ = 0;

public:
      // constructor delete to prevent instantiation
      Time() = delete;

      static void Update() {
            deltaTime_ = GetFrameTime();
            totalTime_ = GetTime();
            frameCount_++;
      }

      // [[nodiscard]] forces the compiler to warn if the return value is
      // ignored
      [[nodiscard]] static float DeltaTime() { return deltaTime_ * timeScale_; }
      [[nodiscard]] static float UnscaledDeltaTime() { return deltaTime_; }
      [[nodiscard]] static double TotalTime() { return totalTime_; }
      [[nodiscard]] static int FrameCount() { return frameCount_; }

      static void SetGameSpeed(float speed) { timeScale_ = speed; }
};
