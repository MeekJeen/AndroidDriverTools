#!/bin/bash

#1.给print.ko签名
perl Sign_files/sign-file sha512 Sign_files/signing_key.priv Sign_files/signing_key.x509 print.ko

#2.查看prin.ko
file print.ko && hexdump -C print.ko | tail


