This is an Analisys code written to work symbiontically with ARICH_G4 [either v10.3 or 11.0.2].

The makefile should work, if it doesn't I'm sorry. It also need to source root (if not done yet).

how to use it: \\
bash:
  make 
  ./bin/ana [events_root_file_name] [particle_E] (argv[2] is the particle name but I usually add the energy to not get confused)

It saves the files into a directory called "out_100k_timew" but it can be changed at your convenience.

Other than that, it not a complicated code so hopefully works on your device too.
