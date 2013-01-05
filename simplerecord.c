/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <sys/time.h>
#include <unistd.h>
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <pthread.h>

struct timeval start, end;
long mtime, seconds, useconds;


int record_one(snd_pcm_t *handle, char *buffer, int frames){
  int rc;

  gettimeofday(&start, NULL);
  rc = snd_pcm_readi(handle, buffer, frames);
  gettimeofday(&end, NULL);
  seconds  = end.tv_sec  - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  mtime = ((seconds) * 1000000 + useconds);;

  // fprintf(stderr, "Elapsed time: %ld microseconds, rc=%d\n", mtime, rc);
   
  if (rc == -EPIPE) {
    /* EPIPE means overrun */
    fprintf(stderr, "overrun occurred\n");
    snd_pcm_prepare(handle);
  } else if (rc < 0) {
    fprintf(stderr, "error from read: %s\n", snd_strerror(rc));
  } else if (rc != (int)frames) {
    fprintf(stderr, "short read, read %d frames\n", rc);
  }

  return(0);
}

int play_one(snd_pcm_t *handle, char *buffer, int frames){
  int rc;

  rc = snd_pcm_writei(handle, buffer, frames);
  if (rc == -EPIPE) {
    fprintf(stderr, "underrun occurred\n");
    snd_pcm_prepare(handle);
  } else if (rc < 0) {
    fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));
  }  else if (rc != (int)frames) {
    fprintf(stderr, "short write, write %d frames\n", rc);
  }
  return(0);
}


int main() {
  long loops;
  int rc, rc2;
  int size;
  snd_pcm_t *handle;
  snd_pcm_t *handle2;
  snd_pcm_hw_params_t *params;
  snd_pcm_hw_params_t *params2;
  unsigned int val, val2;
  int dir, dir2;
  snd_pcm_uframes_t frames, frames2;
  char *buffer;


  /* Open PCM device for recording (capture). */
  rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE | SND_PCM_NONBLOCK, 0);
//  rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0);
  if (rc < 0) {
    fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
    exit(1);
  }


  /* Open PCM device for playback. */
  rc2 = snd_pcm_open(&handle2, "default", SND_PCM_STREAM_PLAYBACK, 0);
  if (rc2 < 0) {
    fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc2));
    exit(1);
  }


/* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params2);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle2, params2);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle2, params2, SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle2, params2, SND_PCM_FORMAT_S16_LE);

  /* Two channels (stereo) */
  snd_pcm_hw_params_set_channels(handle2, params2, 2);

  /* 44100 bits/second sampling rate (CD quality) */
  val2 = 96000;
  snd_pcm_hw_params_set_rate_near(handle2, params2, &val2, &dir2);

  /* Set period size to 32 frames. */
  frames2 = 8;
  snd_pcm_hw_params_set_period_size_near(handle2, params2, &frames2, &dir2);

  /* Write the parameters to the driver */
  rc2 = snd_pcm_hw_params(handle2, params2);
  if (rc2 < 0) {
    fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc2));
    exit(1);
  }



  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

  /* Two channels (stereo) */
  snd_pcm_hw_params_set_channels(handle, params, 2);

  /* 44100 bits/second sampling rate (CD quality) */
  val = 96000;
  //val = 48000;
  snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

  /* Set period size to 32 frames. */
  frames = 8;
  snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) {
    fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
    exit(1);
  }

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params, &frames, &dir);
  size = frames * 4; /* 2 bytes/sample, 2 channels */
  buffer = (char *) malloc(size);

  fprintf(stderr, "sizeof buffer = %d\n", (int)size);
  /* We want to loop for 5 seconds */
  snd_pcm_hw_params_get_period_time(params, &val, &dir);
  loops = 50000000 / val;
  int cnt =0;

  //pthread_t *alsa_t = malloc(sizeof(pthread_t)*2);

  while (loops > 0) {
    fprintf(stderr, "called %d\n",cnt++);
    loops--;

//    pthread_create(&alsa_t[0], NULL, &record_one( handle, buffer, frames), NULL);
//    pthread_create(record_t, NULL, play_one(handle2, buffer, frames), NULL);
    record_one(handle, buffer, frames);
    play_one(handle2, buffer, frames);
    
  }

  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);

//  pthread_join(alsa_t[0], NULL);
//  pthread_join(play_t, NULL);

  return 0;
}
