#!/bin/bash

login=`cat .ssh/login`
password=`cat .ssh/pass`

sshpass -p $password scp -r $1 $login@samos.dozen.mephi.ru:$1
