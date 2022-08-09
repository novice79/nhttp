#!/usr/bin/env bash

mkdir -p misc
openssl req -new -newkey rsa:2048 -days 3650 -nodes -x509 \
-subj "/C=CN/ST=HuNan/L=ChangSha/O=py/CN=localhost" \
-addext 'subjectAltName = "DNS:foo.co.uk,DNS:127.0.0.1,DNS:example.com,DNS:example.net"' \
-keyout misc/key.pem -out misc/cert.pem

# openssl req -x509 -newkey rsa:2048 -sha256 -days 3650 -nodes \
#   -keyout example.key -out example.crt -subj '/CN=example.com' \
#   -addext 'subjectAltName=DNS:example.com,DNS:example.net'