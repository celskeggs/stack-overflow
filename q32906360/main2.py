#import io
#io.DEFAULT_BUFFER_SIZE = 70000
#fp = open('/dev/urandom', 'rb')
# fp = fp.detach()
# fp = _io.BufferedReader(fp, 70000)

import _io
fp = _io.BufferedReader(_io.FileIO("/dev/urandom", "rb"), 30000)
ans = fp.read(65600)
fp.close()
