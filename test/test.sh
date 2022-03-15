RESET="\033[0m"
BLACK="\033[30m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
WHITE="\033[37m"

BOLDBLACK="\033[1m\033[30m"
BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
BOLDYELLOW="\033[1m\033[33m"
BOLDBLUE="\033[1m\033[34m"
BOLDMAGENTA="\033[1m\033[35m"
BOLDCYAN="\033[1m\033[36m"
BOLDWHITE="\033[1m\033[37m"
# "rl_outstream = stderr;" поместить в мейн.
# Compile and set executable rights
make -C ../ > /dev/null
cp ../minishell .
chmod 755 minishell


GOOD=0
TOTAL=0

pipe=/tmp/testpipe
trap "rm -f $pipe" EXIT
if [[ ! -p $pipe ]]; then
    mkfifo $pipe
fi

function exec_test()
{
  
  # execute commands, separated by ';' in minishell, using nfifo
  ./minishell <$pipe >msh_log 2>&-  &
  mshpid=$!
  IFS=';' read -ra CMND <<< "$@"
  for command in "${CMND[@]}"; do
    echo $command > $pipe
  done

  # only leaks mode
  if [ $LEAKSMODE  ]; then
    echo "leaks $mshpid | grep 'leaks for' | grep -v ' 0 leaks' >> leaks" > $pipe
    echo 'exit' > $pipe 
    sleep 0.02
    wait $!
    sleep 0.02
    if [[ $(cat leaks 2>&-) != "" ]]; then
      printf "\n$BOLDRED LEAKS! $YELLOW%s$RESET\n" "$@"
      rm -f leaks
    else
      printf "$BOLDGREEN%s$RESET" "✓ "
    fi
    return
  fi

  echo 'exit' > $pipe 
  sleep 0.01
  wait $!
  ES_1=$?
  TEST1=$(cat msh_log)

  # execute commands in bash
  bash < $pipe >msh_log 2>&-  &
  IFS=';' read -ra CMND <<< "$@"
  for command in "${CMND[@]}"; do
    echo $command > $pipe
  done
  echo 'exit' > $pipe
  wait $!
  ES_2=$?
  TEST2=$(cat msh_log)

  # compare result
  if [ "$TEST1" == "$TEST2" ] && [ "$ES_1" == "$ES_2" ]; then
    printf "$BOLDGREEN%s$RESET" "+ "
    ((GOOD++))
  else
    printf "\n$BOLDRED%s$RESET" "- "
    printf "$CYAN \"$@\" $RESET"
  fi
  if [ "$TEST1" != "$TEST2" ]; then
    echo
    printf $BOLDRED"Your output : \n%.20s\n$BOLDRED$TEST1\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
    printf $BOLDGREEN"Expected output : \n%.20s\n$BOLDGREEN$TEST2\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
    echo
  fi
  if [ "$ES_1" != "$ES_2" ]; then
    echo
    printf $BOLDRED"Your exit status : $BOLDRED$ES_1$RESET\n"
    printf $BOLDGREEN"Expected exit status : $BOLDGREEN$ES_2$RESET\n"
    echo
  fi
  ((TOTAL++))
  sleep 0.08
}

if [ "$1" == "" ] || [ "$1" == "help" ]; then
  printf "$BOLDMAGENTA\n\tAvailable arg: \n$YELLOW\tall$RESET echo cd pipe env export redirect multi syntax exit\n\
      $GREEN\tbonus$RESET quote wildcard oper\n"
fi
if [ "$1" == "all" ]; then
  printf "$BOLDMAGENTA    _____ _ _ _____ _____ _ _ ______ _ _ \n"
  printf "| \/ |_ _| \ | |_ _|/ ____| | | | ____| | | | \n"
  printf "| \ / | | | | \| | | | | (___ | || | |  | | | | \n"
  printf "| |\/| | | | | . \` | | | \___ \|   | | | | | | \n"
  printf "| | | |_| |_| |\ |_| |_ ____) | | | | |____| |____| |____ \n"
  printf "|_| |_|_____|_| \_|_____|_____/|_| |_|______|______|______|\n$RESET"
fi

if [ "$2" == "leaks" ]; then
  printf $BOLDBLUE"\n\tONLY LEAKS MODE\n"$RESET
  LEAKSMODE=1
  rm -f leaks
fi

MY TESTS
if [ "$1" == "my" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tUTYGETT TESTS\n"$RESET
  exec_test 'pwd'
  exec_test 'ls'
  exec_test 'echo'
  exec_test ''
  exec_test '     '
  exec_test '     '
  exec_test '/bin/ls'
  exec_test '/bin/ls/lfkfh'
  exec_test '/bin/echo'
  exec_test '/bin/pwd'
  exec_test 'exit'
  exec_test 'exit q'
  exec_test 'exit 555'
  exec_test 'exit 54567878098765432987654321'
  exec_test 'exit 4567 5678'
  exec_test '/bin/ls retyu'
  exec_test 'cat test'
  exec_test 'pwd ghjk'
  # exec_test 'export wewe' // !!!!
  exec_test 'sdfghjk'
  exec_test '$? + $?'
  exec_test 'echo "cat lol.c | cat > lol.c"'
  exec_test '"pwd"'
  exec_test '"env "'
  exec_test 'echo '$USER''
  exec_test 'unset ffff'
  exec_test 'cd /bin/ls' // !!!
  exec_test 'cat file | grep bla | more'
  exec_test 'ls filethatdoesntexist | grep bla | more'
  exec_test '$? + $?'
  exec_test 'echo "cat lol.c | cat > lol.c"'
  exec_test 'echo 1 | cat -e'
  exec_test 'echo 1 | cat -e'
fi

# ECHO TESTS
if [ "$1" == "echo" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tECHO TESTS\n"$RESET
  exec_test 'echo test tout'
  exec_test 'echo test tout'
  exec_test 'echo -n test tout'
  exec_test 'echo -n -n -n test tout'
  exec_test 'echo -n -n -n bonjour'
  exec_test 'echo "-n" bonjour'
  exec_test 'echo -n"-n" bonjour'
  exec_test 'echo "-nnnn" bonjour'
  exec_test 'echo "-n -n -n"-n bonjour'
  exec_test 'echo "-n -n" bonjour'
  exec_test 'echo ~'
  exec_test "echo '~'"
  exec_test 'echo "~"'
  exec_test 'echo ~123'
  exec_test 'echo 123'
	exec_test "echo"
	exec_test "echo echo"
	exec_test "echo rhobebou"
	exec_test "echo stop barking"
	exec_test 'echo "bonjour"'
	exec_test 'echo bonjour'
	exec_test "echo 'bonjour'"
	exec_test 'echo -n bonjour'
	exec_test "echo -nn bonjour"
	exec_test "echo -n -n -n bonjour"
	exec_test "echo -n -n -n bonjour"
	exec_test 'echo "-n" bonjour'
	exec_test 'echo -n"-n" bonjour'
	exec_test 'echo "-nnnn" bonjour'
	exec_test 'echo "-n -n -n"-n bonjour'
	exec_test 'echo "-n '-n'" bonjour'
	exec_test 'echo ~'
	exec_test 'echo "~"'
	exec_test "echo '~'"
	exec_test 'echo ~123'
	exec_test 'echo 123~'
	exec_test 'echo $USER'
	exec_test 'echo "$USER"'
	exec_test 'echo "'$USER'"'
	exec_test 'echo " '$USER' "'
	exec_test 'echo text"$USER"'
	exec_test 'echo text"'$USER'" ' $USER ''
	exec_test 'echo "text"   "$USER"    "$USER"'
	exec_test "echo '              $USER          '"
	exec_test 'echo               text "$USER"            "$USER"text'
	exec_test "echo ''''''''''$USER''''''''''"
	exec_test 'echo """"""""$USER""""""""'
	exec_test "echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER ''"
	exec_test "echo $USER '' $USER $USER '' $USER '' $USER -n $USER"
	exec_test 'echo "$USER""$USER""$USER"'
	exec_test 'echo text"$USER"test'
	exec_test 'echo $USER " "'
	exec_test 'echo "$USER$USER$USER"'
	exec_test 'echo " $USER  "'$PWD''
	exec_test 'echo " $USER  "'$PWD''
	exec_test 'echo $USER=4'
	exec_test 'echo $USER=thallard'
	exec_test 'echo $USER'
	exec_test 'echo $?'
	exec_test 'echo $USER213'
	exec_test 'echo $USER$12USER$USER=4$USER12'
	exec_test 'echo $USER $9999USER $8888USER $7777USER'
	exec_test 'echo "$USER=12$USER"'
	exec_test 'echo "$9USER" "'$USER=12$SOMETHING'"'
	exec_test 'echo "text" "text$USER" ... "$USER"'
	exec_test 'echo $PWD'
fi

# CD TESTS
if [ "$1" == "cd" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tCD TESTS\n"$RESET
  exec_test 'cd /Users'
  exec_test 'cd ../'
  exec_test 'cd .'
  exec_test "cd"
  exec_test "cd .."
  exec_test "cd . "
  exec_test "cd /Users"
  exec_test "cd '/'"
  exec_test "cd //////"
  exec_test "cd ./././"
  exec_test "cd /"
  exec_test "cd '/home'"
  exec_test 'cd "$PWD/file_tests"' 
  exec_test 'cd "doesntexist"' 
  exec_test 'cd "doesntexist" 2>/dev/null' 
  exec_test "cd ../../.."
  exec_test "cd "wtf" 2>/dev/null"
  exec_test "cd .."
  exec_test "cd .."
  exec_test "cd ? "
  exec_test "cd + "
  exec_test "cd woof"
  exec_test "cd bark bark"
  exec_test "cd '/'"
  exec_test "cd $PWD/file_tests"
  exec_test "cd $OLDPWD/something"
fi


# ERROR TESTS
if [ "$1" == "error" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tERROR TESTS\n"$RESET
exec_test "test"
exec_test "| test"
exec_test "echo > <"
exec_test "echo | |"
exec_test "<"
exec_test "|"
exec_test "| | |"
exec_test "hello world"
exec_test "||||||||"
exec_test "            "
exec_test "cat wouaf wouaf"
exec_test ">"
exec_test ">>>>>>>>"
exec_test "<<<<<<<<<"
exec_test "> > > >"
exec_test ">> >> >> >>"
exec_test "~"
exec_test "<<"
exec_test "/Users"
exec_test " "
exec_test "/"
exec_test '\\\'
exec_test "EechoE"
exec_test ".echo."
exec_test ">echo>"
exec_test "<echo<"
exec_test ">>echo>>"
exec_test "echo"
exec_test "|echo|"
exec_test "echo -n"
exec_test "echo -n"
exec_test "echo"
exec_test "echo something"
exec_test "rm -f something"
exec_test "cat something"
exec_test "| echo -n oui"
exec_test "echo -n oui"
exec_test "trying to destroy your minishell"
exec_test "trying something again echo if you see this message thats not a good new"
exec_test "qewew"
exec_test "wtf"
exec_test "hi im thallard"
exec_test "nice to meet you if these tests are green"
exec_test "your minishell is perfect"
fi


# PIPE TESTS
if [ "$1" == "pipe" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tPIPE TESTS\n"$RESET
  exec_test '/bin/ls'
  exec_test 'cat tests/lorem.txt | grep arcu | cat -e'
  #exec_test 'echo test | cat -e | cat -e | cat -e |cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e|cat -e'
  exec_test 'cat /dev/random | head -c 100 | wc -c'
  exec_test 'ls|ls|ls|ls|lls|ls|ls|ls|ls|ls'
  exec_test 'head -c 2000000 /dev/zero > big_file'
  exec_test ' cat big_file | ls'
  exec_test ' cat big_file | echo lol'
  exec_test ' echo oui | cat -e'
  exec_test 'echo oui | echo non | echo something | grep oui'
  exec_test 'echo oui | echo non | echo something | grep non'
  exec_test 'echo oui | echo non | echo something | grep something'
  exec_test 'cd .. | echo "something"'
  exec_test 'cd .. | echo "something"'
  exec_test 'cd / | echo "something"'
  #exec_test 'cd .. | pwd'
  exec_test 'ifconfig | grep ":"'
  exec_test 'ifconfig | grep nothing'
  exec_test 'whoami | grep $USER'
  exec_test 'cat Makefile | grep "FLAGS"'
  exec_test 'cat Makefile | cat -e | cat -e'
  exec_test 'cat Makefile | grep "FLAGS" | grep "FLAGS" | cat -e'
  exec_test 'export TEST=123 | cat -e | cat -e'
  exec_test 'unset TEST | cat -e'
  exec_test 'echo test | cat -e | cat -e | cat -e'
  exec_test 'whereis ls | cat -e | cat -e > test'
  exec_test 'echo test | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e | cat -e'
  exec_test 'ls -la | grep "."'
fi


# ENV EXPANSIONS
if [ "$1" == "env" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tENV EXPANSIONS TESTS\n"$RESET
	exec_test 'echo test test'
	exec_test 'echo test'
	exec_test 'echo $TEST'
	exec_test 'echo "$TEST"'
	exec_test "echo '$TEST'"
	exec_test 'echo "$TEST$TEST$TEST"'
	exec_test 'echo "$TEST$TEST=lol$TEST"'
	exec_test 'echo " $TEST lol $TEST"'
	exec_test 'echo $TEST$TEST$TEST'
	exec_test 'echo $TEST$TEST=lol$TEST""lol'

	exec_test 'echo test "$TEST" test "$TEST " test'
	exec_test 'echo "$"'
	exec_test 'echo "$?TEST"'
	exec_test 'echo "$1TEST"'
	exec_test 'echo "$T1TEST"'
fi

# EXPORT
if [ "$1" == "export" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tEXPORT TESTS\n"$RESET
  ENV_SHOW="env | sort | grep -v SHLVL | grep -v _="
  EXPORT_SHOW="export | sort | grep -v SHLVL | grep -v _= | grep -v OLDPWD"
  exec_test 'export = '  
  exec_test 'export 1TEST= ' 
  exec_test 'export TEST ' 
  exec_test 'export ""=""  '
  exec_test 'export TES=T=""' 
  exec_test 'export TE+S=T=""' 
  #exec_test 'export | grep "SHLVL"'
  exec_test 'export $? '
  exec_test 'export TEST'
  exec_test 'export TEST='
  exec_test 'export TEST=123'
  exec_test 'export ___TEST=123'
  exec_test 'export ""=""'
  exec_test 'export ''='''
  exec_test 'export "="="="'
  exec_test "export '='='='"
  exec_test 'export TE\\\ST=100'
  exec_test 'export TEST-=100'
  exec_test 'export _TEST=100'
  #exec_test 'export | grep "SHLVL"'
  exec_test 'export TEST'
  exec_test 'export =========='
  exec_test 'export 1TEST='
  exec_test 'export TEST'
  exec_test 'export ""=""'
  exec_test 'export TES=T=""'
  exec_test 'export TE+S=T=""'
  exec_test 'export TES\\\\T=123'
  exec_test 'export TES.T=123'
  exec_test 'export TES\\\$T=123'
  exec_test 'export TES\\\\T'
  exec_test 'export TES.T=123'
  exec_test 'export TES+T=123'
  exec_test 'export TES=T=123'
  exec_test 'export TES}T=123'
  exec_test 'export TES{T=123'
  exec_test 'export TES-T=123'
  exec_test 'export _TEST=123'
  exec_test 'export TES_T=123'
  exec_test 'export TEST_=123'
  exec_test 'export TE*ST=123'
  exec_test 'export TES#T=123'
  exec_test 'export TES@T=123'
  exec_test 'export TES!T=123'
  exec_test 'export TES$?T=123'
  exec_test 'export =============123'
  exec_test 'export +++++++=123'
  exec_test 'export ________=123'
  exec_test 'export export'
  exec_test 'export echo'
  exec_test 'export pwd'
  exec_test 'export cd'
  exec_test 'export export'
  exec_test 'export unset'
  exec_test 'export sudo'
  exec_test 'export TES^T=123'
  exec_test 'export TES!T=123'
  exec_test 'export TES\~T=123'

fi

if [ "$1" == "unset" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tUNSET TESTS\n"$RESET
exec_test "unset ."
exec_test "unset /"
exec_test "unset ]"
exec_test "unset ["
exec_test "unset ,"
exec_test "unset @"
exec_test "unset +"
exec_test "unset %"
exec_test "unset ^"
exec_test "unset !"
exec_test "unset ?"
exec_test "unset cat=."
exec_test "unset cat=/"
exec_test "unset cat=]"
exec_test "unset cat=["
exec_test "unset cat=;"
exec_test "unset cat=:"
exec_test "unset cat=,"
exec_test "unset cat=@"
exec_test "unset cat=+"
exec_test "unset cat=%"
exec_test "unset cat=^"
exec_test "unset cat=!"
exec_test "unset cat=?"
exec_test "unset _cat"
exec_test "unset %cat"
exec_test "unset 4cat"
exec_test "unset ca_t"
exec_test "unset ca%t"
exec_test "unset ca4t"
exec_test "unset _cat=wwooff"
exec_test "unset cat=_wwooff"
exec_test "unset %cat=wwooff"
exec_test "unset cat=%wwooff"
exec_test "unset cat=wwooff"
exec_test "unset cat=%wwooff"
exec_test "unset 4cat=wwooff"
exec_test "unset cat=4wwooff"
exec_test "unset c+at=wwooff"
exec_test "unset cat=w+wooff"
exec_test "unset ca%t=wwooff"
exec_test "unset cat=wwoo%ff"
exec_test "unset ca_t=wwooff"
exec_test "unset cat=wwo_off"
exec_test "unset ca4t=wwooff"
exec_test "unset cat=wwo4off"
fi

# REDIRECTIONS
if [ "$1" == "redirect" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tREDIRECTION TESTS\n"$RESET
  exec_test 'echo test > ls'
  exec_test 'echo test >> ls'
  exec_test '> lol echo test lol'
  exec_test 'cat < ls'
  exec_test 'cat <ls'
  exec_test 'echo 1 > 1'
  exec_test 'echo 1 > 1 > 2'
  exec_test 'echo 1 > 1 > 2 > 3'
  exec_test 'echo 1 > 1 > 2 > 3'
  exec_test 'echo 1 > 1 > 2 > 3 > 4'
  exec_test 'pwd > pwd'
fi


# MULTI TESTS
# if [ "$1" == "multi" ] || [ "$1" == "all" ]; then
#   printf $BOLDMAGENTA"\n\tMULTI TESTS\n"$RESET
#   exec_test 'echo "test 1  | and 2" >>lol && cat <lol && cat Makefile <lol | grep minishell && rm -rf lol'
#   exec_test 'unset PATH && /bin/ls'
#   exec_test 'unset PATH && ./Makefile'
#   exec_test 'echo 5 > ls && < 5 cat'
#   exec_test 'ls | echo 6 > ls && <6 cat'
#   exec_test 'cd && unset HOME && cd'
#   exec_test 'cd .. && pwd'
#   exec_test 'cd .. > 1 | pwd'
#   exec_test 'pwd > 1'
#   exec_test 'pwd > 1 && cat 1'
#   exec_test 'pwd > 1 && pwd'
#   exec_test 'pwd > 1 | pwd'
#   exec_test 'pwd && unset HOME && pwd'
#   exec_test 'ls | export TEST=5 && echo $TEST'
#   exec_test 'export TEST1=LOL TEST2=PIKAPIKA'
#   exec_test 'unset TEST1 TEST2'
#   exec_test 'echo $TEST1'
#   exec_test 'echo $TEST2'
# fi

# SYNTAX 
if [ "$1" == "syntax" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tSYNTAX\n"$RESET
  exec_test ''
  exec_test '" echo" lol'
  exec_test '"echo " lol'
  exec_test '" echo" | " pwd"'
  exec_test '"echo " | "pwd "'
  exec_test '| test'
  exec_test 'echo > <'
  exec_test 'echo | |'
  exec_test 'echo "||"'
  exec_test '<'
  exec_test "grep -z"
  exec_test "ls'| 'wc -l"
fi

# EXIT
# if [ "$1" == "bonus" ] || [ "$1" == "all" ]; then
#   printf $BOLDMAGENTA"\n\tEXIT\n"$RESET
#   exec_test "exit 42"
#   exec_test "exit 42 53 68"
#   exec_test "exit 259"
#   exec_test "exit 9223372036854775807"
#   exec_test "exit -9223372036854775808"
#   exec_test "exit -4"
#   exec_test "exit wrong"
#   exec_test "exit  2 wrong_command"
#   exec_test "gdagadgag"
#   exec_test "ls -Z"
#   exec_test "cd gdhahahad"
#   exec_test "ls -la | wtf || echo $?"
# fi

# BONUS QUOTES
# if [ "$1" == "bonus" ] || [ "$1" == "all" ]; then
#   printf $BOLDMAGENTA"\n\tBONUS QUOTE\n"$RESET
#   exec_test "echo '"$USER"'"
#   exec_test "echo "'$USER'""
# fi

BONUS WILDCARD
if [ "$1" == "bonus" ] || [ "$1" == "all" ]; then
  printf $BOLDMAGENTA"\n\tBONUS WILDCARD\n"$RESET
  exec_test "echo * | wc"
  exec_test "echo * | cat -e"
  exec_test "echo *"
  exec_test "mkdir empty && cd empty && touch 1 lol help me && echo * && cd .. && rm -rf empty"
fi

# BONUS OPERATOR && || ()
# if [ "$1" == "bonus" ] || [ "$1" == "all" ]; then
#   printf $BOLDMAGENTA"\n\tBONUS OPERATOR \$\$ || () \n"$RESET
  
#   exec_test "true && ls"
#   exec_test "false&&ls"
#   exec_test "true||ls"
#   exec_test "false || ls"
#   exec_test "true || echo 1 && echo 2"
#   exec_test "false || echo 1 && echo 2"
#   exec_test "true || (echo 1 && echo 2)"
#   exec_test "true || echo 1 && echo 2 || echo 3"
#   exec_test "(ls)"
#   exec_test "( ls )"
#   exec_test "true || (echo 1 && echo 2) || echo 3"
#   exec_test "true || (echo 1 && echo 2) && echo 3"
#   exec_test "(true || (echo 1 && echo 2) && echo 3)"
#   exec_test "true || ((echo 1 && echo 2) && echo 3)"
#   exec_test "()"
#   exec_test " ls )"
#   exec_test "touch 1 && (ls && pwd) && rm -rf 1"
# fi


if [[ "$1" != "" ]] && (( $TOTAL > 0)); then
  PROCENT=$(( $GOOD * 100  / $TOTAL ))
  if (( $PROCENT > 90)); then
    printf $GREEN
  elif (( $PROCENT <= 90 )) && (( $PROCENT > 60 )); then
    printf $YELLOW
  else  
    printf $RED
  fi
  printf "\nPASS: $GOOD / $TOTAL ($PROCENT%%)$RESET\n"
fi

rm -f $pipe lol ls 1 test big_file msh_log leaks pwd