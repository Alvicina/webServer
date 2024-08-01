# /bin/bash

printf "Compiling server...\n"
make re
make clean
valgrind --leak-check=full --show-leak-kinds=all --error-limit=no --log-file=valgrind.log ./webserv test/test.conf > webserv.log 2>&1 &
SERVER_PID=$!

printf "Initializing server...\n"
sleep 5
printf "\n"

python3 test/test_basic_requests.py

kill -2 $SERVER_PID
wait $SERVER_PID

printf "\nValgrind report:\n"
cat valgrind.log

printf "\nTo print the server log use the following command:\n"
printf "cat webserv.log\n"
