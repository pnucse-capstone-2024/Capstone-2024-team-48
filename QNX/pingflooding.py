$ip = "192.168.56.101" # 핑을 보낼 IP 주소
for ($i=0; $i -lt 10000; $i++) {
    ping -n 1 $ip
}
