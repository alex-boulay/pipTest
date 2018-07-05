make clean
git add . 
VAR=$(date "+%d-%m-%y %H:%M:%S")
git commit -m "autopush AlexBoulay $VAR"
git push -u origin master
