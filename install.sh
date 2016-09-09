export PATH=$PATH:$PWD
exec zsh
if [[ $rc != 0]]; then exec bash; fi
