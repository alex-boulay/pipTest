make clean
git add . 
VAR=$(date "+%d-%m-%y %H:%M:%S")
git commit -m "autopush $USER $VAR"
git push -u origin master
