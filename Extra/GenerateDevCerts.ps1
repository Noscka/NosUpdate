openssl req -newkey rsa:4096 -nodes -keyout server.key -x509 -days 365 -out server.crt -subj "/C=PL/ST=Poland/L=Poland/O=NosTeck/OU=HighDeparment/CN=NosTeck.com"
Get-Content server.crt, server.key | Set-Content Server/server.pem
mv -Force server.crt Updater/
rm server.key
openssl dhparam -out Server/dh2048.pem 2048