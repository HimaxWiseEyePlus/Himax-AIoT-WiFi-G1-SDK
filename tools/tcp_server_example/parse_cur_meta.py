import binascii



def parse_meta(data):
	#print(binascii.hexlify(data))
	human_presence = int.from_bytes(data[252:253], byteorder='little')
	num_of_track = int.from_bytes(data[250:252], byteorder='little')
	#print(num_of_track)
	hts = []
	for i in range(num_of_track):
		offset = 256 + i * 72
		x = int.from_bytes(data[offset:offset+4], byteorder='little')
		y = int.from_bytes(data[offset+4:offset+8], byteorder='little')
		w = int.from_bytes(data[offset+8:offset+12], byteorder='little')
		h = int.from_bytes(data[offset+12:offset+16], byteorder='little')
		score = int.from_bytes(data[offset+20:offset+24], byteorder='little')
		#print('x = %d, y = %d, w = %d, h = %d, score = %d' % (x, y, w, h, score))
		ht = {'x':x, 'y':y, 'w':w, 'h':h, 'score':score}
		hts.append(ht)
	result = {'human_presence':human_presence, 'num_of_track': num_of_track, 'hts':hts}
	return result

if __name__ == '__main__':
	metadata = bytearray(b'')
	file = open('amr.meta', 'rb')
	buf = file.read(1664)
	file.close()
	result = parse_meta(buf)
	print(result)