header = ""
for i in range(0, 100000):
	header += "/* padding contents 387654269458609540685467 */\n"
for i in "efghijklmnopqrstuvwxyz":
	with open("%s.js" % i, "w") as f:
		f.write(header)
		f.write("out++;")
	print('<script src="http://localhost:8000/%s.js"></script>' % i)
