#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

typedef enum {
  LOADER_IDLE,
  LOADER_LOADING,
  LOADER_COMPLETE,
  LOADER_ERROR
} LoaderStatus;

// Texture loader
void StartTextureLoader(void);
LoaderStatus GetTextureLoaderStatus(void);
float GetTextureLoaderProgress(void);
void WaitForTextureLoader(void);
void StopTextureLoader(void);

// Music manager
void StartMusicManager(void);
void StopMusicManager(void);
void SetMusicTrackThreaded(const char *trackPath);
void SetMusicVolumeThreaded(float volume);
float GetMusicVolumeThreaded(void);

#endif
