#include "external/tinycthread.h"
#include "string.h"
#include "game/thread_manager.h"
#include "time.h"
#include "stdbool.h"

// function prototypes
void LoadAllTexturesAsync(void);
void ProcessTextureLoadingOnMainThread(void);
void InitTextures(void);

// same reason as below
typedef struct {
  unsigned int sampleRate;
  unsigned int sampleSize;
  unsigned int channels;
  void *buffer;
  unsigned int bufferSizeInFrames;
  unsigned int framesProcessed;
  unsigned int looping;
  void *usedSequencer;
} AudioStream;

// clone the raylib music struct because including raylib has conflicts with windows.h
typedef struct {
  AudioStream stream;
  unsigned int frameCount;
  unsigned int looping;
  int ctxType;
  void *ctxData;
} Music;

Music LoadMusicStream(const char *fileName);
void UnloadMusicStream(Music music);
void PlayMusicStream(Music music);
void StopMusicStream(Music music);
void SetMusicVolume(Music music, float volume);
void UpdateMusicStream(Music music);

// ===== TEXTURE LOADER =====
static thrd_t textureLoaderThread;
static mtx_t textureLoaderMutex;
static LoaderStatus textureLoaderStatus = LOADER_IDLE;
static int textureLoaderProgress = 0;
static volatile bool textureLoaderRunning = false;

static int TextureLoaderThreadFunc(void *arg) {
  (void)arg;
  
  mtx_lock(&textureLoaderMutex);
  textureLoaderStatus = LOADER_LOADING;
  textureLoaderProgress = 0;
  mtx_unlock(&textureLoaderMutex);
  
  // Load all image data from disk (no OpenGL needed)
  LoadAllTexturesAsync();
  
  mtx_lock(&textureLoaderMutex);
  textureLoaderProgress = 100;
  textureLoaderStatus = LOADER_COMPLETE;
  mtx_unlock(&textureLoaderMutex);
  
  return 0;
}

void StartTextureLoader(void) {
  if (textureLoaderRunning) return;
  
  mtx_init(&textureLoaderMutex, mtx_plain);
  textureLoaderRunning = true;
  textureLoaderStatus = LOADER_IDLE;
  
  thrd_create(&textureLoaderThread, TextureLoaderThreadFunc, NULL);
}

LoaderStatus GetTextureLoaderStatus(void) {
  LoaderStatus status;
  mtx_lock(&textureLoaderMutex);
  status = textureLoaderStatus;
  mtx_unlock(&textureLoaderMutex);
  return status;
}

float GetTextureLoaderProgress(void) {
  float progress;
  mtx_lock(&textureLoaderMutex);
  progress = (float)textureLoaderProgress / 100.0f;
  mtx_unlock(&textureLoaderMutex);
  return progress;
}

void WaitForTextureLoader(void) {
  if (!textureLoaderRunning) return;
  
  int result;
  thrd_join(textureLoaderThread, &result);
  mtx_destroy(&textureLoaderMutex);
  textureLoaderRunning = false;
}

void StopTextureLoader(void) {
  WaitForTextureLoader();
}

// ===== MUSIC MANAGER =====
static thrd_t musicManagerThread;
static mtx_t musicMutex;
static Music currentMusic = {0};
static volatile bool musicManagerRunning = false;
static volatile bool musicShouldStop = false;
static char currentMusicPath[256] = {0};
static float musicVolume = 1.0f;

static int MusicManagerThreadFunc(void *arg) {
  (void)arg;
  
  while (musicManagerRunning && !musicShouldStop) {
    mtx_lock(&musicMutex);
    
    // Update music and volume
    if (currentMusic.frameCount > 0) {
      SetMusicVolume(currentMusic, musicVolume);
      UpdateMusicStream(currentMusic);
    }
    
    mtx_unlock(&musicMutex);
    
    // Sleep 16ms (roughly 60fps)
    #ifdef _WIN32
    Sleep(16);
    #else
    struct timespec ts = {0, 16000000};  // 16ms in nanoseconds
    thrd_sleep(&ts, NULL);
    #endif
  }
  
  return 0;
}

void StartMusicManager(void) {
  if (musicManagerRunning) return;
  
  mtx_init(&musicMutex, mtx_plain);
  musicManagerRunning = true;
  musicShouldStop = false;
  
  thrd_create(&musicManagerThread, MusicManagerThreadFunc, NULL);
}

void StopMusicManager(void) {
  if (!musicManagerRunning) return;
  
  musicShouldStop = true;
  
  int result;
  thrd_join(musicManagerThread, &result);
  
  mtx_lock(&musicMutex);
  if (currentMusic.frameCount > 0) {
    StopMusicStream(currentMusic);
    UnloadMusicStream(currentMusic);
    currentMusic = (Music){0};
  }
  mtx_unlock(&musicMutex);
  
  mtx_destroy(&musicMutex);
  musicManagerRunning = false;
}

void SetMusicTrackThreaded(const char *trackPath) {
  if (!musicManagerRunning || !trackPath) return;
  
  mtx_lock(&musicMutex);
  
  // Stop and unload current music
  if (currentMusic.frameCount > 0) {
    StopMusicStream(currentMusic);
    UnloadMusicStream(currentMusic);
  }
  
  // Load new music
  currentMusic = LoadMusicStream(trackPath);
  strncpy(currentMusicPath, trackPath, sizeof(currentMusicPath) - 1);
  currentMusicPath[sizeof(currentMusicPath) - 1] = '\0';
  
  if (currentMusic.frameCount > 0) {
    PlayMusicStream(currentMusic);
    SetMusicVolume(currentMusic, musicVolume);
  }
  
  mtx_unlock(&musicMutex);
}

void SetMusicVolumeThreaded(float volume) {
  volume = volume < 0.0f ? 0.0f : (volume > 1.0f ? 1.0f : volume);
  
  mtx_lock(&musicMutex);
  musicVolume = volume;
  if (currentMusic.frameCount > 0) {
    SetMusicVolume(currentMusic, volume);
  }
  mtx_unlock(&musicMutex);
}

float GetMusicVolumeThreaded(void) {
  float volume;
  mtx_lock(&musicMutex);
  volume = musicVolume;
  mtx_unlock(&musicMutex);
  return volume;
}
