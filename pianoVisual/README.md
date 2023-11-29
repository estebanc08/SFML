# How to run
### Submodules
Firstly, make sure the appropriate submodules are installed. To do this, run 
```bash
git submodule sync
git submodule update --init --recursive --remote  
```
Now, navigate to the midifles directory and run `make` to make sure the library dependencies are installed.
### Creating Wav files
Add Midi files to directory outputMidi and then run `./makeList.sh` to make a new .wav file if needed. To speed up the process, can use
control-z and then use the bg command to allow it to run in the background.<br>
Run `make` command to update executable if needed and run the program. 

# Demo
Rach 3 Demo <br>
[![Rach 3 Demo](https://img.youtube.com/vi/dlsCJ1E4_k8/0.jpg)](https://youtu.be/dlsCJ1E4_k8)
