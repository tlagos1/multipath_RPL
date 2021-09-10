#!/bin/bash

testseq="<ratio>"
while IFS='' read -r line || [[ -n "$line" ]]; do
	if [[ $line =~ $testseq ]]; then
		random_link="0.$(shuf -i 50-99 -n 1)" 
		line=$(echo "$line" | sed "s/\(<ratio.*>\)[^<>]*\(<\/ratio.*\)/\1$random_link\2/g" ) 
	fi
	echo "$line" >> case6a_randomSeed.csc
done < "case6a.csc"
