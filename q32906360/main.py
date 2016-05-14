fp = open('/dev/urandom', 'rb')
ans = fp.read(65600)
fp.close()
