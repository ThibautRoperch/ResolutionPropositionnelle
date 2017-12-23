echo "Deux instances du problème des pigeonniers vont être résolues par toutes les méthodes, leur temps d'exécution va être mesuré."
echo ""

echo "********** Instance SATISFIABLE : 4 pigeons et 4 pigeonniers **********"

echo "* M1 : brute (approche naïve) *"
time ./bin/pigeons.exe -p 4 -c 4 -m 1
echo "" 
echo "* M2 : efficace récursive *"
time ./bin/pigeons.exe -p 4 -c 4 -m 2
echo "" 
echo "* M3 : brute parallélisée avec OpenMP *"
time ./bin/pigeons.exe -p 4 -c 4 -m 3
echo "" 
echo "* M4 : brute parallélisée avec MPI *"
time ./bin/pigeons.exe -p 4 -c 4 -m 4
echo "" 
echo "* M5 : efficace parallélisée avec OpenMP *"
time ./bin/pigeons.exe -p 4 -c 4 -m 5

echo "" 
echo "_______________________________________________________________"
echo ""

echo "********** Instance INSATISFIABLE : 5 pigeons et 4 pigeonniers **********"

echo "* M1 : brute (approche naïve) *"
time ./bin/pigeons.exe -p 5 -c 4 -m 1
echo "" 
echo "* M2 : efficace récursive *"
time ./bin/pigeons.exe -p 5 -c 4 -m 2
echo "" 
echo "* M3 : brute parallélisée avec OpenMP *"
time ./bin/pigeons.exe -p 5 -c 4 -m 3
echo "" 
echo "* M4 : brute parallélisée avec MPI *"
time ./bin/pigeons.exe -p 5 -c 4 -m 4
echo "" 
echo "* M5 : efficace parallélisée avec OpenMP *"
time ./bin/pigeons.exe -p 5 -c 4 -m 5
