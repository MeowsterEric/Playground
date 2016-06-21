#!/bin/sh

yes_or_no() {
    line=""

    while [ "$line" != "y" -a "$line" != "yes" -a "$line" != "n" -a "$line" != "no" ]
    do
        read line || exit 0
    done

    [ "$line" = "y" -o "$line" = "yes" ]
}

# initial input checking
if [ $# -ne 1 ]
then
    echo "Usage: $0 db" >&2
    exit 1
fi

set -e   # Exit immediately if a command exits with a non-zero status
cd "$1"

echo "Think of an animal."
while [ -d "yes" ]
do
    cat "question"
    if yes_or_no
    then
	    cd "yes"
    else
	    cd "no"
    fi
done


echo "Is your animal a `cat animal`?"
if yes_or_no
then
    echo "I got it!"
else
    echo "Aw, I lose. What animal were you thinking of?"
    # Don't make any changes until we've received all user input data,
    # so as to keep the database valid in case of eof in the middle of
    # the interaction
    animal=""
    while [ -z "$animal" ]
    do
	    read animal || exit 0
    done

    echo "Tell me a question which would distinguish a `cat animal` from a $animal."
    question=""
    while [ -z "$question" ]
    do
	    read question || exit 0
    done

    echo "What would be the answer for a $animal?"
    if yes_or_no
    then
	    mkdir yes no
	    echo "$animal" >yes/animal
	    mv animal no/animal
    else
	    mkdir yes no
	    echo "$animal" >no/animal
	    mv animal yes/animal
    fi

    echo "$question" >question
    echo "I'll remember that."
fi
