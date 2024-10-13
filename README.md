# README

This README provides setup and usage instructions for four different C programs, each named `task1.c` to `task4.c`. These tasks include:
- **Task 1: Music Player Setup Guide**
- **Task 2: Binary Search Tree Database Setup Guide**
- **Task 3: Social Network Recommendation System Setup Guide**
- **Task 4: Emergency Room Patient Management Setup Guide**

Each task has its own instructions for installation, compilation, and usage.

---

## Task 1: Music Player Setup Guide

This guide helps you set up and run a **Music Playlist Manager** that uses PortAudio for audio playback and libsndfile for reading audio files.

### System Requirements
- C compiler (GCC or Clang)
- CMake (for building PortAudio)
- Git (for cloning repositories)

### Setup and Compilation

1. **Install PortAudio and libsndfile** (See specific instructions for [Windows](#windows-setup), [macOS](#macos-setup), or [Linux](#linux-setup)).
2. Save the music player code as `task1.c`.
3. Compile the program:
   - **Windows**:
     ```bash
     gcc -o task1 task1.c -IC:\portaudio\include -IC:\libsndfile\include -LC:\portaudio\build\Release -LC:\libsndfile\bin -lportaudio -lsndfile-1
     ```
   - **macOS/Linux**:
     ```bash
     gcc -o task1 task1.c -lportaudio -lsndfile
     ```
4. Run the program:
   ```bash
   ./task1
