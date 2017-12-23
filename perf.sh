echo "Deux instances du problème des pigeonniers vont être résolues par la méthode 1 (brute non parallélisée), leur temps d'exécution va être mesuré."
echo ""

echo "********** Instance SATISFIABLE : 4 pigeons et 4 pigeonniers **********"

time ./bin/pigeons.exe -p 4 -c 4 -m 1
echo "" 


echo "_______________________________________________________________"
echo ""
echo "********** Instance INSATISFIABLE : 5 pigeons et 4 pigeonniers **********"

time ./bin/pigeons.exe -p 5 -c 4 -m 1
